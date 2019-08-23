#ifndef HMDCHITSIM_H
#define HMDCHITSIM_H

#include "hmdchit.h"

class HMdcHitSim : public HMdcHit {
  protected:
    Int_t nTracks;        // number of tracks in the list of tracks
    Int_t listTracks[5];  // list of tracks
    UChar_t nTimes[5];    // num. of drift times from each track
    Int_t status;         // status of hit (e.g. 1 if ok)
  public:
    HMdcHitSim(void){clear();}
    ~HMdcHitSim(void) {;}
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
    
  ClassDef(HMdcHitSim,1) // simulated hit on a MDC 
};

#endif  /* HMDCHITSIM_H */
