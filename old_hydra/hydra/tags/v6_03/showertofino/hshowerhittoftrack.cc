///////////////////////////////////////////////////////////////////////////////
//                                                                           
// HShowerHitTofTrack
// class joins data in Tofino/Shower HIT level with track number
//                              
///////////////////////////////////////////////////////////////////////////////
#pragma implementation

#include "hshowerhittoftrack.h"

ClassImp(HShowerHitTofTrack)

void HShowerHitTofTrack::clear() {
  // clears the object
  HShowerHitTof::clear();
  nTrack = 0;
}

HShowerHitTofTrack& HShowerHitTofTrack::operator=(HShowerHitTofTrack& ht) {
  HShowerHitTof::operator=(ht);
  nTrack = ht.nTrack;
  
  return *this;
}

HShowerHitTofTrack& HShowerHitTofTrack::operator=(HShowerHitTof& ht) {
  HShowerHitTof::operator=(ht);
  nTrack = 0;
  
  return *this;
}

