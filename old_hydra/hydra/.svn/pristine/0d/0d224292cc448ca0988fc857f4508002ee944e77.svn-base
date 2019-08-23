// $Id: hpairdata.h,v 1.12 2006-08-16 15:42:18 halo Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2006-06-28 13:43:39
//
// ---------------------------------------------------------------------------
#ifndef HPAIRDATA_H
#define HPAIRDATA_H
// ---------------------------------------------------------------------------
#include <TObject.h>
#include <TString.h>
// ---------------------------------------------------------------------------
class HPair;
class HPairExt;
class HPairSimExt;
class HPidParticle;
class HEventHeader;
// ---------------------------------------------------------------------------

class HPairData : public TObject {


public:

    HPairData();
    HPairData(HPair*);
    HPairData(HPairExt*);
    HPairData(HPairSimExt*);
    virtual ~HPairData();
    virtual void print(void);
    Float_t* getArray(void) const;
    static const TString& getVarNames(void);
    Bool_t getInit(){return kIsInit;};
    void   setInit(void){kIsInit = kTRUE;};
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

    Float_t getVertX(void) const {return aVert.X();};
    Float_t getVertY(void) const {return aVert.Y();};
    Float_t getVertZ(void) const {return aVert.Z();};
    Float_t getDistX(void) const {return aDist.X();};
    Float_t getDistY(void) const {return aDist.Y();};
    Float_t getDistZ(void) const {return aDist.Z();};
    Float_t pDist(void) const {return aDist.Mag();};

    Float_t getThetaDeg1(void) {return  fThetaDeg1;}
    Float_t getThetaDeg2(void) {return  fThetaDeg2;}
    Float_t getPhiDeg1(void) {return  fPhiDeg1;}
    Float_t getPhiDeg2(void) {return  fPhiDeg2;}
    Float_t getMom1(void) {return  fMom1;}
    Float_t getMom2(void) {return  fMom2;}

    //Bool_t getIsCPCandidate1(void) {return bIsCPCandidate1;}
    //Bool_t getIsCPCandidate2(void) {return bIsCPCandidate2;}

    Bool_t getClosestLepIsFitted1(void) {return bClosestLepIsFitted1;}
    Bool_t getClosestHadIsFitted1(void) {return bClosestHadIsFitted1;}

    Bool_t getClosestLepIsFitted2(void) {return bClosestLepIsFitted2;}
    Bool_t getClosestHadIsFitted2(void) {return bClosestHadIsFitted2;}

    Float_t getAngleWithClosestLep1(void) {return fAngleToClosestLep1;}
    Float_t getAngleWithClosestHad1(void) {return fAngleToClosestHad1;}

    Float_t getAngleWithClosestLep2(void) {return fAngleToClosestLep2;}
    Float_t getAngleWithClosestHad2(void) {return fAngleToClosestHad2;}
    Int_t    hasDoubleDetectorHits(void) const;
    Int_t    getBitFieldPassedCuts(void) const {return nBitFieldPassedCuts;};
    static const Int_t getNbDataItems(){return DATAITEMS;};


protected:

    virtual void reset(void);
    virtual Bool_t init(HPair*);
    virtual Bool_t init(HPairExt*);
    virtual Bool_t init(HPairSimExt*);
    virtual Bool_t initData(HPair*,HPidParticle*,HPidParticle*,HEventHeader*);
    
    // event header info
    Int_t   nChrg1;
    Int_t   nChrg2;
    Int_t   nMomAlg1;
    Int_t   nMomAlg2;
    Int_t   nMostProbPid1;
    Int_t   nMostProbPid2;
    Int_t   nPid1;
    Int_t   nPid2;
    Int_t   nSec1;
    Int_t   nSec2;
    Float_t IOm_chi2_1, IOm_chi2_2;
    TVector3 aDist;
    TVector3 aVert;
    Float_t fEvtVertexX, fEvtVertexY, fEvtVertexZ;
    Bool_t bClosestHadIsFitted1, bClosestHadIsFitted2;
    Bool_t bClosestLepIsFitted1, bClosestLepIsFitted2;
    Char_t cCharge;
    Char_t cIsCut;
    Float_t fAngleToClosestHad1, fAngleToClosestHad2;
    Float_t fAngleToClosestLep1, fAngleToClosestLep2;
    Float_t fBetaExp1;
    Float_t fBetaExp2;
    Float_t fDeltaPhiRM1;   
    Float_t fDeltaPhiRM2;   
    Float_t fDeltaThetaRM1;
    Float_t fDeltaThetaRM2; 
    Float_t fInnerChiSquare1, fOuterChiSquare1, fDistanceToVertex1;
    Float_t fInnerChiSquare2, fOuterChiSquare2, fDistanceToVertex2; 
    Float_t fMass;
    Float_t fMassExp1;   
    Float_t fMassExp2;   
    Float_t fMom1;
    Float_t fMom2;
    Float_t fOpang;
    Float_t fPhiDeg1;
    Float_t fPhiDeg2;
    Float_t fProbability1;
    Float_t fProbability2;
    Float_t fR1;
    Float_t fR2;
    Float_t fRapidity;
    Float_t fRichPhi1;      
    Float_t fRichPhi2;      
    Float_t fRichTheta1;    
    Float_t fRichTheta2;    
    Float_t fRingCentroid1; 
    Float_t fRingCentroid2; 
    Float_t fSys1;
    Float_t fSys2;
    Float_t fThetaDeg1; 
    Float_t fThetaDeg2; 
    Float_t fTof1;          
    Float_t fTof2;          
    Float_t fTransverseMomentum;
    Float_t fWeightMostProbPID1;
    Float_t fWeightMostProbPID2;
    Float_t fZ1;
    Float_t fZ2;
    Int_t nBitFieldPassedCuts;
    Int_t nDSflag;
    Int_t nEvtNr;
    Int_t nRingAmplitude1;
    Int_t nRingAmplitude2;
    Int_t nRingHouTra1;   
    Int_t nRingHouTra2;   
    Int_t nRingLocalMax41;
    Int_t nRingLocalMax42;
    Int_t nRingPadNr1;    
    Int_t nRingPadNr2;    
    Int_t nRingPatMat1;   
    Int_t nRingPatMat2;   
    Int_t nRun;
    Int_t nTrigDec;
    Float_t qSpline1,qSpline2;
    Short_t sIdxP1; 
    Short_t sIdxP2;
    Short_t sIdxPidCand1;
    Short_t sIdxPidCand2;
    Short_t sIndInnerSeg1;
    Short_t sIndInnerSeg2;
    Short_t sIndOuterSeg1;
    Short_t sIndOuterSeg2;
    Short_t sIndRICH1;
    Short_t sIndRICH2;
    Short_t sIndShower1;
    Short_t sIndShower2;
    Short_t sIndTOF1;
    Short_t sIndTOF2;


    static const TString cVars;
    static const Int_t DATAITEMS;
    
    Bool_t kIsInit;//!

// ---------------------------------------------------------------------------
    ClassDef(HPairData,1)
	};
#endif // HPAIRDATA_H
