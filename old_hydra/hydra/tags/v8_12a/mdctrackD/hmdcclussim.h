#ifndef HMDCCLUSSIM_H
#define HMDCCLUSSIM_H

#include "hmdcclus.h"

class HMdcClusSim : public HMdcClus {
protected:
  Int_t   nTracks;        // num. of GEANT tracks in cluster
  Int_t   listTr[5];      // list of tracks
  Short_t nTimes[5];      // num. of hits in cluster from track
  UChar_t nLayers[5][2];  // num. of layers with hits from track
  
  Int_t   nTracksM[2];    //! [mod] num. of GEANT tracks in cluster
  Int_t   listTrM[2][5];  //! list of tracks
  Short_t nTimesM[2][5];  //! num. of hits in cluster from track
public:
  HMdcClusSim(void) {setDef();}
  HMdcClusSim(HMdcList12GroupCells& lcells)  : HMdcClus(lcells) {setDef();}
  ~HMdcClusSim() {}
  
  void    setDef(void);
  void    setNTracks(Int_t nTrs)                {nTracks = nTrs;}
  void    clear(void)                           {setDef(); HMdcClus::clear();}
  void    setTrack(Int_t ind,Int_t tr,Short_t nTm,UChar_t lM1,UChar_t lM2);
  void    setNTracksM(Int_t modi,Int_t nTrs);
  void    setTrackM(Int_t modi,Int_t ind,Int_t tr,Short_t nTm);
  void    calcTrList(void);
  void    calcTrListMod(HMdcList12GroupCells& list,Int_t m);
  
  Bool_t  isGeant(void) const                   {return kTRUE;}
  Int_t   getArrsSize(void) const               {return 5;}
  Int_t   getNTracks(void) const                {return nTracks;}
  Int_t   getTrack(Int_t indx) const;
  Short_t getNTimesInTrack(Int_t indx) const;
  Int_t   getNLayersInTrack(Int_t indx,Int_t m=-1) const;
  
  Int_t   getNTracksMod(Int_t m) const          {return nTracksM[m&1];}
  Int_t   getTrackMod(Int_t m,Int_t indx) const;
  Short_t getNTimesInMod(Int_t m,Int_t indx) const;
  Int_t   getTrackIndex(Int_t tr) const;
  Int_t   getNoiseIndex(void) const;
  const Int_t*   getListTrM(Int_t m) const      {return listTrM[m&1];}
  const Short_t* getNTimesM(Int_t m) const      {return nTimesM[m&1];}
  
  void    printCont(Bool_t fl=kTRUE) const;
  
  ClassDef(HMdcClusSim,1)
};

#endif
