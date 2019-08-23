// File: hhitmatch.h
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 01/07/30 21:48:39
//
#ifndef HHITMATCH_H
#define HHITMATCH_H

#include "TObject.h"
#include "hcategory.h"

class HHitMatch: public TObject {

private:
    Int_t nSector; // sector of matched hit
    // ANGLES in DEG
    //RICH parameters
    Float_t fRichPhi;//azimuthal angle in RICH (degrees)
    Float_t fRichTheta;//polar angle in RICH (degrees)
    Int_t nRingX;// RICH hit properties copied from HRichHit
    Int_t nRingY;
    Int_t nRingPatMat;
    Int_t nRingHouTra;
    Int_t nRingAmplitude;
    Int_t nRingPadNr;
    Float_t fRingRadius;
    Float_t fRingCentroid; 
    Int_t nCentX;
    Int_t nCentY;
    Int_t nRingLocalMax4;
    Int_t fMaxClusterSize;
    Int_t fMaxThrClusterSize;
    Int_t fTests;
    Int_t indRich;
    // MDC SEG parameters
    Float_t fMdcTheta;//polar angle of MDC segment (degrees)
    Float_t fMdcPhi;//azimuthal angle of MDC segment (degrees)
    Float_t fSecMdcSegThetaCut;//window around RICH hit allowed for close track
    Float_t fSecMdcSegPhiCut;
    Float_t fClusterSize;
    Int_t iSegIOFlag;//inner seg=0, outer=1 
    Int_t nHitLay;
    Int_t indMdc;

    //TOF parameters
    Float_t fTofTof;
    Float_t fTofTheta; // polar angle of matched hit in TOF (degrees)
    Float_t fTofPhi; // azimuthal angle of matched hit in TOF  (degrees)
    Int_t indTof;

    //SHOWER-TOFINO parameters
    Float_t fShowerTheta; //polar angle of matched hit in SHOWER (degrees)
    Float_t fShowerPhi; // azimuthal angle of matched hit in SHOWER (degrees)
    Float_t fTofinoTof; // tof in tofino
    Float_t m_fSum0; //sum of charge of nine pads in pre
    Float_t m_fSum1; //sum of charge of nine pads in post
    Float_t m_fSum2; //sum of charge  of nine pads in post2
    Int_t iTofino_Mult; // multiplicity in tofino
    Int_t indShower;

    //*******************************************************************
    //*******************************************************************

    // CUTS
    Float_t fRichMdcThetaCut; // polar angle window for correlation 
                              // -fRichMdcThetaCut to fRichMdcThetaCut
    Float_t fRichTofThetaCut; 
    Float_t fRichShowerThetaCut;
    Float_t fMdcTofThetaCut;
    Float_t fMdcShowerThetaCut;

    Float_t fRichMdcPhiCut;
    Float_t fRichTofPhiCut;
    Float_t fRichShowerPhiCut; 
    Float_t fMdcTofPhiCut;
    Float_t fMdcShowerPhiCut; 

    Float_t fTofMin;
    Float_t fTofMax;
    Float_t fTofinoMin;
    Float_t fTofinoMax;

    Float_t fShowerAmplification;

    Int_t nTofinoMultCut;
    // SWITCHES
    Int_t nShowerCond;

    // CORRELATION FLAGS
    Int_t nRichMdcCorr;//flag signaling that this object contains this corr.
    Int_t nRichTofCorr;
    Int_t nRichShowerCorr;
    Int_t nMdcTofCorr;
    Int_t nMdcShowerCorr;
    // RICH HIT 
   
public:

 HHitMatch();
 virtual ~HHitMatch();

 HHitMatch(const HHitMatch& source);
 HHitMatch& operator=(const HHitMatch& source);

 void Reset();
 void dumpToStdout(); 

    Int_t getSector(void) {return nSector;};
    void setSector(Int_t s) {nSector = s;};

    Int_t getTofinoMultCut(){return nTofinoMultCut;}
    void setTofinoMultCut(Int_t m){nTofinoMultCut=m;};
    Int_t getShowerCond(){return nShowerCond;}
    void setShowerCond(Int_t m){nShowerCond=m;};
    Int_t getShowInd(void){return indShower;};
    void setShowInd(Int_t ind){ indShower = ind;};

