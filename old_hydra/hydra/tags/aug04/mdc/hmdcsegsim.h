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
    ~HMdcSegSim(void) {;}
    void clear(void);
    void setStatus(const Int_t f) { status=f; }
    void setNTracks(const Int_t nTr, const Int_t* listTr, const UChar_t* nTm); 
    Int_t getStatus(void) const { return status; }
    Int_t getNTracks(void) { return (nTracks>=0) ? nTracks : calcNTracks(); }
    Int_t getTrack(const Int_t n) { if(nTracks<0) calcNTracks();
      return (n>=0 && n<nTracks) ? listTracks[n]:-1; }
    UChar_t getNTimes(const Int_t n) { if(nTracks<0) calcNTracks();
      return (n>=0 && n<nTracks) ? nTimes[n]:-1; }
    Int_t calcNTracks(void);
    void print(void);
      
  ClassDef(HMdcSegSim,1) // simulated seg hit on a MDC 
};

#endif  /*HMDCSEGSIM_H */
