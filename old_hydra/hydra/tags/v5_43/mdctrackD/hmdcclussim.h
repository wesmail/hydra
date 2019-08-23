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
  HMdcClusSim() {nTracks=-1;}
  ~HMdcClusSim() {;}
  Bool_t isGeant() {return kTRUE;}
  void clear() {nTracks=-1; HMdcClus::clear();}
  void printCont();
  Int_t getNTracks();
  Int_t getTrack(Int_t indx);
  Short_t getNTimesInTrack(Int_t indx);
  Int_t getNLayersInTrack(Int_t indx, Int_t m=-1);
//  HGeantMdc *getGeantMdc(Int_t indx);
  void associate(HMdcClus *clus) {nTracks=-1; HMdcClus::associate(clus);}
  ClassDef(HMdcClusSim,1)
};

#endif
