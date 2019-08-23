// $Id: hpairdata.cc,v 1.19 2006-12-18 09:10:28 eberl Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2006-12-18 10:07:45
//
// ---------------------------------------------------------------------------
//_HADES_CLASS_DESCRIPTION
///////////////////////////////////////////////////////////////////////////////
//                                                                           
// HPairData                                                                   
//                                                                           
///////////////////////////////////////////////////////////////////////////////
//
using namespace std;
// ---------------------------------------------------------------------------
#include <iostream>
// ---------------------------------------------------------------------------
#include <TMath.h>
// ---------------------------------------------------------------------------
#include <hades.h>
#include <hcategory.h>
#include <hevent.h>
#include <heventheader.h>
#include <hpidhitdata.h>
#include <hpidparticle.h>
#include <hpidtrackdata.h>
// ---------------------------------------------------------------------------
#include "hpair.h"
#include "hpairext.h"
#include "hpairsimext.h"
#include "hpairfl.h"
#include "piddef.h"
// ---------------------------------------------------------------------------
#include "hpairdata.h"

// ---------------------------------------------------------------------------
ClassImp(HPairData)
// ---------------------------------------------------------------------------
const TString HPairData::cVars=
"invmass:opang:rap:pt:charge:isCutNb:idxPart1:idxPart2:prob1:prob2:pid1:pid2"
":idxpidcand1:sys1:r1:z1:massexp1:betaexp1:momalgidx1:chrg1:mostprobpid1"
":weightmostprobpid1:theta1:phi1:sec1:idxpidcand2:sys2:r2:z2:massexp2"
":betaexp2:momalgidx2:chrg2:mostprobpid2:weightmostprobpid2:theta2:phi2"
":sec2:drmt1:drmp1:drmt2:drmp2:tof1:tof2:rpadnr1:rcentroid1:rt1:rp1:rpatmat1"
":rhoutra1:rampl1:rlocmax41:rpadnr2:rcentroid2:rt2:rp2:rpatmat2:rhoutra2"
":rampl2:rlocmax42:mom1:mom2:doubleHit:qspline1:qspline2:rkchi1:rkchi2"
":innerchisquare1:innerchisquare2:outerchisquare1:outerchisquare2:tofeloss1"
":tofeloss2:innermdcdedx1:innermdcdedx2:innermdcdedxsigma1:innermdcdedxsigma2"
":outermdcdedx1:outermdcdedx2:outermdcdedxsigma1:outermdcdedxsigma2"
":shower_sum0_1:shower_sum0_2:distancetovertex1:distancetovertex2"
":angletoclosestfittedlep1:angletoclosestnonfittedlep1"
":angletoclosestfittedhad1:angletoclosestnonfittedhad1"
":angletoclosestfittedlep2:angletoclosestnonfittedlep2"
":angletoclosestfittedhad2:angletoclosestnonfittedhad2"
":IOm_chi2_1:IOm_chi2_2:pairvertx:pairverty"
":pairvertz:pairdistx:pairdisty:pairdistz:pairdist:evtNr:DSflag:trigDec"
":evtVertX:evtVertY:evtVertZ:run";

const Int_t HPairData::DATAITEMS = 109;
// ---------------------------------------------------------------------------
HPairData::HPairData() 
{
    reset();
}

// ---------------------------------------------------------------------------
HPairData::HPairData(HPair* p) 
{
    if (init(p)) setInit();
    else ::Error("ctor HPair","data not init");
}
// ---------------------------------------------------------------------------
HPairData::HPairData(HPairExt* p) 
{
    if (init(p)) setInit();
    else ::Error("ctor HPairExt","data not init");

}
// ---------------------------------------------------------------------------
HPairData::HPairData(HPairSimExt* p) 
{
    if (init(p)) setInit();
    else ::Error("ctor HPairSimExt","data not init");
}
// ---------------------------------------------------------------------------
HPairData::~HPairData() 
{

}

