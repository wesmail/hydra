//*-- AUTHOR : Leszek Kidon
//*-- Modified : 19/04/05 by Jacek Otwinowski
//
///////////////////////////////////////////////////////////////////////////////
//                                                                           
// HShowerHitTofTrack
// 
// Data object for correlated Shower/Tofino hits and
// corresponding geant track numbers.  
// This data object is filled by HShowerHitTofTrackMatcher 
// which takes information from HShowerHitTof 
// and HShowerTrack data containers.
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

