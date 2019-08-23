#ifndef HTOFRAWSIM_H
#define HTOFRAWSIM_H

#include "htofraw.h"

class HTofRawSim : public HTofRaw {
protected:
  Int_t nTrack1;   // number of Track for first hit in cell
  Int_t nTrack2;   // number of Track for second hit in cell

public:
  HTofRawSim(void) : nTrack1(0), nTrack2(0) {}
  ~HTofRawSim(void) {}
  void clear(void);
  inline void setNTrack1(const Int_t n) {nTrack1=n;}
  inline void setNTrack2(const Int_t n) {nTrack2=n;}
  void setNTrack(Int_t );
  inline Int_t getNTrack1(void) const {return nTrack1;}
  inline Int_t getNTrack2(void) const {return nTrack2;}


  ClassDef(HTofRawSim,1) // simulated raw data level of Tof
};

#endif  /* HTOFRAWSIM_H */

