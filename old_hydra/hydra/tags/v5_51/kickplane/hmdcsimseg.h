#ifndef HMDCSIMSEG_H
#define HMDCSIMSEG_H

#include "hmdcseg.h"

class HMdcSimSeg : public HMdcSeg {
protected:
  Float_t fMomentum;
  Int_t fTrack; // track Nb
public:
  HMdcSimSeg(void) {}
  ~HMdcSimSeg(void) {}
  Float_t getMomentum(void) {return fMomentum;}
  Int_t getTrack(void) {return fTrack;}
  void setMomentum(Float_t momentum) { fMomentum=momentum; }
  void setTrack(Int_t track) { fTrack=track; }
  ClassDef(HMdcSimSeg,0)
};

#endif
