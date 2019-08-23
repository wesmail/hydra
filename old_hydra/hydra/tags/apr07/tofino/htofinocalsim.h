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

    void incNHit() { if(nHits<MAXHITS) nHits++;}
    void setNHit(Int_t n)
    {
	if (n<=MAXHITS && n>=0) nHits = n;
        else if (n>MAXHITS)     nHits = MAXHITS;
	else if (n>0)           nHits = 0;
    }
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
    void resetTrack (Int_t   val=-1){for(Int_t i=0;i<MAXHITS;i++) {nTrack[i]  =val;} nHits=0;}
    void resetGeaTof(Float_t val=-1){for(Int_t i=0;i<MAXHITS;i++) {fGeaTof[i] =val;} nHits=0;}
    void fillTrack(Int_t i,Int_t val=-1)   {if(i>=0 && i<MAXHITS)nTrack[i] =val;}
    void fillGeaTof(Int_t i,Float_t val=-1){if(i>=0 && i<MAXHITS)fGeaTof[i] =val;}
    ClassDef(HTofinoCalSim,1)  // TOFINO detector calibrated Sim data
};

#endif /* ! HTOFINOCALSIM_H */



