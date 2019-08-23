//*-- AUTHOR : Leszek Kidon
//*-- Modified : 19/04/05 by Jacek Otwinowski
//
//_HADES_CLASS_DESCRIPTION 
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
  for(Int_t i = 0; i < SH0WER_MAX_TRACK; i ++){ nTracks[i] = -1;}
}

HShowerHitTofTrack& HShowerHitTofTrack::operator=(HShowerHitTofTrack& ht) {
  HShowerHitTof::operator=(ht);
  nTrack = ht.nTrack;
  for(Int_t i = 0; i < SH0WER_MAX_TRACK; i ++){ nTracks[i] = ht.nTracks[i]; }
  return *this;
}

HShowerHitTofTrack& HShowerHitTofTrack::operator=(HShowerHitTof& ht) {
  HShowerHitTof::operator=(ht);
  nTrack = 0;
  for(Int_t i = 0; i < SH0WER_MAX_TRACK; i ++){ nTracks[i] = -1;}

  return *this;
}

void HShowerHitTofTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class HShowerHitTofTrack.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      HShowerHitTof::Streamer(R__b);
      R__b >> nTrack;
      if(R__v > 1){
	  R__b.ReadStaticArray((int*)nTracks);
      } else {
	  for(Int_t i = 0; i < SH0WER_MAX_TRACK; i ++){ nTracks[i] = -1;}
      }
      R__b.CheckByteCount(R__s, R__c, HShowerHitTofTrack::IsA());
   } else {
      R__c = R__b.WriteVersion(HShowerHitTofTrack::IsA(), kTRUE);
      HShowerHitTof::Streamer(R__b);
      R__b << nTrack;
      R__b.WriteArray(nTracks, SH0WER_MAX_TRACK);
      R__b.SetByteCount(R__c, kTRUE);
   }
}



