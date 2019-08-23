// File: hhitmatchsim.h
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 02/03/03 20:55:49
//
#ifndef HHITMATCHSIM_H
#define HHITMATCHSIM_H

#include "hhitmatch.h"
#include "htrackinfo.h"

class HHitMatchSim: public HHitMatch {

protected :
    
HTrackInfo trackMatched;//info from GEANT
Int_t isFakeFlag;
Int_t isLeptonFlag;
     Int_t isGLeptonRing;
     Int_t isGCLeptonRing;
    Int_t nNbGPart;
    Int_t numPhot;//if larger than zero, lepton went through mirror
    // and ring is confirmed and can be correlated !!!
    Int_t isGLepInMDC;//lepton that gave numPhot seen in mod==0 lay==0 of MDC 
    //Int_t isHadronFlag;
public:
   
    HTrackInfo * getTrackInfoObj() {return & trackMatched;};

public:

 HHitMatchSim();
 virtual ~HHitMatchSim();

 HHitMatchSim(const HHitMatchSim& source);

 virtual HHitMatchSim& operator=(const HHitMatchSim& source);
 void resetSim();
 void dumpToStdoutSim();
 Int_t isFake(){return isFakeFlag;};
 void setFakeFlag(Int_t i){isFakeFlag=i;};
 Int_t isLepton(){return isLeptonFlag;};
 void setLeptonFlag(Int_t i){isLeptonFlag=i;};
    void setNumPhot(Int_t p){numPhot=p;};
    Int_t getNumPhot(){return numPhot;};//number of photons emitted by the lepton that contributed to the ring
    void setGLepInMDC(Int_t p){isGLepInMDC=p;};
    Int_t getGLepInMDC(){return isGLepInMDC;};
    Int_t isGCLepRing(){return isGCLeptonRing;};
    void setGCLepRing(Int_t i){isGCLeptonRing=i;};
    Int_t isGLepRing(){return isGLeptonRing;};
    void setGLepRing(Int_t i){isGLeptonRing=i;};
    void setNbGPart(Int_t i){nNbGPart=i;};
    Int_t getNbGPart(){return nNbGPart;};
//  Int_t isHadron(){return isHadronFlag;};
//  void setHadronFlag(Int_t i){isHadronFlag=i;};
 ClassDef(HHitMatchSim,1)
};


#endif // HHITMATCHSIM_H
