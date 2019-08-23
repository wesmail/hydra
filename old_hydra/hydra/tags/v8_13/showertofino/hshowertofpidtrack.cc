//*-- AUTHOR : Jacek Otwinowski 
//*-- Modified : 19/04/05 by Jacek Otwinowski
//
//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
//
//  HShowerTofPIDTrack
//  
//  Data object for Shower/Tofino lepton candidates and 
//  corresponding geant track numbers. This data object is filled
//  by HShowerTofPIDTrackMatcher which takes information from HShowerTofPID and 
//  HShowerTrack data containers.
//  
/////////////////////////////////////////////////////////////////////////////////
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

