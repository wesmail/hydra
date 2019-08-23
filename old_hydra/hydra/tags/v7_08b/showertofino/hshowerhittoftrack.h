#ifndef HSHOWERHITTOFTRACK_H
#define HSHOWERHITTOFTRACK_H

#pragma interface

#include "hshowerhittof.h"

class HShowerHitTofTrack : public HShowerHitTof{
protected:
  Int_t nTrack;   // Track number

public:
  HShowerHitTofTrack(void) : nTrack(0){}
  ~HShowerHitTofTrack(void) {}
  void clear(void);
  inline void setTrack(const Int_t track) {nTrack=track;} //set track number
  inline Int_t getTrack(void) const {return nTrack;} //get track number

  Bool_t  IsSortable() const { return kTRUE; }
  inline Int_t   Compare(TObject *obj);

  HShowerHitTofTrack& operator=(HShowerHitTofTrack& ht);
  HShowerHitTofTrack& operator=(HShowerHitTof& ht);

  ClassDef(HShowerHitTofTrack,1) // Tofino/Shower HIT data tied with track number
};

inline Int_t   HShowerHitTofTrack::Compare(TObject *obj) {
   if (nTrack==((HShowerHitTofTrack*)obj)->getTrack()) return 0;
   return (nTrack > ((HShowerHitTofTrack*)obj)->getTrack()) ? 1 : -1;
}

#endif /* !HSHOWERHITTOFTRACK_H */

