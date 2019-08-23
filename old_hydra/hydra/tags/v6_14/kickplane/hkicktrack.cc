#include "hkicktrack.h"
//*-- Author : Manuel Sanchez Garcia
//*-- Modified : 30/03/98
//*-- Modified : 2.10.2000
//*-- Copyright : FPGENP

////////////////////////////////////
//HKickTrack
// 
//  This is the output of the HKickTrackF class which matches a segment
//in MDC (class HMdcSeg) with a hit in Meta (HTofHit or HShowerHitTof)
//and assigns the corresponding momentum.
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
////////////////////////////////

const Char_t HKickTrack::protonId=14;
const Char_t HKickTrack::antiProtonId=15;
const Char_t HKickTrack::kPlusId=11;
const Char_t HKickTrack::kMinusId=12;
const Char_t HKickTrack::piPlusId=8;
const Char_t HKickTrack::piMinusId=9;
const Char_t HKickTrack::unknownId=0;

HKickTrack::HKickTrack(void) {

}

HKickTrack::HKickTrack(HKickTrack &aTrack) {
  z=aTrack.z ;
  r=aTrack.r ;
  p=aTrack.p ;
  theta=aTrack.theta ;
  phi=aTrack.phi ;
  mass=aTrack.mass ;
  charge=aTrack.charge ;   
  cov=aTrack.cov ; 
  sector=aTrack.sector ;
  ringId = -1;
  outerHitId = -1;
  showerSum10 = -1.;
  showerSum20 = -1.;
}

HKickTrack::~HKickTrack(void) {  
}

void HKickTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class HKickTrack.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> pTof;
      R__b >> errPTof;
      R__b >> pid;
      R__b >> quality;
      R__b >> z;
      R__b >> r;
      R__b >> p;
      R__b >> theta;
      R__b >> phi;
      R__b >> mass;
      R__b >> charge;
      cov.Streamer(R__b);
      R__b >> sector;
      R__b >> system;
      R__b >> tof;
      R__b >> beta;
      R__b >> showerSum10;
      R__b >> showerSum20;
      R__b >> segmentId;
      R__b >> ringId;
      R__b >> outerHitId;
      R__b >> pull;
      if(R__v <= 1){
        metaeloss = 0.;
      } else {
        R__b >> metaeloss;
      }
      R__b.CheckByteCount(R__s, R__c, HKickTrack::IsA());


   } else {
      R__c = R__b.WriteVersion(HKickTrack::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << pTof;
      R__b << errPTof;
      R__b << pid;
      R__b << quality;
      R__b << z;
      R__b << r;
      R__b << p;
      R__b << theta;
      R__b << phi;
      R__b << mass;
      R__b << charge;
      cov.Streamer(R__b);
      R__b << sector;
      R__b << system;
      R__b << tof;
      R__b << metaeloss;
      R__b << beta;
      R__b << showerSum10;
      R__b << showerSum20;
      R__b << segmentId;
      R__b << ringId;
      R__b << outerHitId;
      R__b << pull;
      R__b.SetByteCount(R__c, kTRUE);
   }
}


ClassImp(HKickTrack)

