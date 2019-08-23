#ifndef HMDCCLUSSIM_H
#define HMDCCLUSSIM_H

#include "hmdclistcells.h"
#include "hmdcclus.h"

class HMdcClusSim : public HMdcClus {
protected:
  Int_t nTracks;          // num. of GEANT tracks in cluster
  Int_t listTr[5];        // list of tracks
  Short_t nTimes[5];      // num. of hits in cluster from track
  UChar_t nLayers[5][2];  // num. of layers with hits from track
public:
  HMdcClusSim(void) {nTracks=-1;}
  HMdcClusSim(HMdcList12GroupCells& lcells)  : HMdcClus(lcells) {
    nTracks=-1;}
  ~HMdcClusSim() {}
  Bool_t isGeant(void) const {return kTRUE;}
  void clear(void) {nTracks=-1; HMdcClus::clear();}
  void calcTrList(void);
  void printCont(void) const;
  Int_t getNTracks(void) const {return nTracks;}
  Int_t getTrack(Int_t indx) const;
  Short_t getNTimesInTrack(Int_t indx) const;
  Int_t getNLayersInTrack(Int_t indx, Int_t m=-1) const;
  ClassDef(HMdcClusSim,1)
};

#endif