    Float_t getShowerTheta(void) {return fShowerTheta;};
    void setShowerTheta(Float_t st){fShowerTheta=st;};
    Float_t getShowerPhi(void) {return fShowerPhi;};
    void setShowerPhi(Float_t sp){fShowerPhi=sp;};
    void  setShowerfSum0(Float_t f0){m_fSum0 = f0;};
    Float_t getShowerfSum0(void){return m_fSum0;};
    void  setShowerfSum1(Float_t f1){m_fSum1 = f1;};
    Float_t getShowerfSum1(void){return m_fSum1;};
    void  setShowerfSum2(Float_t f2){m_fSum2 = f2;};
    Float_t getShowerfSum2(void){return m_fSum2;};
    void setTofinoMul(Int_t m){iTofino_Mult=m;};
    Int_t getTofinoMul(void){return iTofino_Mult;};
    void setTofinoTof(Float_t tof){fTofinoTof = tof;};
    Float_t getTofinoTof(void) {return fTofinoTof;};

   
    Float_t getRichTheta(void) {return fRichTheta;};
    void  setRichTheta(Float_t rt){fRichTheta=rt;};
    Float_t getRichPhi(void) {return fRichPhi;};
    void setRichPhi(Float_t rp){fRichPhi=rp;};
    Int_t getRichInd(void){return indRich;};
    void setRichInd(Int_t ind){ indRich = ind;};

    Int_t getRingCenterX(){return nRingX;};
    Int_t getRingCenterY(){return nRingY;};
    Int_t getRingPatMat() {return nRingPatMat;};
    Int_t getRingHouTra() {return nRingHouTra;};
    Int_t getRingAmplitude(){return nRingAmplitude;};
    Int_t getRingPadNr(){return nRingPadNr;};
    Int_t getRingLocalMax4(){return nRingLocalMax4;};
    Float_t getCentroid(){return fRingCentroid;};
    Int_t getCentX(){return nCentX;};
    Int_t getCentY(){return nCentY;};
    Float_t getRadius(){return fRingRadius;};
    Int_t getRingMaxClusterSize(){return fMaxClusterSize;};
    Int_t getRingMaxThrClusterSize(){return fMaxThrClusterSize;};
    Int_t getRingTestFlags(){return fTests;};

    void setRingCenterX(Int_t x){ nRingX=x;};
    void setRingCenterY(Int_t y){ nRingY=y;};
    void setRingPatMat(Int_t patmat) { nRingPatMat=patmat;};
    void setRingHouTra(Int_t houtra) { nRingHouTra=houtra;};
    void setRingAmplitude(Int_t ampl){nRingAmplitude=ampl;};
    void setRingPadNr(Int_t padnr){nRingPadNr=padnr;};
    void setRingLocalMax4(Int_t locmax4){nRingLocalMax4=locmax4;};
    void setCentroid(Float_t centro){ fRingCentroid=centro;};
    void setCentX(Int_t centx){ nCentX=centx;};
    void setCentY(Int_t centy){ nCentY=centy;};
    void setRadius(Float_t rad){fRingRadius = rad;};
    void setRingMaxClusterSize(Int_t clustersize){ fMaxClusterSize=clustersize;};
    void setRingMaxThrClusterSize(Int_t fmaxthrclussize){fMaxThrClusterSize = fmaxthrclussize;};
    void setRingTestFlags(Int_t flag){fTests = flag;};

 
    Float_t getMdcTheta(void) {return fMdcTheta;};
    Int_t getMdcFlagIO(void) { return iSegIOFlag;};

    void setMdcTheta(Float_t mt){fMdcTheta=mt;};
    Float_t getMdcPhi(void) {return fMdcPhi;};
    void setMdcPhi(Float_t mp){fMdcPhi = mp;};
    void setMdcClusterSize(Float_t s) {fClusterSize =s;};
    void setMdcClusterHit(Int_t i) {nHitLay = i;};
    Int_t getMdcInd(void){return indMdc;};
    void setMdcInd(Int_t ind){ indMdc = ind;};
    void setMdcFlagIO(Int_t flag){ iSegIOFlag = flag;};
    

    void setClosePairThetaCut(Float_t mtcut){fSecMdcSegThetaCut=mtcut;};
    void setClosePairPhiCut(Float_t mpcut){fSecMdcSegPhiCut=mpcut;};
    
