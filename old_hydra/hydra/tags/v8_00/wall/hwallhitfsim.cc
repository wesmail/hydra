//*-- AUTHOR : Jaroslav Bielcik
//*-- Modified : 23/05/2005 by F. Krizek
//*-- Modified : 13/11/2001 by D. Zovinec
//*-- Modified : 25/05/2000 by R. Holzmann
//*-- Modified : 17/12/99 by Jaroslav Bielcik


//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// HWallHitFSim                                                              //
// This class is derived from HWallHitF and passes track numbers of the      //
// HGeant hit from raw to hit level                                          //
///////////////////////////////////////////////////////////////////////////////

#include "hwallhitfsim.h"
#include "hdebug.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hwalldetector.h"
#include "hevent.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hlocation.h"
#include "hwallrawsim.h"

#include "hwallhitsim.h"

ClassImp(HWallHitFSim)

 HWallHitFSim::HWallHitFSim(void){
   fWallSimulation=kTRUE;
 }

 HWallHitFSim::HWallHitFSim(Text_t* name,Text_t* title) : HWallHitF(name,title){
   fWallSimulation=kTRUE;
 }

 HWallHitFSim::~HWallHitFSim(void) {
 }

 Bool_t HWallHitFSim::init(void) {
 // initialization of fRawCat and fHitCat containers
 //initParContainer(gHades->getSetup(),gHades->getRuntimeDb());
   initParContainer();//???????????????????????????????????????????
 fRawCat =gHades->getCurrentEvent()->getCategory(catWallRaw);
  if (!fRawCat) {
    HWallDetector* wall=(HWallDetector*)(gHades->getSetup()->getDetector("Wall"));
    fRawCat=wall->buildMatrixCategory("HWallRawSim",0.5F);
    if (!fRawCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catWallRaw,fRawCat,"Wall");
  } else {
    if (fRawCat->getClass()!=HWallRawSim::Class()) {
      Error("HWallHitFSim::init()","Misconfigured input category");
      return kFALSE;
    }
  }
  iter = (HIterator*)fRawCat->MakeIterator();  // cannot be "native", because
                                               // iter->getLocation() is used

  fHitCat=gHades->getCurrentEvent()->getCategory(catWallHit);
  if (!fHitCat) {
    HWallDetector* wall=(HWallDetector*)(gHades->getSetup()->getDetector("Wall"));
    fHitCat=wall->buildMatrixCategory("HWallHitSim",0.5F);
    if (!fHitCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catWallHit,fHitCat,"Wall");
  } else {
    if (fHitCat->getClass()!=HWallHitSim::Class()) {
      Error("HWallHitfSim::init()","Misconfigured output category");
      return kFALSE;
    }
  }

  fActive = kTRUE;
  return kTRUE;
  }


HWallHit *HWallHitFSim::makeHit(TObject *address) {
  return new(address) HWallHitSim;
}

void HWallHitFSim::fillHit(HWallHit *hit, HWallRaw *raw) {
  HWallHitSim *hs=(HWallHitSim *)hit;
  HWallRawSim *rs=(HWallRawSim *)raw;
  hs->setNTrack1(rs->getNTrack1());
  hs->setNTrack2(rs->getNTrack2());
}

