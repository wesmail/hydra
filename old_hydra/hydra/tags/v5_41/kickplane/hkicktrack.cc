#include "hkicktrack.h"
//*-- Author : Manuel Sanchez Garcia
//*-- Modified : 30/03/98
//*-- Modified : 2.10.2000
//*-- Copyright : FPGENP

////////////////////////////////////
//HTrack
// 
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

ClassImp(HKickTrack)

