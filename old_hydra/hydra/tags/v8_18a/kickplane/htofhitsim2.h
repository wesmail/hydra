#ifndef HTOFHITSIM2_H
#define HTOFHITSIM2_H

#include "htofhitsim.h"

class HTofHitSim2 : public HTofHitSim {
protected:
  Float_t sim_y;
  Int_t pid;
  Double_t p;
  Int_t nHits;
public:
  HTofHitSim2(void) {nHits=0;}
  ~HTofHitSim2(void) {}
  void clear(void);
  Int_t getPid(void) { return pid; }
  void setPid(Int_t p) { pid=p;}
  Double_t getMomentum(void) { return p; }
  void setMomentum(Double_t pp) { p=pp; }
  Int_t getNHits(void) { return nHits; }
  void setNHits(Int_t n) { nHits=n; }
  Bool_t containsTrack(Int_t tr) {
    if (tr == getNTrack1() || tr == getNTrack2()) return kTRUE;
    else return kFALSE;
  }
  Float_t getY(void) { return sim_y; }
  void setY(Float_t y) { sim_y=y; }
  
  ClassDef(HTofHitSim2,1) // simulated hit data level of TOF
};

#endif  /* HTOFHITSIM_H */
