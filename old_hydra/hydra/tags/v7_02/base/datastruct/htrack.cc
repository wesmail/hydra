#include "htrack.h"
//*-- Author : Manuel Sanchez Garcia
//*-- Modified : 30/03/98
//*-- Copyright : FPGENP

////////////////////////////////////
//HTrack
// 
////////////////////////////////

ClassImp(HTrack)

HTrack::HTrack(void) {
  fP=0.0;
}

HTrack::HTrack(HTrack &aTrack) {
  fP=aTrack.fP;
}

HTrack::~HTrack(void) {  
}

void HTrack::setMomentum(Float_t aP) {
  fP=aP;
}

