#ifndef HKICKIMPACTSIM_H
#define HKICKIMPACTSIM_H

#include "hkickimpact.h"

class HKickImpactSim : public HKickImpact {
protected:
  Float_t fMom; //Momentum from simulation 
  Int_t fTrack;
public:
  HKickImpactSim(void) {fMom=-1;}
  ~HKickImpactSim(void) {}
  Float_t getMomentum(void) { return fMom; }
  void setMomentum(Float_t mom) { fMom=mom; }
  Int_t getTrack(void) { return fTrack; }
  void setTrack(Int_t track) { fTrack=track; }
  ClassDef(HKickImpactSim,0) //Impact in kick plane
};

#endif
