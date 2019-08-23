#include "hmdcgeantrootreader.h"
#include "hgeantmdc.h"
#include "hmdcdef.h"
#include "hdebug.h"
#include "hades.h"
#include "hmdcgeantcell.h"
#include "hmatrixcatiter.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "hcategory.h"
#include "hlocation.h"
#include "hmdclayergeompar.h"
#include "hdatasource.h"
#include "hindextable.h"
#include <TDirectory.h>
#include <iostream.h>

//*-- Author : A.Nekhaev
//*-- Modified: 20/08/99 by R. Holzmann
//*-- Modified: 18/06/99 by Ilse Koenig
//*-- Modified: 29/06/99 by A.Nekhaev

///////////////////////////////////////////////////
//  HMdcGeantRootReader for HGeant Root MDC branch
//
///////////////////////////////////////////////////

ClassImp(HMdcGeantRootReader)

HMdcGeantRootReader::HMdcGeantRootReader(void) {
  fEventId = 0;
  pi=acos(-1)/180;
}

HMdcGeantRootReader::~HMdcGeantRootReader(void) {
}


Bool_t HMdcGeantRootReader::init(void) {
//  Set up categories and HGeant input tree
//  (this still needs splitlevel=2 on input file)
//
  setParContainers();
  fGeantCellCat=gHades->getCurrentEvent()->getCategory(catMdcGeantCell);
  if (!fGeantCellCat) {
    fGeantCellCat=gHades->getSetup()->
                    getDetector("Mdc")->buildCategory(catMdcGeantCell);
    if (!fGeantCellCat)
       return kFALSE;
    else
       gHades->getCurrentEvent()->
                    addCategory(catMdcGeantCell,fGeantCellCat,"Mdc");
  }
  fGeantCellCat->setPersistency(kFALSE);  // don't want to write this one

  fGeantMdcCat = (HMatrixCategory*)(gHades->getCurrentEvent()
                          ->getCategory(catMdcGeantRaw));  
  if (!fGeantMdcCat) {
     if (!fInputFile) {
        t = NULL;
        return kFALSE;   // no category, no input file !
     }
     if (fInputFile->cd("dirSimul")) { // change dir, read category definition
        fGeantMdcCat = (HMatrixCategory*)(gDirectory->Get("HGeantMdc"));
        fInputFile->cd("..");        // go back up one level
     } else {
        Int_t ini[4] = {6,4,6,200};   // use old default definition 
        fGeantMdcCat = new HMatrixCategory("HGeantMdc",4,ini,1.0);
     }
     gHades->getCurrentEvent()->
               addCategory(catMdcGeantRaw,fGeantMdcCat,"Simul");

     printf("\n%s(%d,%d,%d,%d) matrix category created for HGeant input\n",
              fGeantMdcCat->getClassName(),
              fGeantMdcCat->getSize(0), fGeantMdcCat->getSize(1),
              fGeantMdcCat->getSize(2), fGeantMdcCat->getSize(3));
  }
  if (!fInputFile) {
     t = NULL;
     return kTRUE;   // category exists, but no input file (called from HGeant)
  }

  if ((t=(TTree*)fInputFile->Get("T")) == NULL) return kFALSE;

  t->SetBranchAddress("HGeantMdc",&fGeantMdcCat);   // connect to Mdc category
  t->SetBranchStatus("HGeantMdc",kTRUE);
  fGeantMdcCat->activateBranch(t,2);
  
  if (gHades->getOutputFile() != NULL) gHades->getOutputFile()->cd(); // !!!
  return kTRUE;
}


Bool_t HMdcGeantRootReader::execute(void) {
//  Read one entry from tree and prepare data for the HMdcGeantCell category
//
  if(t) {   // input tree does exist
     if(t->GetEntry(fEventId)==0) return kFALSE;  // read 1 entry from tree
  }

#if DEBUG_LEVEL>2
  fprintf(stderr,"\rProcessing event: %d", fEventId+1);   // event counter
#endif

  HMatrixCatIter next(fGeantMdcCat);  // iterator over HGeantMdcRaw
  HGeantMdc* geant;
  Float_t xcoord, ycoord, tof, theta, phi;
  Int_t trkNum;

  loc.set(4,0,0,0,0);   // location used to fill the HMdcGeantCell category

  while((geant=(HGeantMdc*)next()) != NULL) {   // loop over all entries
     loc[0] = (Int_t)(geant->getSector());
     loc[1] = (Int_t)(geant->getModule());
     loc[2] = (Int_t)(geant->getLayer());
//   loc[3] is filled in transform(...) with the cell number
     geant->getHit(xcoord, ycoord, tof);
     geant->getIncidence(theta, phi);
     trkNum = geant->getTrack();
     if(loc[2]<6) transform(xcoord,ycoord,theta,phi,tof,trkNum);
  }    
  fEventId++;

#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HMdcGReader::execute");
#endif

  return kTRUE;  // return type is Bool_t
}