    Float_t getMdcClusterSize(void) {return fClusterSize;};
    Int_t getMdcClusterHit(void) {return nHitLay ;};
  
    Float_t getClosePairThetaCut(void){return fSecMdcSegThetaCut;};
    Float_t getClosePairPhiCut(void){return fSecMdcSegPhiCut;};
   
    Float_t getTofTheta(void) {return fTofTheta;};
    void setTofTheta(Float_t tt){fTofTheta=tt;};
    Float_t getTofPhi(void) {return fTofPhi;};
    void setTofPhi(Float_t tp){fTofPhi=tp;};
    void setTofTof(Float_t tof){fTofTof = tof;};
    Float_t getTofTof(void) {return fTofTof;};


    void setTofMin(Float_t tofmin){fTofMin = tofmin;};
    Float_t getTofMin(void){return fTofMin;};
    void setTofMax(Float_t tofmax){fTofMax = tofmax;};
    Float_t getTofMax(void){return fTofMax;};
    Int_t getTofInd(void){return indTof;};
    void setTofInd(Int_t ind){ indTof = ind;};   


    void setTofinoMin(Float_t tofinomin){fTofinoMin = tofinomin;};
    Float_t getTofinoMin(void){return fTofinoMin;};
    void setTofinoMax(Float_t tofinomax){fTofinoMax = tofinomax;};
    Float_t getTofinoMax(void){return fTofinoMax;};

    void setShowerAmpl(Float_t shampl){fShowerAmplification=shampl;};
    Float_t getShowerAmpl(void){return fShowerAmplification;};

    void setRichMdcThetaCut(Float_t c){fRichMdcThetaCut=c;};
    void setRichMdcPhiCut(Float_t c){fRichMdcPhiCut=c;};
    void setRichTofThetaCut(Float_t c){fRichTofThetaCut=c;};
    void setRichTofPhiCut(Float_t c){fRichTofPhiCut=c;};
    void setRichShowerThetaCut(Float_t c){fRichShowerThetaCut=c;};
    void setRichShowerPhiCut(Float_t c){fRichShowerPhiCut=c;};
    void setMdcShowerThetaCut(Float_t c){fMdcShowerThetaCut=c;};
    void setMdcShowerPhiCut(Float_t c){fMdcShowerPhiCut=c;};
    void setMdcTofThetaCut(Float_t c){fMdcTofThetaCut=c;};
    void setMdcTofPhiCut(Float_t c){fMdcTofPhiCut=c;};

    Float_t getRichMdcThetaCut(void){return fRichMdcThetaCut;};
    Float_t getRichMdcPhiCut(void){return fRichMdcPhiCut;};
    Float_t getRichTofThetaCut(void){return fRichTofThetaCut;};
    Float_t getRichTofPhiCut(void){return fRichTofPhiCut;};
    Float_t getRichShowerThetaCut(void){return fRichShowerThetaCut;};
    Float_t getRichShowerPhiCut(void){return fRichShowerPhiCut;};
    Float_t getMdcShowerThetaCut(void){return fMdcShowerThetaCut;};
    Float_t getMdcShowerPhiCut(void){return fMdcShowerPhiCut;};
    Float_t getMdcTofThetaCut(void){return fMdcTofThetaCut;};
    Float_t getMdcTofPhiCut(void){return fMdcTofPhiCut;};

    void setMatchedRichMdc(Int_t m){nRichMdcCorr=m;};
    void setMatchedRichTof(Int_t m){nRichTofCorr=m;};
    void setMatchedRichShower(Int_t m){nRichShowerCorr=m;};
    void setMatchedMdcTof(Int_t m){nMdcTofCorr=m;};
    void setMatchedMdcShower(Int_t m){nMdcShowerCorr=m;};
    
    Int_t getMatchedRichMdc(void){return nRichMdcCorr;};
    Int_t getMatchedRichTof(void){return nRichTofCorr;};
    Int_t getMatchedRichShower(void){return nRichShowerCorr;};
    Int_t getMatchedMdcTof(void){return nMdcTofCorr;};
    Int_t getMatchedMdcShower(void){return nMdcShowerCorr;};
    
   

   
 ClassDef(HHitMatch,1)
};


#endif // HHITMATCH_H