// ---------------------------------------------------------------------------
void HPairData::print()
{
    cout<<"==============================================================="<<endl;
    cout<<" opang:"<< 
	fOpang<<" chrg:"<<
	(Int_t)cCharge<<" isCut:"<<
	(Int_t)cIsCut<<" mass:"<<
	fMass<<" rap:"<<
	fRapidity<<" pt:"<<
	fTransverseMomentum<<endl;
    cout<<"-----------------------------------"<<endl;
    
    cout<<"idxP1:"<<
	sIdxP1 <<" idxP2:"<<
	sIdxP2<<" prob1:"<<
	fProbability1<<" prob2:"<<
	fProbability2<<" pid1:"<<
	nPid1<<" pid2:"<<
	nPid2<<endl;
    cout<<"-----------------------------------"<<endl;
    cout<<"aVert: (X):"<< aVert.X()<<", (Y): "<<aVert.Y()
	<<", (Z): "<<aVert.Z()<<endl;
    cout<<"aDist: (X):"<< aDist.X()<<", (Y): "<<aDist.Y()
	<<", (Z): "<<aDist.Z()<<", (R): "<<aDist.Mag()<<endl;
    cout<<"-----------------------------------"<<endl;
    cout<<"idxPidCand1:"<<sIdxPidCand1<<" sys1:"<<
	fSys1<<" r1:"<<
	fR1<<" z1:"<<
	fZ1<<" massE1:"<<
	fMassExp1 <<" betaE1:"<<  
	fBetaExp1<<" momalg1:"<<
	nMomAlg1<<" mom1:"<<
	fMom1<<" chrg1:"<<
	nChrg1<<" mbpid1:"<<
	nMostProbPid1<<" weight1:"<<
	fWeightMostProbPID1<<" theta1:"<<
	fThetaDeg1 <<" phi1:"<<
	fPhiDeg1<<" sec1:"<<
	nSec1<<endl;
    cout<<"-----------------------------------"<<endl;
    cout<<"idxPidCand2:"<<sIdxPidCand2<<" sys2:"<<
	fSys2<<" r2:"<<
	fR2<<" z2:"<<
	fZ2<<" massE2"<<
	fMassExp2 <<" betaE2"<<  
	fBetaExp2<<" momalg2:"<<
	nMomAlg2<<" mom2:"<<
	fMom2<<" chrg2:"<<
	nChrg2<<" mbpid2:"<<
	nMostProbPid2<<" weight2:"<<
	fWeightMostProbPID2<<" theta2:"<<
	fThetaDeg2 <<" phi2:"<<
	fPhiDeg2<<" sec2:"<<
	nSec2<<
	endl;
}
// ---------------------------------------------------------------------------
Bool_t HPairData::init(HPair* pPair)
{
    // get pointers to objects with relevant information

    // assumes that init is called while analysis is still
    // in event from which pair originates.

    if (!pPair)
    {
	::Error("HPairData::init(HPair*)",
		"pointer to pair is NULL");
	return kFALSE;
    }

    HCategory* pCatPart = HPairFL::getCategory(catPidPart);
    if (!pCatPart) 
    {
	::Error("init","pointer to HPidParticle category is NULL");
	return kFALSE;
    }

    HPidParticle* pParticle1    = (HPidParticle*)
	(pCatPart->getObject( pPair->getIndexParticle1() ) );

    if (!pParticle1) 
    {
	::Error("init","pointer to particle 1 is NULL");
	return kFALSE;
    }

    HPidParticle* pParticle2    = (HPidParticle*)
	(pCatPart->getObject( pPair->getIndexParticle2() ) );
    if (!pParticle2) 
    {
	::Error("init","pointer to particle 2 is NULL");
	return kFALSE;
    }

    // store event header info for this pair
    HEvent* pEvt = gHades->getCurrentEvent();
    if (!pEvt)
    {
	::Error("HPairData::init(HPair*)","current event not found");
	return kFALSE;
    }
    HEventHeader* pEvtHeader = (HEventHeader*) pEvt->getHeader();
    if (!pEvtHeader)
    {
	::Error("HPairData::init(HPair*)","current event header not found");
	return kFALSE;
    }

    // now init the data members for this pair
    return initData(pPair,pParticle1,pParticle2,pEvtHeader);
}
// ---------------------------------------------------------------------------
Bool_t HPairData::init(HPairSimExt* pPair)
{
    // get pointers to objects with relevant information

    // assumes that init is called while analysis is still
    // in event from which pair originates.
    if (!pPair)
    {
	::Error("HPairData::init(HPairSimExt*)",
		"pointer to pair is NULL");
	return kFALSE;
    }


    HPidParticle* pParticle1 = (HPidParticle*) pPair->getPidParticle1();
    HPidParticle* pParticle2 = (HPidParticle*) pPair->getPidParticle2();
    // now init the data members for this pair
    return initData((HPair*)pPair,pParticle1,pParticle2,NULL);
}
// ---------------------------------------------------------------------------
Bool_t HPairData::init(HPairExt* pPair)
{
    // get pointers to objects with relevant information

    // assumes that init is called while analysis is still
    // in event from which pair originates.

    
    if (!pPair)
    {
	::Error("HPairData::init(HPairExt*)",
		"pointer to pair is NULL");
	return kFALSE;
    }

    HPidParticle* pParticle1 = pPair->getPidParticle1();
    HPidParticle* pParticle2 = pPair->getPidParticle2();
    // now init the data members for this pair
    return initData(pPair,pParticle1,pParticle2,NULL);
}
// ---------------------------------------------------------------------------
Bool_t HPairData::initData(HPair* p, 
			   HPidParticle* p1, HPidParticle* p2,
			   HEventHeader* pEvtHeader)
{
    reset();
    

    // in case of across-event-boundary analyses,
    // the event header of the current event is  - possibly -
    // not in sync with the data objects. Therefore, 
    // do not fill it, but leave default values!
    if (pEvtHeader)
    {
	nEvtNr    = (Int_t) pEvtHeader->getEventSeqNumber();
	nDSflag   = (Int_t) pEvtHeader->getDownscalingFlag();
	nTrigDec  = (Int_t) pEvtHeader->getTriggerDecision();
	nRun      = (Int_t) pEvtHeader->getEventRunNumber();
	
	HVertex& vertex = pEvtHeader->getVertex();
	fEvtVertexX = (Float_t)  vertex.getX();
	fEvtVertexY = (Float_t)  vertex.getY();
	fEvtVertexZ = (Float_t)  vertex.getZ();
    }
    
    //      cout << "evt nr: " << (Int_t)nEvtNr << "  DSflag: " << (Int_t)nDSflag
    //  	 << "  nTrigDec: " << (Int_t)nTrigDec << endl;
    

    // -------------------------------------


    if (!p)
    {
	::Error("HPairData::initData()",
		"pair not found");
	return kFALSE;
    }

    nBitFieldPassedCuts   = p->getBitFieldPassedCuts();
    cCharge               = p->getCharge();
    fOpang                = p->getOpeningAngleDeg();
    fMass                 = p->M();
    fRapidity             = p->Rapidity();
    fTransverseMomentum   = p->Pt();
    cIsCut                = p->getIsCutFlag();
    
    sIdxP1                = p->getIndexParticle1();
    fProbability1         = p->getPidProbability1();
    nPid1                 = p->getPid1();
    
    sIdxP2                = p->getIndexParticle2();
    fProbability2         = p->getPidProbability2();
    nPid2                 = p->getPid2();
    
    
    
    if (!p1)
    {
	::Error("HPairData::initData()",
		"particle 1 not found");
	return kFALSE;
    }
    if (!p2)
    {
	::Error("HPairData::initData()",
		"particle 2 not found");
	return kFALSE;
    }
    
    HPairFL::calcVertex(p1, p2, &aVert, &aDist);
    
    
    sIdxPidCand1          = p1->getPidCandidateIndex();
    fSys1                 = p1->getSystem();
    fR1                   = p1->getR();
    fZ1                   = p1->getZ();
    nMomAlg1              = p1->getMomAlg();
    fMom1                 = p1->P();
    nChrg1                = p1->getCharge();
    nMostProbPid1         = p1->getMostProbablePID();
    fWeightMostProbPID1   = p1->getWeightForPID((Short_t)nMostProbPid1);
    
    fThetaDeg1            = p1->thetaDeg();
    fPhiDeg1              = p1->phiDeg();
    nSec1                 = p1->sector();
    
	
    const HPidHitData* pHitData1     =  p1 -> getHitData();
    if (!pHitData1) 
    {
	Error("init","pointer to hit data of particle 1 is NULL");
	return kFALSE;
    }
    const HPidTrackData* pTrackData1 =  p1 -> getTrackData(); 
    if (!pTrackData1) 
    {
	Error("init","pointer to track data of particle 1 is NULL");
	return kFALSE;
    }
    
    fDeltaThetaRM1 = pHitData1->getDeltaThetaMDCRICH();
    fDeltaPhiRM1   = pHitData1->getDeltaPhiMDCRICH();

    fTof1          = pTrackData1->getTofRecTof(nMomAlg1);
    fTofEloss1     = pHitData1->getTofEloss();

    fMassExp1             = TMath::Sqrt(pTrackData1->getMass2(nMomAlg1));
    fBetaExp1             = pTrackData1->getBeta(nMomAlg1);

    nRingPadNr1     = pHitData1->getRingPadNr();
    fRingCentroid1  = pHitData1->getRingCentroid();
    fRichTheta1     = pHitData1->getRichTheta();
    fRichPhi1       = pHitData1->getRichPhi();
    nRingPatMat1    = pHitData1->getRingPatMat();
    nRingHouTra1    = pHitData1->getRingHouTra();
    nRingAmplitude1 = pHitData1->getRingAmplitude();
    nRingLocalMax41 = pHitData1->getRingLocalMax4();
    qSpline1         = pTrackData1->getSplineChiSquare();
    rkchi1           = pTrackData1->fRKChiSquare;
    fInnerChiSquare1 = pHitData1->getInnerMdcChiSquare();
    fOuterChiSquare1 = pHitData1->getOuterMdcChiSquare();
    
    fInnerMdcdEdx1  = pHitData1->getInnerMdcdEdx();
    fInnerMdcdEdxSigma1 = pHitData1->getInnerMdcdEdxSigma();
    fOuterMdcdEdx1 = pHitData1->getOuterMdcdEdx();
    fOuterMdcdEdxSigma1 = pHitData1->getOuterMdcdEdxSigma();
  
    fShower_Sum0_1 = pHitData1->fShowerSum[0];

    fAngleToClosestFittedLepton1    = pTrackData1->getAngleWithClosestCandidate( 1, 1);
    fAngleToClosestNonFittedLepton1 = pTrackData1->getAngleWithClosestCandidate( 1,-1);
  
    fAngleToClosestFittedHadron1    = pTrackData1->getAngleWithClosestCandidate(-1, 1);
    fAngleToClosestNonFittedHadron1 = pTrackData1->getAngleWithClosestCandidate(-1,-1);
  
#if 0
    bClosestLepIsFitted1 = pTrackData1->getClosestLeptonCandidateIsFitted();
    bClosestHadIsFitted1 = pTrackData1->getClosestHadronCandidateIsFitted();

    fAngleToClosestLep1=pTrackData1->getAngleWithClosestLeptonCandidate();
    fAngleToClosestHad1=pTrackData1->getAngleWithClosestHadronCandidate();
#endif // 0

    fDistanceToVertex1      = pHitData1->getDistanceToVertex(p1->getMomAlg());
    IOm_chi2_1 = pTrackData1->getIOMatching(p1->getMomAlg());

    sIndRICH1     = pHitData1->getIndRICH();
    sIndInnerSeg1 = pHitData1->getIndInnerSeg();
    sIndOuterSeg1 = pHitData1->getIndOuterSeg();
    sIndTOF1      = pHitData1->getIndTOF();
    sIndShower1   = pHitData1->getIndShower();
    
    
    // ----------------------------------------------------------------------
    sIdxPidCand2          = p2->getPidCandidateIndex();
    fSys2                 = p2->getSystem();
    fR2                   = p2->getR();
    fZ2                   = p2->getZ();
    nMomAlg2              = p2->getMomAlg();
    fMom2                 = p2->P();
    nChrg2                = p2->getCharge();
    nMostProbPid2         = p2->getMostProbablePID();
    fWeightMostProbPID2   = p2->getWeightForPID((Short_t)nMostProbPid2);
    
    fThetaDeg2            = p2->thetaDeg();
    fPhiDeg2              = p2->phiDeg();
    nSec2                 = p2->sector();


    const HPidHitData* pHitData2     =  p2 -> getHitData();
    if (!pHitData2) 
    {
	Error("init","pointer to hit data of particle 2 is NULL");
	return kFALSE;
    }
    const HPidTrackData* pTrackData2 =  p2 -> getTrackData(); 
    if (!pTrackData2) 
    {
	Error("init","pointer to track data of particle 2 is NULL");
	return kFALSE;
    }


    fDeltaThetaRM2 = pHitData2->getDeltaThetaMDCRICH();
    fDeltaPhiRM2   = pHitData2->getDeltaPhiMDCRICH();
    nRingPadNr2     = pHitData2->getRingPadNr();
    fRingCentroid2  = pHitData2->getRingCentroid();
    fRichTheta2     = pHitData2->getRichTheta();
    fRichPhi2       = pHitData2->getRichPhi();
    nRingPatMat2    = pHitData2->getRingPatMat();
    nRingHouTra2    = pHitData2->getRingHouTra();
    nRingAmplitude2 = pHitData2->getRingAmplitude();
    nRingLocalMax42 = pHitData2->getRingLocalMax4();
    
    fTof2          = pTrackData2->getTofRecTof(nMomAlg2);
    fTofEloss2     = pHitData2->getTofEloss();
    
    fMassExp2             = TMath::Sqrt(pTrackData2->getMass2(nMomAlg2));
    fBetaExp2             = pTrackData2->getBeta(nMomAlg2);
    
    qSpline2         = pTrackData2->getSplineChiSquare();
    rkchi2           = pTrackData2->fRKChiSquare;
    fInnerChiSquare2 = pHitData2->getInnerMdcChiSquare();
    fOuterChiSquare2 = pHitData2->getOuterMdcChiSquare();
    
    fInnerMdcdEdx2        = pHitData2->getInnerMdcdEdx();
    fInnerMdcdEdxSigma2   = pHitData2->getInnerMdcdEdxSigma();
    fOuterMdcdEdx2        = pHitData2->getOuterMdcdEdx();
    fOuterMdcdEdxSigma2   = pHitData2->getOuterMdcdEdxSigma();

    fShower_Sum0_2 = pHitData2->fShowerSum[0];

     fAngleToClosestFittedLepton2    = pTrackData2->getAngleWithClosestCandidate( 1, 1);    
     fAngleToClosestNonFittedLepton2 = pTrackData2->getAngleWithClosestCandidate( 1,-1); 
  
     fAngleToClosestFittedHadron2    = pTrackData2->getAngleWithClosestCandidate(-1, 1);      
     fAngleToClosestNonFittedHadron2 = pTrackData2->getAngleWithClosestCandidate(-1,-1);    

#if 0  
    bClosestLepIsFitted2 = pTrackData2->getClosestLeptonCandidateIsFitted();
    bClosestHadIsFitted2 = pTrackData2->getClosestHadronCandidateIsFitted();
    
    fAngleToClosestLep2=pTrackData2->getAngleWithClosestLeptonCandidate();
    fAngleToClosestHad2=pTrackData2->getAngleWithClosestHadronCandidate();
#endif // 0    
    
    fDistanceToVertex2      = pHitData2->getDistanceToVertex(p2->getMomAlg());
    
    IOm_chi2_2 = pTrackData2->getIOMatching(p2->getMomAlg());

    sIndRICH2     = pHitData2->getIndRICH();
    sIndInnerSeg2 = pHitData2->getIndInnerSeg();
    sIndOuterSeg2 = pHitData2->getIndOuterSeg();
    sIndTOF2      = pHitData2->getIndTOF();
    sIndShower2   = pHitData2->getIndShower();
    
    return kTRUE;
}
// ---------------------------------------------------------------------------
void HPairData::reset()
{
    kIsInit = kFALSE;
    nRun = -1;
    IOm_chi2_1 = -100.;
    IOm_chi2_2 = -100.;
    aDist.SetXYZ(-1000.,-1000.,-1000.);
    aVert.SetXYZ(-1000.,-1000.,-1000.);
#if 0
    bClosestHadIsFitted1 = kFALSE;
    bClosestHadIsFitted2 = kFALSE;
    bClosestLepIsFitted1 = kFALSE;
    bClosestLepIsFitted2 = kFALSE;
#endif //0
    cCharge = -10;
    cCharge=-10;
    cIsCut = -1;
    cIsCut=-1;
#if 0
    fAngleToClosestHad1 = -1.;
    fAngleToClosestHad2 = -1.;
    fAngleToClosestLep1 = -1.;
    fAngleToClosestLep2 = -1.;
#endif //0

    fAngleToClosestFittedLepton1 = -1.;
    fAngleToClosestFittedLepton2 = -1.;
    fAngleToClosestNonFittedLepton1 = -1.;
    fAngleToClosestNonFittedLepton2 = -1.;
    fAngleToClosestFittedHadron1 = -1.;
    fAngleToClosestFittedHadron2 = -1.;
    fAngleToClosestNonFittedHadron1 = -1.;
    fAngleToClosestNonFittedHadron2 = -1.;

    fBetaExp1 = -1.;
    fBetaExp2 = -1.;
    fDeltaPhiRM1 = -1000.;   
    fDeltaPhiRM2 = -1000.;   
    fDeltaThetaRM1 = -1000.;
    fDeltaThetaRM2 = -1000.; 
    fDistanceToVertex1 = -1.; 
    fDistanceToVertex2 = -1.; 
    fInnerChiSquare1 = -1.;
    fInnerChiSquare2 = -1.;
    fMass = -1.;
    fMass=-1.;
    fMassExp1 = -1.;   
    fMassExp2 = -1.;   
    fMom1 = -1.;
    fMom2 = -1.;
    fOpang = -1.;
    fOpang=-1.;
    fOuterChiSquare1 = -1.;
    fOuterChiSquare2 = -1.;
    fPhiDeg1 = -1.;
    fPhiDeg2 = -1.;
    fProbability1 = -1.;
    fProbability2 = -1.;
    fR1 = -10000.;
    fR2 = -10000.;
    fRapidity = -1.;
    fRapidity=-1.;
    fRichPhi1 = -1.;      
    fRichPhi2 = -1.;      
    fRichTheta1 = -1.;    
    fRichTheta2 = -1.;    
    fRingCentroid1 = -1.; 
    fRingCentroid2 = -1.; 
    fSys1 = -1.;
    fSys2 = -1.;
    fThetaDeg1 = -1.; 
    fThetaDeg2 = -1.; 
    fTof1 = -1.;          
    fTof2 = -1.;          
    fTransverseMomentum = -1.;
    fTransverseMomentum=-1.;
    fWeightMostProbPID1 = -1.;
    fWeightMostProbPID2 = -1.;
    fZ1 = -10000.;
    fZ2 = -10000.;
    nBitFieldPassedCuts=0;
    nChrg1 = -10;
    nChrg2 = -10;
    nDSflag= -1;
    nEvtNr = -1;
    nMomAlg1 = -1;
    nMomAlg2 = -1;
    nMostProbPid1 = -1;
    nMostProbPid2 = -1;
    nPid1 = -1;
    nPid2 = -1;
    nRingAmplitude1 = -1;
    nRingAmplitude2 = -1;
    nRingHouTra1 = -1;   
    nRingHouTra2 = -1;   
    nRingLocalMax41 = -1;
    nRingLocalMax42 = -1;
    nRingPadNr1 = -1;    
    nRingPadNr2 = -1;    
    nRingPatMat1 = -1;   
    nRingPatMat2 = -1;   
    nSec1 = -1;
    nSec2 = -1;
    nTrigDec=-1;
    qSpline1 = -1.;
    qSpline2 = -1.;
    rkchi1 = -1;
    rkchi2 = -1;
    fTofEloss1 = -1;
    fTofEloss2 = -1;
    fInnerMdcdEdx1 = -1;
    fInnerMdcdEdx2 = -1;
    fInnerMdcdEdxSigma1 = -1;
    fInnerMdcdEdxSigma2 = -1;
    fOuterMdcdEdx1 = -1;
    fOuterMdcdEdx2 =-1;
    fOuterMdcdEdxSigma1 = -1;
    fOuterMdcdEdxSigma2 = -1;
    fShower_Sum0_1 = -1;
    fShower_Sum0_2 = -1;
    sIdxP1 = -1; 
    sIdxP2 = -1;
    sIdxPidCand1 = -1;
    sIdxPidCand2 = -1;
    sIndInnerSeg1 = -1;
    sIndInnerSeg2 = -1;
    sIndOuterSeg1 = -1;
    sIndOuterSeg2 = -1;
    sIndRICH1 = -1;
    sIndRICH2 = -1;
    sIndShower1 = -1;
    sIndShower2 = -1;
    sIndTOF1 = -1;
    sIndTOF2 = -1;
    fEvtVertexX = -9999.;
    fEvtVertexY = -9999.;
    fEvtVertexZ = -9999.;
    
}
// ---------------------------------------------------------------------------

