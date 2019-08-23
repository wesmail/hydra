#ifndef HTOFINOCALSIM_H
#define HTOFINOCALSIM_H

#include "hcategory.h"
#include "htofinocal.h"

#define MAXHITS 5

class HTofinoCalSim : public HTofinoCal {
protected:
    Int_t nHits;             // number of hits in cell
    Int_t nTrack[MAXHITS];   // Geant track number(s)
    Float_t fGeaTof[MAXHITS];   // Geant Tof(s)
public:
    HTofinoCalSim(void);
    ~HTofinoCalSim(void);

    void clear();

    void incNHit() {nHits++;}
    void setNHit(Int_t n) {nHits = n;}
    Int_t getNHit() {return nHits;}

    void setTrack(Int_t n) {if (nHits<=MAXHITS) nTrack[nHits-1] = n;}
    void setGeaTof(Float_t tof) {if (nHits<=MAXHITS) fGeaTof[nHits-1] = tof;}
    Int_t getTrack(Int_t i=0) {
      if (i>=0 && i<nHits) return nTrack[i];
      else return -1;
    }
    Float_t getGeaTof(Int_t i=0) {
      if (i>=0 && i<nHits) return fGeaTof[i];
      else return -1;
    }

    ClassDef(HTofinoCalSim,1)  // TOFINO detector calibrated Sim data
};

#endif /* ! HTOFINOCALSIM_H */



