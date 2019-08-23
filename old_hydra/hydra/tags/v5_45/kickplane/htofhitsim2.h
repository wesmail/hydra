#ifndef HTOFHITSIM2_H
#define HTOFHITSIM2_H

#include "htofhitsim.h"

class HTofHitSim2 : public HTofHitSim {
protected:
  Int_t trackNb[20];
  Int_t nTrack;
  Float_t sim_y;
  Int_t pid;
  Double_t p;
  Int_t nHits;
public:
  HTofHitSim2(void) {nHits=0;nTrack=0;}
  ~HTofHitSim2(void) {}
  void clear(void);
  Int_t getPid(void) { return pid; }
  void setPid(Int_t p) { pid=p;}
  Double_t getMomentum(void) { return p; }
  void setMomentum(Double_t pp) { p=pp; }
  Int_t getNHits(void) { return nHits; }
  void setNHits(Int_t n) { nHits=n; }
  Int_t getTrackNb(Int_t idx) { return trackNb[idx]; }
  void addTrack(Int_t tra) { trackNb[nTrack]=tra; nTrack++;}
  Bool_t containsTrack(Int_t tr) {
    for (int i=0; i<nTrack;i++) if (trackNb[i]==tr) return kTRUE;
    return kFALSE;
  }
  Int_t getTrack(void) { return trackNb[0]; }
  Float_t getY(void) { return sim_y; }
  void setY(Float_t y) { sim_y=y; }
  
  ClassDef(HTofHitSim2,1) // simulated hit data level of TOF
};

#endif  /* HTOFHITSIM_H */
