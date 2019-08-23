//*-- AUTHOR : Dusan Zovinec (23.10.2002)

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// HTofClusterFSim                                                           //
// This class is derived from HTofClusterF and passes track numbers of the   //
// HGeant cluster from hit to cluster level                                  //
///////////////////////////////////////////////////////////////////////////////

#include "htofclusterfsim.h"
#include "hdebug.h"
#include "hades.h"
#include "hspectrometer.h"
#include "htofdetector.h"
#include "hevent.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hlocation.h"
#include "htofhitsim.h"
#include "htofclustersim.h"

ClassImp(HTofClusterFSim)

HTofClusterFSim::~HTofClusterFSim(void) {
}

Bool_t HTofClusterFSim::init(void) {
 // initialization of fHitCat and fClusterCat containers
 initParContainer(gHades->getSetup(),gHades->getRuntimeDb());
 fHitCat =gHades->getCurrentEvent()->getCategory(catTofHit);
  if (!fHitCat) {
    HTofDetector* tof=(HTofDetector*)(gHades->getSetup()->getDetector("Tof"));
    fHitCat=tof->buildMatrixCategory("HTofHitSim",0.5F);
    if (!fHitCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catTofHit,fHitCat,"Tof");
  } else {
    if (fHitCat->getClass()!=HTofHitSim::Class()) {
      Error("HTofClusterFSim::init()","Misconfigured input category");
      return kFALSE;
    }
  }
  iterh = (HIterator*)fHitCat->MakeIterator();

  fClusterCat=gHades->getCurrentEvent()->getCategory(catTofCluster);
  if (!fClusterCat) {
    HTofDetector* tof=(HTofDetector*)(gHades->getSetup()->getDetector("Tof"));
    fClusterCat=tof->buildMatrixCategory("HTofClusterSim",0.5F);
    if (!fClusterCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catTofCluster,fClusterCat,"Tof");
  } else {
    if (fClusterCat->getClass()!=HTofClusterSim::Class()) {
      Error("HTofClusterFSim::init()","Misconfigured output category");
      return kFALSE;
    }
  }
  iterc = (HIterator*)fClusterCat->MakeIterator();

  fActive = kTRUE;
  return kTRUE;
  }


Int_t  HTofClusterFSim::execute() {
// This method executes HTofClusterF::execute() and moreover
// it copies the track numbers from the hit to the cluster level containers

  HTofClusterF::execute();  // do work for real data part

  HTofHit *hit=NULL;
  HTofCluster *cluster=NULL;

  iterh->Reset();
  iterc->Reset();
  while((cluster = (HTofCluster*)iterc->Next()) != NULL) {
    fCLoc[0]=cluster->getSector();
    fCLoc[1]=cluster->getModule();
    fCLoc[2]=cluster->getCell();
    ((HTofClusterSim*)cluster)->clear();
    ((HTofClusterSim*)cluster)->setNParticipants(1);
    for(Int_t i=0; i<(cluster->getClusterSize()); i++){
      hit = (HTofHit*)iterh->Next();
      fLoc[0]=hit->getSector();
      fLoc[1]=hit->getModule();
      fLoc[2]=hit->getCell();
      ((HTofClusterSim*)cluster)->setNTrack1( ((HTofHitSim*)hit)->getNTrack1() );
      ((HTofClusterSim*)cluster)->setNTrack2( ((HTofHitSim*)hit)->getNTrack2() );
      ((HTofClusterSim*)cluster)->incNParticipants();
    }
  }
  return 0;
}

