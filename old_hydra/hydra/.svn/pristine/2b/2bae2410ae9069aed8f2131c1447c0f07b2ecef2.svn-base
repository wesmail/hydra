//*-- Author : Stefano Spataro (spataro@lns.infn.it)

#include "hpidtofinocorr.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "TMath.h"
#include "hpidtrackcandsim.h"
#include "hpidgeanttrackset.h"
#include "hpidtrackdata.h"
#include "hpidhitdata.h"
#include "hpidphysicsconstants.h"
#include <iostream>

  ClassImp(HPidTofinoCorr)
  
HPidTofinoCorr::HPidTofinoCorr(void)
{
  // Default constructor

}

HPidTofinoCorr::~HPidTofinoCorr(void){
  // Destructor.
}

Bool_t HPidTofinoCorr::init(void){
  cout << "PID Tof Reconstructor default initialization *** " << endl;
  
  pTrackCand=gHades->getCurrentEvent()->getCategory(catPidTrackCand);
  if (!pTrackCand) {
    Error("init","HPidTofinoCorr: No HPIDTrackCand Input -> Switching HPidTofinoCorr OFF");
  }
  else iterTrackCand= (HIterator *)pTrackCand->MakeIterator();
  
  return kTRUE;
}

Int_t HPidTofinoCorr::execute(void){
  // Execute function
  Float_t tofino_corr[24] = {0.71, -0.02, 0.44, 0.90,
                             -0.36, -0.13, 0.74, 1.44,
                             1.53, 1.19, 2.25, 2.3,
                             3.8, 2.88, 0.59, 1.03,
                             2.04, 1.65, 1.51, 2.22,
                             1.77, 1.44, 3.07, 4.84};
  Float_t sec_corr[6] = {-3.96824e-03, -2.64326e-03, 1.0790774e-01, 1.492324e-02, -1.237478e-01, 0.};
  HPidTrackCandSim *pCand  = 0;
  
  iterTrackCand->Reset();
  while ((pCand = (HPidTrackCandSim *)iterTrackCand->Next()) != 0) // begin of PidTrackCand iterator
    {
      if (pCand->getHitData()->iSystem==0){
	pCand->getHitData()->fShowerTimeOfFlight = pCand->getHitData()->fShowerTimeOfFlight - tofino_corr[pCand->getHitData()->nMetaCell+4*pCand->getHitData()->nSector] - sec_corr[pCand->getHitData()->nSector] - 2.06532e-01;
      }
      if (pCand->getHitData()->iSystem==1){
	pCand->getHitData()->fTOFTimeOfFlight = pCand->getHitData()->fTOFTimeOfFlight - sec_corr[pCand->getHitData()->nSector];
      }
    } 
  
  return 0;
}

Bool_t HPidTofinoCorr::finalize(void){
  // Execute function  
  return kTRUE;
}
