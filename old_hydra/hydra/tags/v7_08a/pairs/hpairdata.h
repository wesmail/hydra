// $Id: hpairdata.h,v 1.8 2005-11-18 14:14:39 sudol Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2005-05-11 18:31:32
//
// ---------------------------------------------------------------------------
#ifndef HPAIRDATA_H
#define HPAIRDATA_H
// ---------------------------------------------------------------------------
#include <TObject.h>
#include <TString.h>
// ---------------------------------------------------------------------------
class HPidHitData;
class HPidParticle;
class HPidTrackData;
// ---------------------------------------------------------------------------
class HPair;
// ---------------------------------------------------------------------------

class HPairData : public TObject {


public:

    HPairData();
    HPairData(HPair*);
    virtual ~HPairData();
    virtual void print(void);
    Float_t* getArray(void) const;
    static const TString& getVarNames(void);
    Float_t  getOpeningAngleDeg(void) const {return fOpang;};
    Char_t   getCharge(void) const {return cCharge;};
    Char_t   getIsCutFlag(void) const {return cIsCut;};
    Float_t  getInvMass(void) const {return fMass;};
    Float_t  getRapidity(void) const {return fRapidity;};
    Float_t  getTransverseMomentum(void) const {return fTransverseMomentum;};
    Short_t  getParticleIndex1(void) const {return sIdxP1;}; 
    Short_t  getParticleIndex2(void) const {return sIdxP2;};
    Float_t  getProbability1(void) const {return fProbability1;};
    Float_t  getProbability2(void) const {return fProbability2;};
    Int_t    getPid1(void) const {return nPid1;};
    Int_t    getPid2(void) const {return nPid2;};

    Float_t getThetaDeg1(void) {return  fThetaDeg1;}
    Float_t getThetaDeg2(void) {return  fThetaDeg2;}
    Float_t getPhiDeg1(void) {return  fPhiDeg1;}
    Float_t getPhiDeg2(void) {return  fPhiDeg2;}
    Float_t getMom1(void) {return  fMom1;}
    Float_t getMom2(void) {return  fMom2;}
    Bool_t getIsCPCandidate1(void) {return bIsCPCandidate1;}
    Bool_t getIsCPCandidate2(void) {return bIsCPCandidate2;}

    Int_t    hasDoubleDetectorHits(void) const;
    Int_t    getBitFieldPassedCuts(void) const {return nBitFieldPassedCuts;};
    static const Int_t getNbDataItems(){return DATAITEMS;};


protected:

    virtual void reset(void);
    virtual Bool_t init(HPair*);

    Int_t nBitFieldPassedCuts;
    Float_t fMass;
    Float_t fOpang;
    Float_t fRapidity;
    Float_t fTransverseMomentum;
    Char_t cCharge;
    Char_t cIsCut;
    Short_t sIdxP1; 
    Short_t sIdxP2;
    Float_t fProbability1;
    Float_t fProbability2;
    Int_t   nPid1;
    Int_t   nPid2;

    Short_t sIdxPidCand1;
    Float_t fSys1;
    Float_t fR1;
    Float_t fZ1;
    Float_t fMassExp1;   
    Float_t fBetaExp1;
    Int_t   nMomAlg1;
    Float_t fMom1;
    Int_t   nChrg1;
    Int_t   nMostProbPid1;
    Float_t fWeightMostProbPID1;

    Float_t fThetaDeg1; 
    Float_t fPhiDeg1;
    Int_t   nSec1;

    Short_t sIdxPidCand2;
    Float_t fSys2;
    Float_t fR2;
    Float_t fZ2;
    Float_t fMassExp2;   
    Float_t fBetaExp2;
    Int_t   nMomAlg2;
    Float_t fMom2;
    Int_t   nChrg2;
    Int_t   nMostProbPid2;
    Float_t fWeightMostProbPID2;
    
    Float_t fThetaDeg2; 
    Float_t fPhiDeg2;
    Int_t   nSec2;

    Float_t fDeltaThetaRM1;
    Float_t fDeltaPhiRM1;   
    Float_t fDeltaThetaRM2; 
    Float_t fDeltaPhiRM2;   
    Float_t fTof1;          
    Float_t fTof2;          

    Int_t nRingPadNr1;    
    Float_t fRingCentroid1; 
    Float_t fRichTheta1;    
    Float_t fRichPhi1;      
    Int_t nRingPatMat1;   
    Int_t nRingHouTra1;   
    Int_t nRingAmplitude1;
    Int_t nRingLocalMax41;

    Int_t nRingPadNr2;    
    Float_t fRingCentroid2; 
    Float_t fRichTheta2;    
    Float_t fRichPhi2;      
    Int_t nRingPatMat2;   
    Int_t nRingHouTra2;   
    Int_t nRingAmplitude2;
    Int_t nRingLocalMax42;

    
    //Variables for advanced filtering
    Float_t qSpline1,qSpline2;
    Float_t fInnerChiSquare1, fOuterChiSquare1, fDistanceToVertex1;
    Float_t fInnerChiSquare2, fOuterChiSquare2, fDistanceToVertex2; 
    Bool_t bIsCPCandidate1, bIsCPCandidate2;
    Float_t fAngleWithCPCandidate1, fAngleWithCPCandidate2;

    Float_t IOm_chi2_1, IOm_chi2_2;

    Short_t sIndRICH1;
    Short_t sIndInnerSeg1;
    Short_t sIndOuterSeg1;
    Short_t sIndTOF1;
    Short_t sIndShower1;

    Short_t sIndRICH2;
    Short_t sIndInnerSeg2;
    Short_t sIndOuterSeg2;
    Short_t sIndTOF2;
    Short_t sIndShower2;

    /*
    HPidTrackData* pTrackData1;
    HPidTrackData* pTrackData2;
    HPidHitData* pHitData1;
    HPidHitData* pHitData2;
    HPidParticle* pParticle1;
    HPidParticle* pParticle2;
    */


    static const TString cVars;
    static const Int_t DATAITEMS;

// ---------------------------------------------------------------------------
    ClassDef(HPairData,1)
};
#endif // HPAIRDATA_H
