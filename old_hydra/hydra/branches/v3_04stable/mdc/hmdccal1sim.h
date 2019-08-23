#ifndef HMDCCAL1SIM_H
#define HMDCCAL1SIM_H

#include "hmdccal1.h"

class HMdcCal1Sim : public HMdcCal1 {
protected:
  Int_t nTrack1;   // number of Track for first hit
  Int_t nTrack2;   // number of Track for second hit
  Int_t status;    // status of cell (e.g. 1 if ok)
public:
  HMdcCal1Sim(void) : nTrack1(-99), nTrack2(-99), status(0) {}
  ~HMdcCal1Sim(void) {}
  void clear(void);
  void setNTrack1(const Int_t n) {nTrack1=n;}
  void setNTrack2(const Int_t n) {nTrack2=n;}
  void setStatus(const Int_t f) { status=f; }
  Int_t getNTrack1(void) const {return nTrack1;}
  Int_t getNTrack2(void) const {return nTrack2;}
  Int_t getStatus(void) const { return status; }

  ClassDef(HMdcCal1Sim,1) // simulated cal1 hit on a MDC
};

#endif  /* HMDCCAL1SIM_H */

