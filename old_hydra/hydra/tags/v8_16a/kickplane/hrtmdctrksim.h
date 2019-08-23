#ifndef HRTMDCTRKSIM_H
#define HRTMDCTRKSIM_H

#include "hrtmdctrk.h"

class HRtMdcTrkSim : public HRtMdcTrk {
 public:
  HRtMdcTrkSim(void) {}
  ~HRtMdcTrkSim(void) {}
  Int_t getNTracks(void) { return nTracks; }
  Int_t getTrack(Int_t i) { return lsTracks[i]; }
  Int_t getNTimes(Int_t i) { return lsTimes[i]; }
  Int_t getTotalTimes(void) { return totalTimes; }
  void setNTracks(Int_t i) { nTracks = i; }
  void setTrack(Int_t idx,Int_t i) { lsTracks[idx] = i; }
  void setNTimes(Int_t idx,UChar_t i) { lsTimes[idx] = i; }
  void setTotalTimes(UChar_t t) { totalTimes = t; }

 protected:
  Int_t lsTracks[5];  //List of common tracks between segments
  UChar_t lsTimes[5]; //Contribution for each common track
  UChar_t totalTimes; //Total number of signals originating the track. Common and no common
  UChar_t nTracks;    //Nb of common tracks
 public:
  ClassDef(HRtMdcTrkSim,1)
};

#endif