Float_t* HPairData::getArray(void) const
{
    Float_t* arr = new Float_t[DATAITEMS];
    for (Int_t i=0;i<DATAITEMS;i++) arr[i] = -100.;
    arr[0]=fMass;
    arr[1]=fOpang;
    arr[2]=fRapidity;
    arr[3]=fTransverseMomentum;
    arr[4]=(Float_t)cCharge;
    arr[5]=(Float_t)cIsCut;
    arr[6]=(Float_t)sIdxP1;
    arr[7]=(Float_t)sIdxP2;
    arr[8]=fProbability1;
    arr[9]=fProbability1;
    arr[10]=(Float_t)nPid1;
    arr[11]=(Float_t)nPid2;
    arr[12]=(Float_t)sIdxPidCand1;
    arr[13]=fSys1;
    arr[14]=fR1;
    arr[15]=fZ1;
    arr[16]=fMassExp1;
    arr[17]=fBetaExp1;
    arr[18]=(Float_t)nMomAlg1;
    arr[19]=(Float_t)nChrg1;
    arr[20]=(Float_t)nMostProbPid1;
    arr[21]=fWeightMostProbPID1;
    arr[22]=fThetaDeg1;
    arr[23]=fPhiDeg1;
    arr[24]=(Float_t)nSec1;
    arr[25]=(Float_t)sIdxPidCand2;
    arr[26]=fSys2;
    arr[27]=fR2;
    arr[28]=fZ2;
    arr[29]=fMassExp2;
    arr[30]=fBetaExp2;
    arr[31]=(Float_t)nMomAlg2;
    arr[32]=(Float_t)nChrg2;
    arr[33]=(Float_t)nMostProbPid2;
    arr[34]=fWeightMostProbPID2;
    arr[35]=fThetaDeg2;
    arr[36]=fPhiDeg2;
    arr[37]=(Float_t)nSec2;
    arr[38]=fDeltaThetaRM1;
    arr[39]=fDeltaPhiRM1;
    arr[40]=fDeltaThetaRM2;
    arr[41]=fDeltaPhiRM2;
    arr[42]=fTof1;
    arr[43]=fTof2;
    arr[44]=(Float_t)nRingPadNr1;
    arr[45]=fRingCentroid1;
    arr[46]=fRichTheta1;
    arr[47]=fRichPhi1;
    arr[48]=(Float_t)nRingPatMat1;
    arr[49]=(Float_t)nRingHouTra1;
    arr[50]=(Float_t)nRingAmplitude1;
    arr[51]=(Float_t)nRingLocalMax41;
    arr[52]=(Float_t)nRingPadNr2;
    arr[53]=fRingCentroid2;
    arr[54]=fRichTheta2;
    arr[55]=fRichPhi2;
    arr[56]=(Float_t)nRingPatMat2;
    arr[57]=(Float_t)nRingHouTra2;
    arr[58]=(Float_t)nRingAmplitude2;
    arr[59]=(Float_t)nRingLocalMax42;
    arr[60]=fMom1;
    arr[61]=fMom2;
    arr[62]=(Float_t)hasDoubleDetectorHits();
    arr[63]=qSpline1;
    arr[64]=qSpline2;
    arr[65]=rkchi1;
    arr[66]=rkchi2;
    arr[67]=fInnerChiSquare1;
    arr[68]=fInnerChiSquare2;
    arr[69]=fOuterChiSquare1;
    arr[70]=fOuterChiSquare2;

    arr[71]=fTofEloss1;
    arr[72]=fTofEloss2;
    arr[73]=fInnerMdcdEdx1;
    arr[74]=fInnerMdcdEdx2;
    arr[75]=fInnerMdcdEdxSigma1;
    arr[76]=fInnerMdcdEdxSigma2;
    arr[77]=fOuterMdcdEdx1;
    arr[78]=fOuterMdcdEdx2;
    arr[79]=fOuterMdcdEdxSigma1;
    arr[80]=fOuterMdcdEdxSigma2;
    arr[81]=fShower_Sum0_1;
    arr[82]=fShower_Sum0_2;

    arr[83]=fDistanceToVertex1;
    arr[84]=fDistanceToVertex2;

    arr[85]=fAngleToClosestFittedLepton1;
    arr[86]=fAngleToClosestNonFittedLepton1;
    arr[87]=fAngleToClosestFittedHadron1;
    arr[88]=fAngleToClosestNonFittedHadron1;

    arr[89]=fAngleToClosestFittedLepton2;
    arr[90]=fAngleToClosestNonFittedLepton2;
    arr[91]=fAngleToClosestFittedHadron2;
    arr[92]=fAngleToClosestNonFittedHadron2;
#if 0
    arr[85]=(Float_t) bClosestLepIsFitted1;
    arr[86]=(Float_t) bClosestLepIsFitted2;
    arr[87]=(Float_t) bClosestHadIsFitted1;
    arr[88]=(Float_t) bClosestHadIsFitted2;

    arr[89]=fAngleToClosestLep1;
    arr[90]=fAngleToClosestLep2;
    arr[91]=fAngleToClosestHad1;
    arr[92]=fAngleToClosestHad2;
#endif // 0
    arr[93]=IOm_chi2_1;
    arr[94]=IOm_chi2_2;
    arr[95]=aVert.X();
    arr[96]=aVert.Y();
    arr[97]=aVert.Z();
    arr[98]=aDist.X();
    arr[99]=aDist.Y();
    arr[100]=aDist.Z();
    arr[101]=aDist.Mag();

    arr[102]=(Float_t) nEvtNr;
    arr[103]=(Float_t) nDSflag;
    arr[104]=(Float_t) nTrigDec;

    arr[105]=fEvtVertexX;
    arr[106]=fEvtVertexY;
    arr[107]=fEvtVertexZ;

    arr[108]=nRun;

    return arr;
}
// ---------------------------------------------------------------------------

Int_t HPairData::hasDoubleDetectorHits() const
{
    // flags for detectors
    // RICH 1, inner MDC 2, outer MDC 3, SHOWER 4, TOF 5
    if (sIndRICH1>-1 && sIndRICH2>-1 && sIndRICH1==sIndRICH2) return 1;
    if (sIndInnerSeg1>-1 && sIndInnerSeg2>-1 && 
	sIndInnerSeg1==sIndInnerSeg2) return 2;
    if (sIndOuterSeg1>-1 && sIndOuterSeg2>-1 && 
	sIndOuterSeg1==sIndOuterSeg2) return 3;
    if (sIndShower1>-1 && sIndShower2>-1 && sIndShower1==sIndShower2) return 4;
    if (sIndTOF1>-1 && sIndTOF2>-1 && sIndTOF1==sIndTOF2) return 5;

    return 0; // no double hits
}

// --------------------------------------------

const TString& HPairData::getVarNames()
{
    return cVars;
}
// ---------------------------------------------------------------------------



