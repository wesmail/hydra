#ifndef HKICKTRACKSIMB_H
#define HKICKTRACKSIMB_H
#include "TObject.h"
//#include "hbasetrack.h"
#include "hkicktrackB.h"
#include "hmdcsegsim.h"
#include "htofhitsim.h"
#include "htofclustersim.h"
#include "hshowerhittoftrack.h"

class HKickTrackSimB : public HKickTrackB
{
protected:
  Int_t fNMdcTracks;     //Nb of contributing mdc tracks (Experimental)
  Int_t fNMetaTracks;    //Nb of contributin Meta tracks (Experimental)
  Int_t fMdcTracks[5];   //GEANT ids for tracks in MDC   (Experimental) 
  Int_t fMetaTracks[2];  //GEANT ids for tracks in Meta  (Experimental) 
public:
  HKickTrackSimB(void);
 ~HKickTrackSimB(void) {}
  void setMdcTrack(Int_t i,Int_t id) { fMdcTracks[ i]= id; }
  void setTofTrack(Int_t i,Int_t id) { fMetaTracks[i]= id; }
  inline void setMdcTracks( HMdcSegSim *seg);
  inline void setMetaTracks(HTofHitSim *hit);
  inline void setMetaTracks(HTofClusterSim *cluster);
  inline void setMetaTracks(HShowerHitTofTrack *hit);
  Int_t getMdcTrack(Int_t i) {  return fMdcTracks[i]; }
  Int_t getMetaTrack(Int_t i) { return fMetaTracks[i];}
  Int_t getNMdcTracks(void) {   return fNMdcTracks;   }
  Int_t getNMetaTracks(void) {  return fNMetaTracks;  }
  ClassDef(HKickTrackSimB,1)
};

// ** Inline functions *****
inline void HKickTrackSimB::setMdcTracks(HMdcSegSim *seg) {
  fNMdcTracks = seg->getNTracks();
  for (Int_t i=0; i<fNMdcTracks; i++) {
    fMdcTracks[i] = seg->getTrack(i);
  }
}

inline void HKickTrackSimB::setMetaTracks(HTofHitSim *hit) {
  fNMetaTracks = 2;
  fMetaTracks[0] = hit->getNTrack1();
  fMetaTracks[1] = hit->getNTrack2();
}

inline void HKickTrackSimB::setMetaTracks(HTofClusterSim *cluster) {
  fNMetaTracks = 2;
  fMetaTracks[0] = cluster->getNTrack1(0);
  fMetaTracks[1] = cluster->getNTrack2(0);
}

inline void HKickTrackSimB::setMetaTracks(HShowerHitTofTrack *hit) {
  fNMetaTracks = 1;
  fMetaTracks[0] = hit->getTrack();
}

#endif
