//*-- Author : A.Sadovski (using Anar's Rustamov code as an example)
//HKickTrackB - the output of low resolution kicktrack in a new BaseTrack scheme
//in fuure one has to put here also middle resolution and high resolution kickplane
#include "hkicktrackB.h"
#include "hkicktrack.h"
using namespace std;
#include <iostream>

HKickTrackB::HKickTrackB(void)
{
  showerSum10 = -1.;
  showerSum20 = -1.;
}

HKickTrackB::HKickTrackB(HKickTrack* kickTrk) {
  segmentId=kickTrk->getSegment();
  z=kickTrk->getZ();
  r=kickTrk->getR();
  theta=kickTrk->getTheta();
  phi=kickTrk->getPhi();
  if(kickTrk->getSystem()==1) {
    tofHitInd=kickTrk->getOuterHitId();
    showerHitInd=-1;
  } else if(kickTrk->getSystem()==0){
    showerHitInd=kickTrk->getOuterHitId();
    tofHitInd=-1;
  } else tofHitInd=showerHitInd=-1;
  p=kickTrk->getP();
  beta=kickTrk->getBeta();
  mass2=kickTrk->getMass();
  tof=kickTrk->getTof();
  polarity=kickTrk->getCharge();
  sector=kickTrk->getSector();
  cov=kickTrk->getCov(); // ???
  metaEloss=kickTrk->getMetaEloss();

  quality=kickTrk->getQuality();
  pTof=kickTrk->getPTof();
  errPTof=kickTrk->getErrPTof();
  pid=kickTrk->getPID();
  pull=kickTrk->getPull();
  outherHitId=kickTrk->getOuterHitId();

  Float_t f;
  showerSum10=kickTrk->getShowerSum10(f);
  showerSum20=kickTrk->getShowerSum20(f);
  flag=kickTrk->getFlag();
}

HKickTrackB::~HKickTrackB(void)
{

}

//HKickTrackB::HKickTrackB(HKickTrackB &aTrack){
//  cout << "copy" <<endl;
//}

ClassImp(HKickTrackB)
