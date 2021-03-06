#include "hkinegeantreader.h"
#include "hgeantkine.h"
#include "hgeantheader.h"
#include "hdebug.h"
#include "hades.h"
#include "hevent.h"
#include "hrecevent.h"
#include "hpartialevent.h"
#include "hlinearcategory.h"
#include "hlocation.h"
#include <TDirectory.h>
#include <iostream.h>

//*-- Author : R. Holzmann
//*-- Modified: 19/08/99 by R. Holzmann

/////////////////////////////////////////////////
//  HKineGeantReader for HGeant Root KINE branch
//
/////////////////////////////////////////////////

ClassImp(HKineGeantReader)

HKineGeantReader::HKineGeantReader(void) {
  fEventId = 0;
}

HKineGeantReader::~HKineGeantReader(void) {
}


Bool_t HKineGeantReader::init(void) {
//  Set up categories and HGeant input tree
//  (this still needs splitlevel=2 on input file)
//
  fEventId = 0;
  fGeantKineCat = (HLinearCategory*)(gHades->getCurrentEvent()
                          ->getCategory(catGeantKine));
  if (!fGeantKineCat) {
     if (!fInputFile) {
        t = NULL;
        return kFALSE;   // no category, no input file !
     }
     if (fInputFile->cd("dirSimul")) { // change dir, read category definition
        fGeantKineCat=(HLinearCategory*)gDirectory->Get("HGeantKine");
        fInputFile->cd("..");        // go back up one level
     } else {
        fGeantKineCat = new HLinearCategory("HGeantKine",10000);
     }
     gHades->getCurrentEvent()->
               addCategory(catGeantKine,fGeantKineCat,"Simul");

     printf("\n%s linear category created for HGeant input\n",
              fGeantKineCat->getClassName());
  }
  if (!fInputFile) {
     t = NULL;
     return kTRUE;   // no input file !
  } 

  if ((t=(TTree*)fInputFile->Get("T")) == NULL) return kFALSE;

  t->SetBranchAddress("HGeantKine",&fGeantKineCat); // connect to KINE category
  t->SetBranchStatus("HGeantKine",kTRUE);
  fGeantKineCat->activateBranch(t,2);

  HRecEvent* fEvent = (HRecEvent*)(gHades->getCurrentEvent());
  HGeantHeader* fSimulHeader = (HGeantHeader*)(fEvent->getPartialEvent(catSimul)
                                                     ->getSubHeader());
  if(!fSimulHeader) {
     fEvent->getPartialEvent(catSimul)->setSubHeader(new HGeantHeader());
     fSimulHeader = (HGeantHeader*)(fEvent->getPartialEvent(catSimul)->getSubHeader());
  }
  t->SetBranchAddress("Simul.Header",&fSimulHeader); // connect Simul header explicitly
  t->SetBranchStatus("Simul.Header",1);

  t->SetBranchAddress("Event",&fEvent);   // connect event explicitly to get at header
  t->SetBranchStatus("Event",1);
  
  if (gHades->getOutputFile() != NULL) gHades->getOutputFile()->cd(); // !!!
  return kTRUE;
}


Bool_t HKineGeantReader::execute(void) {
//  Read one entry from tree
//
  if(t) {   // input tree does exist
     if(t->GetEntry(fEventId)==0) return kFALSE;  // read 1 entry from tree
  }

  fEventId++;

  return kTRUE;  // return type is Bool_t
}




