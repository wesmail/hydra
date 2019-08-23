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
  
  Int_t nTracksM[2];      //! [mod] num. of GEANT tracks in cluster
  Int_t listTrM[2][5];    //! list of tracks
  Short_t nTimesM[2][5];  //! num. of hits in cluster from track
public:
  HMdcClusSim(void) {nTracks=-1; nTracksM[0]=nTracksM[1]=-1;}
  HMdcClusSim(HMdcList12GroupCells& lcells)  : HMdcClus(lcells) {
    nTracks=-1; nTracksM[0]=nTracksM[1]=-1;}
  ~HMdcClusSim() {}
  Bool_t isGeant(void) const {return kTRUE;}
  void clear(void) {nTracks=-1; HMdcClus::clear();}
  void calcTrList(void);
  void printCont(Bool_t fl=kTRUE) const;
  Int_t getNTracks(void) const {return nTracks;}
  Int_t getTrack(Int_t indx) const {
    return (indx>=0 && indx<nTracks) ? listTr[indx] : -1;
  }
  Short_t getNTimesInTrack(Int_t indx) const {
    return (indx<0 || indx>=nTracks) ? 0:nTimes[indx];
  }
  Int_t getNLayersInTrack(Int_t indx, Int_t m=-1) const;
  void calcTrListMod(HMdcList12GroupCells& list,Int_t m);
  Int_t getNTracksMod(Int_t m) const {return nTracksM[m&1];}
  Int_t getTrackMod(Int_t m,Int_t indx) const {
    return (indx>=0 && indx<nTracksM[m&1]) ? listTrM[m&1][indx] : -1;
  }
  Short_t getNTimesInMod(Int_t m,Int_t indx) const {
    return (indx<0 || indx>=nTracksM[m&1]) ? 0:nTimesM[m&1][indx];
  }
  const Int_t* getListTrM(Int_t m) const {return listTrM[m&1];}
  const Short_t* getNTimesM(Int_t m) const {return nTimesM[m&1];}
  
  ClassDef(HMdcClusSim,1)
};

#endif
