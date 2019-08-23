///////////////////////////////////////////////////////////////////////////////
//                                                                           
// HShowerTofPIDTrack
// class joins data in Tofino/Shower PID level with track number
//                              
///////////////////////////////////////////////////////////////////////////////
#pragma implementation

#include "hshowertofpidtrack.h"

ClassImp(HShowerTofPIDTrack)

void HShowerTofPIDTrack::clear() {
  // clears the object
  HShowerTofPID::clear();
  nTrack = 0;
}

HShowerTofPIDTrack& HShowerTofPIDTrack::operator=(HShowerTofPIDTrack& ht) {
  HShowerTofPID::operator=(ht);
  nTrack = ht.nTrack;
  
  return *this;
}

HShowerTofPIDTrack& HShowerTofPIDTrack::operator=(HShowerTofPID& ht) {
  HShowerTofPID::operator=(ht);
  nTrack = 0;
  
  return *this;
}

