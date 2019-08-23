//*-- AUTHOR : Diego Gonzalez-Diaz 19/12/2007

//_HADES_CLASS_DESCRIPTION
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// HRpcClusterSim                                                            //
// this is the ClusterSim data level for simulated data                      //
// it stores 4 tracksID correponding to the 2+2 contributing signals         //
///////////////////////////////////////////////////////////////////////////////


#include "hrpcclustersim.h"

ClassImp(HRpcClusterSim)

void HRpcClusterSim::clear(void) {
  // clears the object
  HRpcClusterSim::clear();

  Track=-999;
  for(Int_t i=0;i<4;i++)     TrackList[i]   = -999;
  for(Int_t i=0;i<4;i++)     RefList[i]     = -999;

}

Bool_t HRpcClusterSim::isTrack(Int_t track) {

  for(Int_t i=0;i<4;i++){
    if (TrackList[i]==track) return kTRUE;
    else return kFALSE;
  }
  return -1;
}

Bool_t HRpcClusterSim::isRef(Int_t ref) {

  for(Int_t i=0;i<4;i++){
    if (RefList[i]==ref) return kTRUE;
    else return kFALSE;
  }
  return -1;
}
