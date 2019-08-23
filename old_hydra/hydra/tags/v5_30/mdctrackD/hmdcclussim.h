#ifndef HMDCCLUSSIM_H
#define HMDCCLUSSIM_H

#include "hmdclistcells.h"
#include "hmdcclus.h"

class HMdcClusSim : public HMdcClus {
protected:
  Int_t nTracks;       // num. of GEANT tracks in cluster
  Int_t listTr[5];     // list of tracks
  Short_t nHits[5];    // num. of hits in cluster from track
public:
  HMdcClusSim() {nTracks=-1;}
  ~HMdcClusSim() {;}
  Bool_t isGeant() {return kTRUE;}
  void clear() {nTracks=-1; HMdcClus::clear();}
  void printCont();
  Int_t getNTracks();
  Int_t getTrack(Int_t indx);
  Short_t getNHitsInTrack(Int_t indx);
//  HGeantMdc *getGeantMdc(Int_t indx);
  void associate(HMdcClus *clus) {nTracks=-1; HMdcClus::associate(clus);}
  ClassDef(HMdcClusSim,1)
};

#endif
