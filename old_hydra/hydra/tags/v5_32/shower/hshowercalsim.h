#ifndef HSHOWERCALSIM_H
#define HSHOWERCALSIM_H

#include "hshowercal.h"

class HShowerCalSim : public HShowerCal {
protected:
  Int_t nTrack1;   // number of Track for first hit
  Int_t nTrack2;   // number of Track for second hit

public:
  HShowerCalSim(void) : nTrack1(0), nTrack2(0) {}
  ~HShowerCalSim(void) {}
  void clear(void);
  inline void setNTrack1(const Int_t n) {nTrack1=n;} //set trk nb of hit 1
  inline void setNTrack2(const Int_t n) {nTrack2=n;} //set trk nb of hit 2
  inline Int_t getNTrack1(void) const {return nTrack1;} //get trk nb of hit 1
  inline Int_t getNTrack2(void) const {return nTrack2;} //get trk nb of hit 2 
  ClassDef(HShowerCalSim,1) // SHOWER simulated CAL data
};

#endif /* !HSHOWERCALSIM_H */ 
