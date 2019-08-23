#ifndef HRTTRACKINGPAR_H
#define HRTTRACKINGPAR_H

#include "hparset.h"

class HRtTrackingPar : public HParSet {
public:
  HRtTrackingPar(void);
  ~HRtTrackingPar(void);
  void setParams(Float_t c,Float_t d,Float_t p);
  Float_t getMaxChi2(void) { return fMaxChi2; }
  Float_t getMaxDistance(void) { return fMaxDistance; }
  Float_t getMaxDeltaPhi(void) { return fMaxDeltaPhi; }
protected:
  Float_t fMaxChi2;
  Float_t fMaxDistance;
  Float_t fMaxDeltaPhi;
public:
  ClassDef(HRtTrackingPar,1)
};

inline  void HRtTrackingPar::setParams(Float_t c,Float_t d,Float_t p) {
  fMaxChi2 = c;
  fMaxDistance = d;
  fMaxDeltaPhi = p;
}

#endif
