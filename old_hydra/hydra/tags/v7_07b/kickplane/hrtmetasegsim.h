#ifndef HRTMETASEGSIM_H
#define HRTMETASEGSIM_H

#include "hrtmetaseg.h"

class HRtMetaSegSim : public HRtMetaSeg {
 public:
  HRtMetaSegSim(void) { nTracks=0; lsTracks[0]=0;}
  ~HRtMetaSegSim(void) {}
  void setTrack(Int_t i,Int_t v) { lsTracks[i] = v; }
  void setNTimes(Int_t i, UChar_t v) {  lsTimes[i] = v; }
  void setNTracks(UChar_t n) {  nTracks = n; }
  void setValidity(Float_t v) { valid = v; }
  Int_t getNTracks(void) { return nTracks; }
  Int_t getTrack(Int_t i) { return lsTracks[i]; }
  Int_t getNTimes(Int_t i) { return lsTimes[i]; }
  
 protected:
  Int_t lsTracks[5];
  UChar_t lsTimes[5];
  UChar_t nTracks;
  Float_t valid; // Quality of matching
 public:
  ClassDef(HRtMetaSegSim,1)
};

#endif
