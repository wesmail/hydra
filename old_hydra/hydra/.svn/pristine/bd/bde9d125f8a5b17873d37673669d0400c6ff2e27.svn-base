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


void HKickTrackB::Streamer(TBuffer &R__b)
{
   // Stream an object of class HKickTrackB.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c);
      HBaseTrack::Streamer(R__b);
      R__b >> quality;
      R__b >> pTof;
      R__b >> errPTof;
      R__b >> pid;
      R__b >> pull;
      R__b >> outherHitId;
      R__b >> showerSum10;
      R__b >> showerSum20;
      R__b >> flag;
      if (R__v>1) R__b >> segmentId;
      else segmentId = -1;
      R__b.CheckByteCount(R__s, R__c, HKickTrackB::IsA());
   } else {
      R__c = R__b.WriteVersion(HKickTrackB::IsA(), kTRUE);
      HBaseTrack::Streamer(R__b);
      R__b << quality;
      R__b << pTof;
      R__b << errPTof;
      R__b << pid;
      R__b << pull;
      R__b << outherHitId;
      R__b << showerSum10;
      R__b << showerSum20;
      R__b << flag;
      R__b << segmentId;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

ClassImp(HKickTrackB)
