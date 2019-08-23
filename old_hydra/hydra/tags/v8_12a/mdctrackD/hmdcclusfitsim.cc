//*--- AUTHOR: Vladimir Pechenov
//*--- Modified: Vladimir Pechenov 05/04/2005

using namespace std;
#include "hmdcclusfitsim.h"
#include <iostream> 
#include <iomanip>
#include "hpidphysicsconstants.h"

//_HADES_CLASS_DESCRIPTION 
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
  particleID=0;
  momentum=0.;
  clear();
}

void HMdcClusFitSim::printSimVsRec(void) const {
  printf(" GEANT  Track %i ID=%i",geantTrack,particleID);
  if(particleID>0) printf(" (%s)",HPidPhysicsConstants::pid(particleID));
  printf(" p=%.0f MeV/c:  %i wires passed fit from %i in cluster.\n",
      momentum,nTrWires,nTrWiresClus);
  printf(" Param: %9.3f %9.3f %9.3f %9.3f\n",x1,y1,x2,y2);
  printf(" Geant: %9.3f %9.3f %9.3f %9.3f\n",x1geant,y1geant,x2geant,y2geant);
  printf(" G.-R.: %9.3f %9.3f %9.3f %9.3f\n",x1geant-x1,y1geant-y1,x2geant-x2,
      y2geant-y2);
}

void HMdcClusFitSim::print() const {
  HMdcClusFit::print();
  printf(" GEANT Track line:    (%8.2f,%8.2f,%7.2f) - (%8.2f,%8.2f,%8.2f)\n",
      x1geant,y1geant,z1geant,x2geant,y2geant,z2geant);
  printf(" GEANT: %itr. in cluster, %itr. in fit output.\n",nTracksClus,nTracks);
  printf(" GEANT  Track %i ID=%i",geantTrack,particleID);
  if(particleID>0) printf(" (%s)",HPidPhysicsConstants::pid(particleID));
  printf(" p=%.0f MeV/c:  %i wires passed fit from %i in cluster.\n",
      momentum,nTrWires,nTrWiresClus);
}

void HMdcClusFitSim::setFakeTrack(Bool_t dir) {
  x1geant=y1geant=z1geant=x2geant=y2geant=z2geant=-10000.;
  direction=dir;
}

void HMdcClusFitSim::Streamer(TBuffer &R__b)
{
   // Stream an object of class HMdcClusFitSim.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      HMdcClusFit::Streamer(R__b);
      R__b >> nTracks;
      R__b >> nTracksClus;
      R__b >> geantTrack;
      R__b >> nTrWires;
      R__b >> nTrWiresClus;
      R__b >> primary;
      R__b >> x1geant;
      R__b >> y1geant;
      R__b >> z1geant;
      R__b >> x2geant;
      R__b >> y2geant;
      R__b >> z2geant;
      R__b >> direction;
      if(R__v>2) {
        R__b >> particleID;
        R__b >> momentum;
      }
      R__b.CheckByteCount(R__s, R__c, HMdcClusFitSim::IsA());
   } else {
      R__c = R__b.WriteVersion(HMdcClusFitSim::IsA(), kTRUE);
      HMdcClusFit::Streamer(R__b);
      R__b << nTracks;
      R__b << nTracksClus;
      R__b << geantTrack;
      R__b << nTrWires;
      R__b << nTrWiresClus;
      R__b << primary;
      R__b << x1geant;
      R__b << y1geant;
      R__b << z1geant;
      R__b << x2geant;
      R__b << y2geant;
      R__b << z2geant;
      R__b << direction;
      R__b.SetByteCount(R__c, kTRUE);
   }
}
