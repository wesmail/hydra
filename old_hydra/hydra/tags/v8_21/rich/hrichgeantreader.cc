using namespace std;
#include "hrichgeantreader.h"
#include "hgeantrich.h"
#include "hgeantmaxtrk.h"
#include "richdef.h"
#include "hdebug.h"
#include "hades.h"
#include "hevent.h"
#include "hcategory.h"
#include "hdatasource.h"
#include "TDirectory.h"
#include <iostream> 
#include <iomanip>

//*-- Author : R. Holzmann
//*-- Modified: 05/12/2008 by I. Koenig

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////
//  HRichGeantReader for HGeant Root RICH branches
//
///////////////////////////////////////////////////

ClassImp(HRichGeantReader)

HRichGeantReader::HRichGeantReader(void) {
  fEventId = 0;
}

HRichGeantReader::~HRichGeantReader(void) {
}

Bool_t HRichGeantReader::init(void) {
//  Set up categories and HGeant input tree
//  (this still needs splitlevel=2 on input file)
//
  fGeantRichPhotCat = (HMatrixCategory*)(gHades->getCurrentEvent()
                             ->getCategory(catRichGeantRaw));  
  fGeantRichDirCat = (HMatrixCategory*)(gHades->getCurrentEvent()
                             ->getCategory(catRichGeantRaw+1));  
  fGeantRichMirrorCat = (HMatrixCategory*)(gHades->getCurrentEvent()
                             ->getCategory(catRichGeantRaw+2));  
  if (!fGeantRichPhotCat || !fGeantRichDirCat || !fGeantRichMirrorCat) {
     if (!fInputFile) {
        t = NULL;
        return kFALSE;   // no category, no input file !
     }
     if (fInputFile->cd("dirSimul")) { // change dir, read category definition
        fGeantRichPhotCat = (HMatrixCategory*)(gDirectory->Get("HGeantRichPhoton"));
        fGeantRichDirCat = (HMatrixCategory*)(gDirectory->Get("HGeantRichDirect"));
        fGeantRichMirrorCat = (HMatrixCategory*)(gDirectory->Get("HGeantRichMirror"));
        fInputFile->cd("..");        // go back up one level
     } else {
        Int_t ini[2]={6,MAXCKOVRICH};   // use old default definition 
        fGeantRichPhotCat = new HMatrixCategory("HGeantRichPhoton",2,ini,1.0);
        Int_t ini1[2]={6,MAXPARTRICH};
        fGeantRichDirCat = new HMatrixCategory("HGeantRichDirect",2,ini1,1.0);
        Int_t ini2[2] = {6,MAXMIRRRICH};
        fGeantRichMirrorCat = new HMatrixCategory("HGeantRichMirror",2,ini2,1.0);
     }
     gHades->getCurrentEvent()->
               addCategory(catRichGeantRaw,fGeantRichPhotCat,"Simul");
     gHades->getCurrentEvent()->
               addCategory(catRichGeantRaw+1,fGeantRichDirCat,"Simul");

     printf("\n%s(%d,%d) matrix category created for HGeant input\n",
              fGeantRichPhotCat->getClassName(),
              fGeantRichPhotCat->getSize(0), fGeantRichPhotCat->getSize(1));
     printf("%s(%d,%d) matrix category created for HGeant input\n",
              fGeantRichDirCat->getClassName(),
              fGeantRichDirCat->getSize(0), fGeantRichDirCat->getSize(1));

     if (fGeantRichMirrorCat) {
       gHades->getCurrentEvent()->
                 addCategory(catRichGeantRaw+2,fGeantRichMirrorCat,"Simul");
       printf("%s(%d,%d) matrix category created for HGeant input\n",
                fGeantRichMirrorCat->getClassName(),
                fGeantRichMirrorCat->getSize(0), fGeantRichMirrorCat->getSize(1));
     }

  }
  if (!fInputFile) {
     t = NULL;
     return kTRUE;   // category exists, but no input file (called from HGeant)
  }

  if ((t=(TTree*)fInputFile->Get("T")) == NULL) return kFALSE;

// connect the two RICH categories now
  t->SetBranchAddress("HGeantRichPhoton",&fGeantRichPhotCat);
  t->SetBranchStatus("HGeantRichPhoton",kTRUE);
  fGeantRichPhotCat->activateBranch(t,2);
  t->SetBranchAddress("HGeantRichDirect",&fGeantRichDirCat);
  t->SetBranchStatus("HGeantRichDirect",kTRUE);
  fGeantRichDirCat->activateBranch(t,2);
  t->SetBranchAddress("HGeantRichMirror",&fGeantRichMirrorCat);
  t->SetBranchStatus("HGeantRichMirror",kTRUE);
  fGeantRichPhotCat->activateBranch(t,2);
  
  if (gHades->getOutputFile() != NULL) gHades->getOutputFile()->cd(); // !!!
  return kTRUE;
}


Bool_t HRichGeantReader::execute(void) {
//  Read one entry from tree 
//
  if(t) {   // input tree does exist
     if(t->GetEntry(fEventId)==0) return kFALSE;  // read 1 entry from tree
  }

  fEventId++;

  return kTRUE;  // return type is Bool_t
}



