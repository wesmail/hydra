//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
//                                                                           
// HShowerHitTrack
// class joins data in hit level with track number
//                              
///////////////////////////////////////////////////////////////////////////////
#pragma implementation

#include "hshowerhittrack.h"

ClassImp(HShowerHitTrack)

void HShowerHitTrack::clear() {
  // clears the object
  HShowerHit::clear();
  nTrack = 0;
}

HShowerHitTrack& HShowerHitTrack::operator=(HShowerHitTrack& ht) {
  HShowerHit::operator=(ht);
  nTrack = ht.nTrack;
  
  return *this;
}

HShowerHitTrack& HShowerHitTrack::operator=(HShowerHit& ht) {
  HShowerHit::operator=(ht);
  nTrack = 0;
  
  return *this;
}

