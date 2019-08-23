#ifndef HMDCSIMHIT_H
#define HMDCSIMHIT_H

#include "hmdchit.h"

class HMdcSimHit : public HMdcHit {
protected:
  Float_t fMomentum;
  Int_t fTrack; // track Nb
public:
  HMdcSimHit(void) {}
  ~HMdcSimHit(void) {}
  Float_t getMomentum(void) {return fMomentum;}
  Int_t getTrack(void) {return fTrack;}
  void setMomentum(Float_t momentum) { fMomentum=momentum; }
  void setTrack(Int_t track) { fTrack=track; }
  ClassDef(HMdcSimHit,0)
};

#endif
