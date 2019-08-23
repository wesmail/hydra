#ifndef HKICKTRACKSIM_H
#define HKICKTRACKSIM_H

#include "hkicktrack.h"
#include "hmdcsegsim.h"
#include "hshowerhittoftrack.h"
#include "htofhitsim.h"

class HKickTrackSim : public HKickTrack {
protected:
  Int_t fNMdcTracks;     //Nb of contributing mdc tracks (Experimental)
  Int_t fNMetaTracks;    //Nb of contributin Meta tracks (Experimental)
  Int_t fMdcTracks[5];   //GEANT ids for tracks in MDC   (Experimental) 
  Int_t fMetaTracks[2];  //GEANT ids for tracks in Meta  (Experimental) 
public:
  HKickTrackSim(void) {}
  ~HKickTrackSim(void) {}
  void setMdcTrack(Int_t i,Int_t id) { fMdcTracks[i] = id; }
  void setTofTrack(Int_t i,Int_t id) { fMetaTracks[i]=id; }
  inline void setMdcTracks(HMdcSegSim *seg);
  inline void setMetaTracks(HTofHitSim *hit);
  inline void setMetaTracks(HShowerHitTofTrack *hit);
  Int_t getMdcTrack(Int_t i) { return fMdcTracks[i]; }
  Int_t getMetaTrack(Int_t i) { return fMetaTracks[i]; }
  Int_t getNMdcTracks(void) { return fNMdcTracks; }
  Int_t getNMetaTracks(void) { return fNMetaTracks; }
  ClassDef(HKickTrackSim,1)
};

// ** Inline functions *****
void HKickTrackSim::setMdcTracks(HMdcSegSim *seg) {
  fNMdcTracks = seg->getNTracks();
  for (int i=0; i<fNMdcTracks; i++) {
    fMdcTracks[i] = seg->getTrack(i);
  }
}

void HKickTrackSim::setMetaTracks(HTofHitSim *hit) {
  fNMetaTracks = 2;
  fMetaTracks[0] = hit->getNTrack1();
  fMetaTracks[1] = hit->getNTrack2();
}

void HKickTrackSim::setMetaTracks(HShowerHitTofTrack *hit) {
  fNMetaTracks = 1;
  fMetaTracks[0] = hit->getTrack();
}

#endif
