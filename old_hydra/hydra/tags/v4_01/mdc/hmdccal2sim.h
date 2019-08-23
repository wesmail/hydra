#ifndef HMDCCAL2SIM_H
#define HMDCCAL2SIM_H

#include "hmdccal2.h"

class HMdcCal2Sim : public HMdcCal2 {
protected:
  Int_t nTrack1;   // number of Track for first hit
  Int_t nTrack2;   // number of Track for second hit
  Int_t status;    // status of the cell (e.g. 1 if ok)
public:
  HMdcCal2Sim(void) : nTrack1(-99), nTrack2(-99), status(0) {}
  ~HMdcCal2Sim(void) {}
  void clear(void);
  void setNTrack1(const Int_t n) {nTrack1=n;}
  void setNTrack2(const Int_t n) {nTrack2=n;}
  void setStatus(const Int_t f) { status=f; }
  Int_t getNTrack1(void) const {return nTrack1;}
  Int_t getNTrack2(void) const {return nTrack2;}
  Int_t getStatus() const {return status;}

  ClassDef(HMdcCal2Sim,1) // simulated cal2 hit on a MDC
};

#endif  /* HMDCCAL2SIM_H */

