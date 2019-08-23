#ifndef HSHOWERRAWSIM_H
#define HSHOWERRAWSIM_H
#pragma interface

#include "hshowerraw.h"

class HShowerRawSim : public HShowerRaw{
protected:
  Int_t nTrack1;   // Track index for first hit
  Int_t nTrack2;   // Track index for last hit

public:
  HShowerRawSim(void) : nTrack1(0), nTrack2(0) {}
  ~HShowerRawSim(void) {}
  void clear(void);
  inline void setNTrack1(const Int_t n) {nTrack1=n;} //set trk nb of hit 1
  inline void setNTrack2(const Int_t n) {nTrack2=n;} //set trk nb of hit 2
  inline Int_t getNTrack1(void) const {return nTrack1;} //get trk nb of hit 1
  inline Int_t getNTrack2(void) const {return nTrack2;} //get trk nb of hit 2 
  ClassDef(HShowerRawSim,1) // SHOWER simulated RAW data
};

class HShowerRawMatrSim : public HShowerRawSim{
public:
  HShowerRawMatrSim(){}
 
  ClassDef(HShowerRawMatrSim,1)  // SHOWER simulated RAW data
};


#endif /* !HSHOWERRAWSIM_H */
