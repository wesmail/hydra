#ifndef HShowerTofPIDTrack_H
#define HShowerTofPIDTrack_H

#pragma interface

#include "hshowertofpid.h"

class HShowerTofPIDTrack : public HShowerTofPID{
protected:
  Int_t nTrack;   // Track number

public:
  HShowerTofPIDTrack(void) : nTrack(0){}
  ~HShowerTofPIDTrack(void) {}
  void clear(void);
  inline void setTrack(const Int_t track) {nTrack=track;} //set track number
  inline Int_t getTrack(void) const {return nTrack;} //get track number

  Bool_t  IsSortable() const { return kTRUE; }
  inline Int_t   Compare(TObject *obj);

  HShowerTofPIDTrack& operator=(HShowerTofPIDTrack& ht);
  HShowerTofPIDTrack& operator=(HShowerTofPID& ht);

  ClassDef(HShowerTofPIDTrack,1) // Tofino/Shower HIT data tied with track number
};

inline Int_t   HShowerTofPIDTrack::Compare(TObject *obj) {
   if (nTrack==((HShowerTofPIDTrack*)obj)->getTrack()) return 0;
   return (nTrack > ((HShowerTofPIDTrack*)obj)->getTrack()) ? 1 : -1;
}

#endif /* !HShowerTofPIDTrack_H */

