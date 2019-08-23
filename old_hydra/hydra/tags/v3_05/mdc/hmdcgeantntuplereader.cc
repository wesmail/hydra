#include "hmdcgeantntuplereader.h"
#include "hmdcdef.h"
#include "hdebug.h"
#include "hades.h"
#include "hmdcgeantcell.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "hcategory.h"
#include "hmdclayergeompar.h"
#include "hdatasource.h"
#include <iostream.h>

//*-- Author : Alexander Nekhaev
//*-- Modified : 02/09/99 by V. Pechenov
//*-- Modified : 05/08/99 by R. Holzmann
//*-- Modified : 18/06/99 by Ilse Koenig

////////////////////////////////////////////////////////////////
//
//  HMdcGeantReader reads geant ntupls after h2root conversion,  
//  makes preliminary transformation before digitization procedure
//  and stores information in MdcGeantCell category.
//  
////////////////////////////////////////////////////////////////

ClassImp(HMdcGeantNtupleReader)



HMdcGeantNtupleReader::HMdcGeantNtupleReader(void) {
  fEventId = 0;
}


Bool_t HMdcGeantNtupleReader::init(void) {
  setParContainers();
  fGeantCellCat=gHades->getCurrentEvent()->getCategory(catMdcGeantCell);
  if (!fGeantCellCat) {
    fGeantCellCat=gHades->getSetup()->getDetector("Mdc")->buildCategory(catMdcGeantCell);
    if (!fGeantCellCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catMdcGeantCell,fGeantCellCat,"Mdc");
  }
  loc.set(4,0,0,0,0);
  //  fActive=kTRUE;

  HLocation loc;
  char* trname[24] = { "h3011","h3012","h3013","h3014",
		       "h3021","h3022","h3023","h3024",
		       "h3031","h3032","h3033","h3034",
		       "h3041","h3042","h3043","h3044",
		       "h3051","h3052","h3053","h3054",
		       "h3061","h3062","h3063","h3064" };
    
  loc.set(4,0,0,0,0);
  Int_t i;
  for(i=0;i<24;i++) tr[i]=(TTree*)fInputFile->Get(trname[i]);
  for(i=0;i<24;i++) {
    tr[i]->SetBranchAddress("Idevt",&mdc.idevt);
    tr[i]->SetBranchAddress("Nhit",&mdc.nhit);
    tr[i]->SetBranchAddress("Mdctrk",mdc.mdctrk);
    tr[i]->SetBranchAddress("Mdcx",mdc.mdcx);
    tr[i]->SetBranchAddress("Mdcy",mdc.mdcy);
    tr[i]->SetBranchAddress("Mdctheta",mdc.mdctheta);
    tr[i]->SetBranchAddress("Mdcphi",mdc.mdcphi);
    tr[i]->SetBranchAddress("Mdctof",mdc.mdctof);
  }

  return kTRUE;
}


Bool_t HMdcGeantNtupleReader::execute(void) {

#if DEBUG_LEVEL>2
  fprintf(stderr,"\rProcessing event: %d", fEventId+1);   // event counter
#endif

  for(loc[0]=0;loc[0]<6;loc[0]++) {
    for(loc[1]=0;loc[1]<4;loc[1]++) {
      Int_t n = loc[0]*4+loc[1];
//*********** the return type is a Bool_t !!!!
//    if (tr[n]->GetEvent(fEventId)==0) return kDsEndData;  // end of tree
      if (tr[n]->GetEvent(fEventId)==0) return kFALSE;  // end of tree
      Int_t j;
      for(j=0;j<mdc.nhit;j++) {
	for(loc[2]=0;loc[2]<6;loc[2]++) {
// loc[0] - sector 
// loc[1] - module
// loc[2] - layer
// loc[3] - cell
	  Float_t theta = mdc.mdctheta[j][loc[2]];
	  Float_t phi = mdc.mdcphi[j][loc[2]];
	  Float_t xcoord = mdc.mdcx[j][loc[2]];
	  Float_t ycoord = mdc.mdcy[j][loc[2]];
	  Float_t tof = mdc.mdctof[j][loc[2]];
	  Int_t trkNum = mdc.mdctrk[j];
	  transform(xcoord,ycoord,theta,phi,tof,trkNum);	  
	}
      }
    }
  }
  
  fEventId++;

#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HMdcGReader::execute");
#endif
  return kTRUE;
}

    




