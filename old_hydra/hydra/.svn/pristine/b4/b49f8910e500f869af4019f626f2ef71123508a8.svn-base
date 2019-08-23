// $Id: hhitmatchsim.h,v 1.6 2003-02-05 14:25:05 fabietti Exp $
// Last update by Thomas Eberl: 02/09/25 17:50:51
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
     Int_t isGLeptonRing; // isGLeptonRing=0: this ring is a fake, no lepton contributed
    // isGLeptonRing=-1: no Rich hit in tracklet
    // isGLeptonRing>0: isGLeptonRing>0 photons really contributed to the hit pattern
     Int_t isGCLeptonRing;// isGCLeptonRing= -1 this ring is a fake, no lepton contributed
    //isGCLeptonRing = 0: the ring is a lepton ring but its track doesnt match to any other detector track
    //isGCLeptonRing = n, n>0 : n is the number of correlation with other detector tracks that are fullfilled.    
    Int_t nNbGPart;
    Int_t isLeptonOnMirror;//=0: the ring doesnt match to any leptons on the mirror
    //=1 :the ring correponds to a single lepton on the mirror
    // =2: the ring corresponds to 2 leptons on the mirror
    Int_t numPhot1;//if larger than zero, lepton went through mirror
    // and ring is confirmed and can be correlated !!!
    Int_t numPhot2;//if larger than zero, a second lepton went through mirror
    // and ring is confirmed and can be correlated !!!
    Float_t weightRatio;//it is the ratio between the weight of the lepton1 and lepton2.
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
     void setGLepInMDC(Int_t p){isGLepInMDC=p;};
    Int_t getGLepInMDC(){return isGLepInMDC;};
    Int_t isGCLepRing(){return isGCLeptonRing;};
    void setGCLepRing(Int_t i){isGCLeptonRing=i;};
    Int_t isGLepRing(){return isGLeptonRing;};
    void setGLepRing(Int_t i){isGLeptonRing=i;};
    void setNbGPart(Int_t i){nNbGPart=i;};
    Int_t getNbGPart(){return nNbGPart;};
    void setLeptonOnMirror(Int_t i){isLeptonOnMirror=i;}
    void setNumPhot1(Int_t i){numPhot1=i;}
    void setNumPhot2(Int_t i){numPhot2=i;}
    void setWeightRatio(Float_t i){weightRatio=i;}
    Int_t getLeptonOnMirror(){ return isLeptonOnMirror;}
    Int_t getNumPhot1(){ return numPhot1;}
    Int_t getNumPhot2(){ return numPhot2;}
    Float_t getWeightRatio(){ return weightRatio;}
//  Int_t isHadron(){return isHadronFlag;};
//  void setHadronFlag(Int_t i){isHadronFlag=i;};
 ClassDef(HHitMatchSim,1)
};


#endif // HHITMATCHSIM_H
