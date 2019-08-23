#ifndef HSHOWERHITTRACK_H
#define HSHOWERHITTRACK_H

#pragma interface

#include "hshowerhit.h"
#include "showerdef.h"

class HShowerHitTrack : public HShowerHit{
protected:
  Int_t nTrack;                    // number of Tracks
  Int_t nTracks[SH0WER_MAX_TRACK]; // array of tracks (default val == -1)
public:
    HShowerHitTrack(void) {
	for(Int_t i = 0; i < SH0WER_MAX_TRACK; i ++) { nTracks[i] = -1; }
	nTrack = 0;
    }
  ~HShowerHitTrack(void) {}
  void clear(void);
  inline void  setTrack(const Int_t track) { if(nTrack < SH0WER_MAX_TRACK) {nTracks[nTrack] = track; nTrack ++; } } //set track number
  inline Int_t getTrack(Int_t n = 0) const { if(n < SH0WER_MAX_TRACK && n >= 0) { return nTracks[n];} else{ return 0;}} //get track number
  Int_t   getNTracks()            {return nTrack;}
  void    setNTracks(Int_t n = 0) {nTrack = n;}
  HShowerHitTrack& operator=(HShowerHitTrack& ht);
  HShowerHitTrack& operator=(HShowerHit& ht);

  ClassDef(HShowerHitTrack,2) // SHOWER HIT data tied with track number
};
#endif /* !HSHOWERHITTRACK_H */

