#ifndef HMDCSEGSIM_H
#define HMDCSEGSIM_H

#include "hmdcseg.h"

class HMdcSegSim : public HMdcSeg {
  protected:
    Short_t nTracks;      // number of tracks in the list of tracks
    Int_t listTracks[5];  // list of tracks
    UChar_t nTimes[5];    // number of times from each track
    Int_t status;         // status of seg. (e.g. 1 if ok)
  public:
    HMdcSegSim(void) {clear();}
    ~HMdcSegSim(void) {}
    void  clear(void);
    void  setStatus(Int_t f) { status=f; }
    void  setNTracks(Int_t nTr, const Int_t* listTr, const UChar_t* nTm);
    void  setNumNoiseWires(Int_t n);
    Int_t calcNTracks(void);
    
    inline Int_t   getStatus(void) const        {return status;}
    inline Int_t   getNTracks(void) const       {return nTracks;}
    inline Int_t   getTrack(Int_t n) const;
    inline UChar_t getNTimes(Int_t n) const;
    inline Int_t   getNumNoiseWires(void) const;

    void print(void);

  ClassDef(HMdcSegSim,1) // simulated seg hit on a MDC 
};

inline Int_t HMdcSegSim::getTrack(Int_t n) const {
  return (n>=0 && n<nTracks) ? listTracks[n] : -1;
}

inline UChar_t HMdcSegSim::getNTimes(Int_t n) const {
  return (n>=0 && n<nTracks) ? nTimes[n]:-1;
}

inline Int_t HMdcSegSim::getNumNoiseWires(void) const {
  return (listTracks[4]==-99) ? nTimes[4] : 0;
}
  
#endif  /*HMDCSEGSIM_H */
