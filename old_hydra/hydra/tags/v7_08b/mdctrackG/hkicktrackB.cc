//*-- Author : A.Sadovski (using Anar's Rustamov code as an example)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// HKickTrackB 
//
//   the output of low resolution KickTrack tracking (by M.Sanchez)
//   which was adopted to a new HBaseTrack scheme.
//
// Used inline functions are:
//   void setSegment(  Int_t   i)   - Sets segment Id
//   void setQuality(  Char_t  q)   - Sets quality
//   void setPTof(     Float_t p)   - Sets momentum calculated using Time-of-Flight
//   void setErrPTof(  Float_t e)   -      and it's error
//   void setPID(      Char_t  p)   - Sets particle identification based on KickPlane internal algorithm
//   //void setMetaEloss(Float_t t) - Sets energy loss of the thrack deposited in META detector (used in HBaseTrack)
//   void setPull(     Float_t p)   - Sets pull variable wich is a normalized quality parameter of kickplane, see Ph.d-thesis of M.Sanchez
//   void setFlag(     Float_t f)   - Undefined
//   void setOuterHitId( Int_t   i) - Sets Id of the hit in META
//   void setShowerSum10(Float_t f) - Sets for Shower: sum of 9 charges in post  / sum of 9 charges in pre
//   void setShowerSum20(Float_t f) - Sets for Shower: sum of 9 charges in post2 / sum of 9 charges in pre
//   
//   Int_t   getSegment(  void)     - Segment Id
//   Char_t  getQuality(  void)     - Quality
//   Float_t getPTof(	void)       - momentum from TOF estimation
//   Float_t getErrPTof(  void)     -          and it's error
//   Char_t  getPID(      void)     - particle identification
//   //Float_t getMetaEloss(void)   - energy loss is also provided but now is a member of HBaseTrack
//   Float_t getPull(	void)       - pull variable, see below...
//   Int_t   getOuterHitId( void)   - Id of hit in META detector
//   Float_t getShowerSum10(void)   - Shower: sum of 9 charges in post  / sum of 9 charges in pre
//   Float_t getShowerSum20(void)   - Shower: sum of 9 charges in post2 / sum of 9 charges in pre
//   Float_t getFlag(       void)   - Undefined
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// See also: HKickTrack, scince it was adopted one to one to the "modern" HBaseTrack scheme
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//  This is the output of the HKickTrackFB class which matches a segment
//  in MDC (class HMdcSeg) with a hit in Meta (HTofHit or HShowerHitTof)
//  and assigns the corresponding momentum.
//
//  Documentation for the non-trivial data members follows:
//  
//  pTof .- 
//    For each track the momentum is determined using to approaches:
//       a) The track's deflection in the field. The resultin momentun is p
//       b) With p and the track's time of flight the mass is calculated.
//          On the basis of the mass and charge the particle is identified,
//          once the particle is 'known' it's mass is forced to be on the
//          correct value and the momentum is recalculated under this 
//          condition. The resulting momentum is stored in pTof.
//
//  pid.-
//    Particle's PID. See HKickTrackF / HKickMassFit for more information.
//
//  quality.-
//    When for a segment in MDC there is only one hit in Meta within the cuts
//    a track is formed with quality=1. When several hits in Meta are within
//    the cuts several tracks are built with quality=0, thus deferring the
//    resolution of such events for a later stage in the analysis.
//
//  r,z,theta,phi.-
//    Same meaning as in HMdcSeg
//
//  mass.-
//    Despite of the name. This variable actually contains the squared mass
//    determined from p and beta. 
//
//  system.-
//    0  - the track was formed with an MDC segment and a Shower hit
//    >0 - the track was formed with an MDC segment and a TOF hit.
//
//  showerSum10, showerSum20
//    For the case system==0. showerSum10=showerhit.sum(1)/showerhit.sum(0)
//    showerSum20 = showerhit.sum(2)/showerhit.sum(0). See HShowerHit for
//    information on what sum(2),sum(1) and sum(0) are.
//
//  segmentId.-
//    Id of the MDC segment used to build the track. The segment's HLocation
//    is derived from segmentId as follows.
//       
//       segLoc.set(3,sector,0,segmentId)
//  
//  outerHitId.-
//    Id of the segment in Meta used to build the track. There are two
//    possibilities:
//
//    system==0   <=> Shower was used
//     
//      HShowerHit *hit = (HShowerHit *)catShowerHits->getObject(outerHitId)
//
//    system>0    <=> TOF was used
//
//      outerHitId = tof.sector*1000 + tof.module*100 + tof.cell
//
//  pull.-
//    (x_meta - x_mdc) / sigma(x_meta - x_mdc). Where
//    Where:
//      x_meta -> x coordinate of the hit in Meta used to build the track
//      x_mdc -> x coordinate of the MDC segment in the Meta detector.
//               The MDC segment used to build the track is propagated 
//               trough the magnetic field and it's intersection with the
//               Meta detector is computed. x_mdc is the x coordinate of
//               such intersection point.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



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
