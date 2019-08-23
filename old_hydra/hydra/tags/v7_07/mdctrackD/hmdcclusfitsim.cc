//*--- AUTHOR: Vladimir Pechenov
//*--- Modified: Vladimir Pechenov 05/04/2005

using namespace std;
#include "hmdcclusfitsim.h"
#include <iostream> 
#include <iomanip>

//////////////////////////////////////////////////////////////////////////////
//
// HMdcClusFitSim
//
// Container class keep geant data for HMdcClusFit
//
//////////////////////////////////////////////////////////////////////////////




ClassImp(HMdcClusFitSim)

HMdcClusFitSim::HMdcClusFitSim() {
  geantTrack=-1;
  nTracks=nTracksClus=nTrWires=nTrWiresClus=0;
  direction=kTRUE;
  clear();
}

void HMdcClusFitSim::print() const {
  HMdcClusFit::print();
  printf(" GEANT Track line:    (%8.2f,%8.2f,%7.2f) - (%8.2f,%8.2f,%8.2f)\n",
      x1geant,y1geant,z1geant,x2geant,y2geant,z2geant);
  printf(" GEANT: %itr. in cluster, %itr. in fit output.",nTracksClus,nTracks);
  printf(" Track %i:  %i wires passed fit from %i in cluster.\n",
      geantTrack,nTrWires,nTrWiresClus);
}

void HMdcClusFitSim::setFakeTrack(Bool_t dir) {
  x1geant=y1geant=z1geant=x2geant=y2geant=z2geant=-10000.;
  direction=dir;
}
