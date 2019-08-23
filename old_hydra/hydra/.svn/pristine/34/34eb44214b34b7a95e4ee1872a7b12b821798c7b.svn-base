//*-- AUTHOR : Jaroslav Bielcik
//*-- Modified : 03/03/2000 by R. Holzmann
//*-- Modified : 17/12/99 by Jaroslav Bielcik

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// HTofHitFSim                                                               //
// This class is derived from HTofHitF and passes track numbers of the       //
// HGeant hit from raw to hit level                                          //
///////////////////////////////////////////////////////////////////////////////

#include "htofhitfsim.h"
#include "hdebug.h"
#include "hades.h"
#include "hspectrometer.h"
#include "htofdetector.h"
#include "hevent.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hlocation.h"
#include "htofrawsim.h"
#include "htofhitsim.h"

ClassImp(HTofHitFSim)

 HTofHitFSim::~HTofHitFSim(void) {
 }

 Bool_t HTofHitFSim::init(void) {
 // initialization of fRawCat and fHitCat containers
 fRawCat =gHades->getCurrentEvent()->getCategory(catTofRaw);
  if (!fRawCat) {
    HTofDetector* tof=(HTofDetector*)(gHades->getSetup()->getDetector("Tof"));
    fRawCat=tof->buildMatrixCategory("HTofRawSim",0.5F);
    if (!fRawCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catTofRaw,fRawCat,"Tof");
  } else {
    if (fRawCat->getClass()!=HTofRawSim::Class()) {
      Error("HTofHitFSim::init()","Misconfigurated input category");
      return kFALSE;
    }
  }
  iter = (HIterator*)fRawCat->MakeIterator("native");

  fHitCat=gHades->getCurrentEvent()->getCategory(catTofHit);
  if (!fHitCat) {
    HTofDetector* tof=(HTofDetector*)(gHades->getSetup()->getDetector("Tof"));
    fHitCat=tof->buildMatrixCategory("HTofHitSim",0.5F);
    if (!fHitCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catTofHit,fHitCat,"Tof");
  } else {
    if (fHitCat->getClass()!=HTofHitSim::Class()) {
      Error("HTofHitfSim::init()","Misconfigurated output category");
      return kFALSE;
    }
  }
 
  fActive = kTRUE;
  return kTRUE;
  }


Int_t  HTofHitFSim::execute() {
// This method executes HTofHitF::execute() and moreover
// it copies the track numbers from the raw to hit the level containers
 
  HTofHitF::execute();  // do work for real data part

  HTofRaw *raw=NULL;
  HTofHit *hit=NULL;

  iter->Reset();
  while((raw = (HTofRaw*)iter->Next()) != NULL) {
    fLoc = iter->getLocation();
    hit = (HTofHit*)fHitCat->getSlot(fLoc);
    if(hit) {
      ((HTofHitSim*)hit)->setNTrack1( ((HTofRawSim*)raw)->getNTrack1() );
      ((HTofHitSim*)hit)->setNTrack2( ((HTofRawSim*)raw)->getNTrack2() );
    }
  }
  return 0;
}

