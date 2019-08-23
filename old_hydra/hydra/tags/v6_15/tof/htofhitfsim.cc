//*-- AUTHOR : Jaroslav Bielcik
//*-- Modified : 13/11/2001 by D. Zovinec
//*-- Modified : 25/05/2000 by R. Holzmann
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

 HTofHitFSim::HTofHitFSim(void){
   fTofSimulation=kTRUE;
 }

 HTofHitFSim::HTofHitFSim(Text_t* name,Text_t* title) : HTofHitF(name,title){
   fTofSimulation=kTRUE;
 }

 HTofHitFSim::~HTofHitFSim(void) {
 }

 Bool_t HTofHitFSim::init(void) {
 // initialization of fRawCat and fHitCat containers
 initParContainer(gHades->getSetup(),gHades->getRuntimeDb());
 fRawCat =gHades->getCurrentEvent()->getCategory(catTofRaw);
  if (!fRawCat) {
    HTofDetector* tof=(HTofDetector*)(gHades->getSetup()->getDetector("Tof"));
    fRawCat=tof->buildMatrixCategory("HTofRawSim",0.5F);
    if (!fRawCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catTofRaw,fRawCat,"Tof");
  } else {
    if (fRawCat->getClass()!=HTofRawSim::Class()) {
      Error("HTofHitFSim::init()","Misconfigured input category");
      return kFALSE;
    }
  }
  iter = (HIterator*)fRawCat->MakeIterator();  // cannot be "native", because
                                               // iter->getLocation() is used

  fHitCat=gHades->getCurrentEvent()->getCategory(catTofHit);
  if (!fHitCat) {
    HTofDetector* tof=(HTofDetector*)(gHades->getSetup()->getDetector("Tof"));
    fHitCat=tof->buildMatrixCategory("HTofHitSim",0.5F);
    if (!fHitCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catTofHit,fHitCat,"Tof");
  } else {
    if (fHitCat->getClass()!=HTofHitSim::Class()) {
      Error("HTofHitfSim::init()","Misconfigured output category");
      return kFALSE;
    }
  }

  fActive = kTRUE;
  return kTRUE;
  }


HTofHit *HTofHitFSim::makeHit(TObject *address) {
  return new(address) HTofHitSim;
}

void HTofHitFSim::fillHit(HTofHit *hit, HTofRaw *raw) {
  HTofHitSim *hs=(HTofHitSim *)hit;
  HTofRawSim *rs=(HTofRawSim *)raw;
  hs->setNTrack1(rs->getNTrack1());
  hs->setNTrack2(rs->getNTrack2());
}

