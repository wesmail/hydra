// $Id: hpairdata.h,v 1.19 2007-12-13 10:44:11 kempter Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2006-11-21 16:05:36
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
// ---------------------------------------------------------------------------
#define storeFWinfo  // to analyze APR07 switch this on!
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

    TVector3 getVert(void) const {return aVert;};
    Float_t getVertX(void) const {return aVert.X();};
    Float_t getVertY(void) const {return aVert.Y();};
    Float_t getVertZ(void) const {return aVert.Z();};
    Float_t getDistX(void) const {return aDist.X();};
    Float_t getDistY(void) const {return aDist.Y();};
    Float_t getDistZ(void) const {return aDist.Z();};
    Float_t pDist(void) const {return aDist.Mag();};

    Float_t getThetaDeg1(void) const {return  fThetaDeg1;}
    Float_t getThetaDeg2(void)  const {return  fThetaDeg2;}
    Float_t getPhiDeg1(void)  const {return  fPhiDeg1;}
    Float_t getPhiDeg2(void)  const {return  fPhiDeg2;}
    Float_t getMom1(void)  const {return  fMom1;}
    Float_t getMom2(void)  const {return  fMom2;}


    Float_t getAngleToClosestFittedLepton1(void)  const {return fAngleToClosestFittedLepton1;}
    Float_t getAngleToClosestFittedLepton2(void)  const {return fAngleToClosestFittedLepton2;}
    Float_t getAngleToClosestNonFittedLepton1(void)  const {return fAngleToClosestNonFittedLepton1;}
    Float_t getAngleToClosestNonFittedLepton2(void)  const {return fAngleToClosestNonFittedLepton2;}

    Float_t getAngleToClosestFittedHadron1(void)  const {return fAngleToClosestFittedHadron1;}
    Float_t getAngleToClosestFittedHadron2(void)  const {return fAngleToClosestFittedHadron2;}
    Float_t getAngleToClosestNonFittedHadron1(void)  const {return fAngleToClosestNonFittedHadron1;}
    Float_t getAngleToClosestNonFittedHadron2(void)  const {return fAngleToClosestNonFittedHadron2;}


#if 0
    Bool_t getIsCPCandidate1(void) {return bIsCPCandidate1;}
    Bool_t getIsCPCandidate2(void) {return bIsCPCandidate2;}
#endif // 0

#if 0
    Bool_t getClosestLepIsFitted1(void) {return bClosestLepIsFitted1;}
    Bool_t getClosestHadIsFitted1(void) {return bClosestHadIsFitted1;}

    Bool_t getClosestLepIsFitted2(void) {return bClosestLepIsFitted2;}
    Bool_t getClosestHadIsFitted2(void) {return bClosestHadIsFitted2;}

    Float_t getAngleWithClosestLep1(void) {return fAngleToClosestLep1;}
    Float_t getAngleWithClosestHad1(void) {return fAngleToClosestHad1;}

    Float_t getAngleWithClosestLep2(void) {return fAngleToClosestLep2;}
    Float_t getAngleWithClosestHad2(void) {return fAngleToClosestHad2;}
#endif //0

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

    Float_t fAngleToClosestFittedLepton1, fAngleToClosestFittedLepton2;
    Float_t fAngleToClosestNonFittedLepton1, fAngleToClosestNonFittedLepton2;
    Float_t fAngleToClosestFittedHadron1, fAngleToClosestFittedHadron2;
    Float_t fAngleToClosestNonFittedHadron1, fAngleToClosestNonFittedHadron2;
#if 0
    Bool_t bClosestHadIsFitted1, bClosestHadIsFitted2;
    Bool_t bClosestLepIsFitted1, bClosestLepIsFitted2;
#endif // 0
    Char_t cCharge;
    Char_t cIsCut;
#if 0
    Float_t fAngleToClosestHad1, fAngleToClosestHad2;
    Float_t fAngleToClosestLep1, fAngleToClosestLep2;
#endif // 0
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
    Int_t nTrigTBit;
    Float_t qSpline1,qSpline2;
    Float_t rkchi1, rkchi2;
    Float_t fMetaEloss1,fMetaEloss2;
    Float_t metamatchqa1,metamatchqa2;

#ifdef  storeFWinfo
//if FW
    Int_t   fwMult;
    Float_t fwTime1, fwTime2, fwTime3;
    Float_t fwCharge1, fwCharge2, fwCharge3;
    Int_t   fwCell1, fwCell2, fwCell3;
    Float_t fwTheta1, fwTheta2, fwTheta3;
    Float_t fwPhi1, fwPhi2, fwPhi3;
    Float_t fwDist1, fwDist2, fwDist3;
    Short_t tofrecflag;    
//endif FW
#endif

    Float_t fInnerMdcdEdx1,fInnerMdcdEdx2;
    Float_t fInnerMdcdEdxSigma1,fInnerMdcdEdxSigma2;
    Float_t fOuterMdcdEdx1,fOuterMdcdEdx2;
    Float_t fOuterMdcdEdxSigma1,fOuterMdcdEdxSigma2;
    Float_t fCombinedMdcdEdx1,fCombinedMdcdEdx2;
    Float_t fCombinedMdcdEdxSigma1,fCombinedMdcdEdxSigma2;
    Float_t fShower_Sum0_1, fShower_Sum0_2;
    Float_t dxRkMeta1, dyRkMeta1, dzRkMeta1, dxRkMeta2, dyRkMeta2, dzRkMeta2;
    Float_t dxMdcMeta1, dyMdcMeta1, dxMdcMeta2, dyMdcMeta2;
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

    Int_t flags1;
    Int_t flags2;


    static const TString cVars;
    static const Int_t DATAITEMS;
    
    Bool_t kIsInit;//!

#ifdef  storeFWinfo
private:
    HIterator *iterFwHit;         //! HWallHit iterator (used for APR07)
#endif

// ---------------------------------------------------------------------------
    ClassDef(HPairData,4)
	};
#endif // HPAIRDATA_H
