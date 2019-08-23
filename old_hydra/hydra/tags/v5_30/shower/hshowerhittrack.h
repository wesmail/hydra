#ifndef HSHOWERHITTRACK_H
#define HSHOWERHITTRACK_H

#pragma interface

#include "hshowerhit.h"

class HShowerHitTrack : public HShowerHit{
protected:
  Int_t nTrack;   // Track number

public:
  HShowerHitTrack(void) : nTrack(0){}
  ~HShowerHitTrack(void) {}
  void clear(void);
  inline void setTrack(const Int_t track) {nTrack=track;} //set track number
  inline Int_t getTrack(void) const {return nTrack;} //get track number

  Bool_t  IsSortable() const { return kTRUE; }
  Int_t   Compare(TObject *obj);

  HShowerHitTrack& operator=(HShowerHitTrack& ht);
  HShowerHitTrack& operator=(HShowerHit& ht);

  ClassDef(HShowerHitTrack,1) // SHOWER HIT data tied with track number
};

inline Int_t   HShowerHitTrack::Compare(TObject *obj) {
   if (nTrack==((HShowerHitTrack*)obj)->getTrack()) return 0;
   return (nTrack > ((HShowerHitTrack*)obj)->getTrack()) ? 1 : -1;
}

#endif /* !HSHOWERHITTRACK_H */

