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
  for(Int_t i = 0; i < SH0WER_MAX_TRACK; i ++){ nTracks[i] = -1; }
}

HShowerHitTrack& HShowerHitTrack::operator=(HShowerHitTrack& ht) {
  HShowerHit::operator=(ht);
  nTrack = ht.nTrack;
  for(Int_t i = 0; i < SH0WER_MAX_TRACK; i ++){ nTracks[i] = ht.nTracks[i]; }
  return *this;
}

HShowerHitTrack& HShowerHitTrack::operator=(HShowerHit& ht) {
  HShowerHit::operator=(ht);
  nTrack = 0;
  for(Int_t i = 0; i < SH0WER_MAX_TRACK; i ++){ nTracks[i] = -1;}
  return *this;
}

void HShowerHitTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class HShowerHitTrack.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      HShowerHit::Streamer(R__b);
      R__b >> nTrack;
      if(R__v > 1){
	  R__b.ReadStaticArray((int*)nTracks);
      } else {
	  for(Int_t i = 0; i < SH0WER_MAX_TRACK; i ++){ nTracks[i] = -1;}
      }
      R__b.CheckByteCount(R__s, R__c, HShowerHitTrack::IsA());
   } else {
      R__c = R__b.WriteVersion(HShowerHitTrack::IsA(), kTRUE);
      HShowerHit::Streamer(R__b);
      R__b << nTrack;
      R__b.WriteArray(nTracks, SH0WER_MAX_TRACK);
      R__b.SetByteCount(R__c, kTRUE);
   }
}
