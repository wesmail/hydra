#ifndef HRPCCLUSTERSIM_H
#define HRPCCLUSTERSIM_H

#include "hrpccluster.h"

class HRpcClusterSim : public HRpcCluster {

protected:
  Int_t TrackList[4];  //List of the four tracks contributing to the cluster, ordered as:
                       //UpLeft, UpRight, DownLeft, DownRight.
  Int_t RefList[4];    //List of the four references to the geantrpc objects contributing 
                       //to the cluster, ordered as: UpLeft, UpRight, DownLeft, DownRight.
                       //Note: In case of cluster type = 1, the order is simply:
                       //Left, Right, -, -

  Int_t Track;         //Track preferred by the cluster finder. Normally is the one that contributes
                       //more times to the cluster

public:
  HRpcClusterSim(void) {}
  ~HRpcClusterSim(void) {}
  void clear(void);
  
  void  resetTrackList()                         {for(Int_t i=0;i<4;i++)TrackList[i] = -999;}
  void  resetRefList()                           {for(Int_t i=0;i<4;i++)RefList[i]   = -999;}
  void  setTrackList (Int_t i, Int_t track)      {TrackList[i]=track;}
  void  setTrackList (Int_t* array)              {for(Int_t i=0;i<4;i++)TrackList[i] = array[i];}
  void  setRefList (Int_t i, Int_t ref)          {RefList[i]=ref;}
  void  setRefList (Int_t* array)                {for(Int_t i=0;i<4;i++)RefList[i]   = array[i];}
  void  setTrack(Int_t atrack)                   {Track=atrack; }

  Int_t getTrack()                               {return Track; }

  void  getTrackList(Int_t* array)               {for(Int_t i=0;i<4;i++) array[i]=TrackList[i];}
  void  getRefList(Int_t* array)                 {for(Int_t i=0;i<4;i++) array[i]=RefList[i];}

  Bool_t isTrack(Int_t track);                                 
  Bool_t isRef(Int_t ref);                                 
    
  ClassDef(HRpcClusterSim,1)
};

#endif  /* HRPCCLUSTERSIM_H */

