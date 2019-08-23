// $Id: hpairdata.cc,v 1.9 2005-06-27 11:18:01 wojcik Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2005-05-12 17:11:19
//
// ---------------------------------------------------------------------------
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
#include <hcategory.h>
#include <hpidhitdata.h>
#include <hpidparticle.h>
#include <hpidtrackdata.h>
// ---------------------------------------------------------------------------
#include "hpair.h"
#include "hpairfl.h"
#include "piddef.h"
// ---------------------------------------------------------------------------
#include "hpairdata.h"
// ---------------------------------------------------------------------------
ClassImp(HPairData)
// ---------------------------------------------------------------------------
const TString HPairData::cVars="invmass:opang:rap:pt:charge:isCutNb:idxPart1:idxPart2:prob1:prob2:pid1:pid2:idxpidcand1:sys1:r1:z1:massexp1:betaexp1:momalgidx1:chrg1:mostprobpid1:weightmostprobpid1:theta1:phi1:sec1:idxpidcand2:sys2:r2:z2:massexp2:betaexp2:momalgidx2:chrg2:mostprobpid2:weightmostprobpid2:theta2:phi2:sec2:drmt1:drmp1:drmt2:drmp2:tof1:tof2:rpadnr1:rcentroid1:rt1:rp1:rpatmat1:rhoutra1:rampl1:rlocmax41:rpadnr2:rcentroid2:rt2:rp2:rpatmat2:rhoutra2:rampl2:rlocmax42:mom1:mom2:doubleHit:qspline1:qspline2:innerchisquare1:innerchisquare2:outerchisquare1:outerchisquare2:distancetovertex1:distancetovertex2:iscpcandidate1:iscpcandidate2:opangcpcandidate1:opangcpcandidate2";
const Int_t HPairData::DATAITEMS = 75;
// ---------------------------------------------------------------------------
HPairData::HPairData() 
{
    reset();
}

// ---------------------------------------------------------------------------
HPairData::HPairData(HPair* p) 
{
    init(p);
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
	nPid2<<" idxPidCand1:"<<
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
Bool_t HPairData::init(HPair* p)
{
    reset();

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


    HCategory* pCatPart = HPairFL::getCategory(catPidPart);
    HPidParticle* p1    = (HPidParticle*)pCatPart->getObject((Int_t) sIdxP1);
    HPidParticle* p2    = (HPidParticle*)pCatPart->getObject((Int_t) sIdxP2);

    if (!p1 || !p2) return kFALSE; 
    
    sIdxPidCand1          = p1->getPidCandidateIndex();
    fSys1                 = p1->getSystem();
    fR1                   = p1->getR();
    fZ1                   = p1->getZ();
    fMassExp1             = p1->getMassExp();
    fBetaExp1             = p1->getBetaExp();
    nMomAlg1              = p1->getMomAlg();
    fMom1                 = p1->P();
    nChrg1                = p1->getCharge();
    nMostProbPid1         = p1->getMostProbablePID();
    fWeightMostProbPID1   = p1->getWeightForPID((Short_t)nMostProbPid1);
    
    fThetaDeg1            = p1->thetaDeg();
    fPhiDeg1              = p1->phiDeg();
    nSec1                 = p1->sector();

    sIdxPidCand2          = p2->getPidCandidateIndex();
    fSys2                 = p2->getSystem();
    fR2                   = p2->getR();
    fZ2                   = p2->getZ();
    fMassExp2             = p2->getMassExp();
    fBetaExp2             = p2->getBetaExp();
    nMomAlg2              = p2->getMomAlg();
    fMom2                 = p2->P();
    nChrg2                = p2->getCharge();
    nMostProbPid2         = p2->getMostProbablePID();
    fWeightMostProbPID2   = p2->getWeightForPID((Short_t)nMostProbPid2);
    
    fThetaDeg2            = p2->thetaDeg();
    fPhiDeg2              = p2->phiDeg();
    nSec2                 = p2->sector();
    // -- here additional info from lower lying classes is retrieved
    
    //    HPidCandidate* pPidCand1   =  p1 -> getPidCandidate();
    HPidHitData* pHitData1     =  p1 -> getHitData();
    HPidTrackData* pTrackData1 =  p1 -> getTrackData(); 

    //HPidCandidate* pPidCand2   =  p2 -> getPidCandidate();
    HPidHitData* pHitData2     =  p2 -> getHitData();
    HPidTrackData* pTrackData2 =  p2 -> getTrackData(); 

    //pHitData1->print();
    //pTrackData1->print();

    fDeltaThetaRM1 = pHitData1->getDeltaThetaMDCRICH();
    fDeltaPhiRM1   = pHitData1->getDeltaPhiMDCRICH();
    fDeltaThetaRM2 = pHitData2->getDeltaThetaMDCRICH();
    fDeltaPhiRM2   = pHitData2->getDeltaPhiMDCRICH();
    fTof1          = pHitData1->getTof(); 
    fTof2          = pHitData2->getTof(); 

    nRingPadNr1     = pHitData1->nRingPadNr;
    fRingCentroid1  = pHitData1->fRingCentroid;
    fRichTheta1     = pHitData1->fRichTheta;
    fRichPhi1       = pHitData1->fRichPhi;
    nRingPatMat1    = pHitData1->nRingPatMat;
    nRingHouTra1    = pHitData1->nRingHouTra;
    nRingAmplitude1 = pHitData1->nRingAmplitude;
    nRingLocalMax41 = pHitData1->nRingLocalMax4;

    nRingPadNr2     = pHitData2->nRingPadNr;
    fRingCentroid2  = pHitData2->fRingCentroid;
    fRichTheta2     = pHitData2->fRichTheta;
    fRichPhi2       = pHitData2->fRichPhi;
    nRingPatMat2    = pHitData2->nRingPatMat;
    nRingHouTra2    = pHitData2->nRingHouTra;
    nRingAmplitude2 = pHitData2->nRingAmplitude;
    nRingLocalMax42 = pHitData2->nRingLocalMax4;

    qSpline1         = pTrackData1->fSplineChiSquare;
    fInnerChiSquare1 = pHitData1->fInnerMdcChiSquare;
    fOuterChiSquare1 = pHitData1->fOuterMdcChiSquare;
    bIsCPCandidate1  = pTrackData1->bIsClosePairCandidate;
    fAngleWithCPCandidate1 = pTrackData1->fAngleWithClosePairCandidate;
    fDistanceToVertex1      = pHitData1->fDistanceToVertex[p1->getMomAlg()];

    qSpline2         = pTrackData2->fSplineChiSquare;
    fInnerChiSquare2 = pHitData2->fInnerMdcChiSquare;
    fOuterChiSquare2 = pHitData2->fOuterMdcChiSquare;
    bIsCPCandidate2  = pTrackData2->bIsClosePairCandidate;
    fAngleWithCPCandidate2 = pTrackData2->fAngleWithClosePairCandidate;
    fDistanceToVertex2      = pHitData2->fDistanceToVertex[p2->getMomAlg()];


    // get hit category indices to check if a pair has overlaps
    sIndRICH1     = pHitData1->iIndRICH;
    sIndInnerSeg1 = pHitData1->iIndInnerSeg;
    sIndOuterSeg1 = pHitData1->iIndOuterSeg;
    sIndTOF1      = pHitData1->iIndTOF;
    sIndShower1   = pHitData1->iIndShower;
    
    sIndRICH2     = pHitData2->iIndRICH;
    sIndInnerSeg2 = pHitData2->iIndInnerSeg;
    sIndOuterSeg2 = pHitData2->iIndOuterSeg;
    sIndTOF2      = pHitData2->iIndTOF;
    sIndShower2   = pHitData2->iIndShower;
    
    return kFALSE;
}
// ---------------------------------------------------------------------------
void HPairData::reset()
{
    
    nBitFieldPassedCuts=0;
    fOpang=-1.;
    cCharge=-10;
    cIsCut=-1;
    fMass=-1.;
    fRapidity=-1.;
    fTransverseMomentum=-1.;

     fMass = -1.;
     fOpang = -1.;;
     fRapidity = -1.;
     fTransverseMomentum = -1.;
     cCharge = -10;
     cIsCut = -1;
     sIdxP1 = -1; 
     sIdxP2 = -1;
     fProbability1 = -1.;
     fProbability2 = -1.;
       nPid1 = -1;
       nPid2 = -1;

     sIdxPidCand1 = -1;
     fSys1 = -1.;
     fR1 = -10000.;
     fZ1 = -10000.;
     fMassExp1 = -1.;   
     fBetaExp1 = -1.;
       nMomAlg1 = -1;
     fMom1 = -1.;
       nChrg1 = -10;
       nMostProbPid1 = -1;
     fWeightMostProbPID1 = -1.;
    
     fThetaDeg1 = -1.; 
     fPhiDeg1 = -1.;
       nSec1 = -1;

     sIdxPidCand2 = -1;
     fSys2 = -1.;
     fR2 = -10000.;
     fZ2 = -10000.;
     fMassExp2 = -1.;   
     fBetaExp2 = -1.;
       nMomAlg2 = -1;
     fMom2 = -1.;
       nChrg2 = -10;
       nMostProbPid2 = -1;
     fWeightMostProbPID2 = -1.;
    
     fThetaDeg2 = -1.; 
     fPhiDeg2 = -1.;
       nSec2 = -1;

     fDeltaThetaRM1 = -1000.;
     fDeltaPhiRM1 = -1000.;   
     fDeltaThetaRM2 = -1000.; 
     fDeltaPhiRM2 = -1000.;   
     fTof1 = -1.;          
     fTof2 = -1.;          

     nRingPadNr1 = -1;    
     fRingCentroid1 = -1.; 
     fRichTheta1 = -1.;    
     fRichPhi1 = -1.;      
     nRingPatMat1 = -1;   
     nRingHouTra1 = -1;   
     nRingAmplitude1 = -1;
     nRingLocalMax41 = -1;

     nRingPadNr2 = -1;    
     fRingCentroid2 = -1.; 
     fRichTheta2 = -1.;    
     fRichPhi2 = -1.;      
     nRingPatMat2 = -1;   
     nRingHouTra2 = -1;   
     nRingAmplitude2 = -1;
     nRingLocalMax42 = -1;

     sIndRICH1 = -1;
     sIndInnerSeg1 = -1;
     sIndOuterSeg1 = -1;
     sIndTOF1 = -1;
     sIndShower1 = -1;
     
     sIndRICH2 = -1;
     sIndInnerSeg2 = -1;
     sIndOuterSeg2 = -1;
     sIndTOF2 = -1;
     sIndShower2 = -1;



    
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
    arr[65]=fInnerChiSquare1;
    arr[66]=fInnerChiSquare2;
    arr[67]=fOuterChiSquare1;
    arr[68]=fOuterChiSquare2;
    arr[69]=fDistanceToVertex1;
    arr[70]=fDistanceToVertex2;
    arr[71]=(Float_t)bIsCPCandidate1;
    arr[72]=(Float_t)bIsCPCandidate2;
    arr[73]=fAngleWithCPCandidate1;
    arr[74]=fAngleWithCPCandidate2;

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
    if (sIndTOF1>-1 && sIndTOF2>-1 && sIndTOF1==sIndTOF2) return 5;
    if (sIndShower1>-1 && sIndShower2>-1 && sIndShower1==sIndShower2) return 4;

	if( ((nMostProbPid1!=2&&nMostProbPid1!=3) && ( nMostProbPid2==2 || nMostProbPid2==3))
	    || ((nMostProbPid2!=2 && nMostProbPid2!=3) && ( nMostProbPid1==2 || nMostProbPid1==3 )) )
	{
		
      if (sIndRICH1>-1 && sIndRICH2>-1 && sIndRICH1==sIndRICH2) return 6;
	  if (sIndInnerSeg1>-1 && sIndInnerSeg2>-1 && sIndInnerSeg1==sIndInnerSeg2) return 7;
	  if (sIndOuterSeg1>-1 && sIndOuterSeg2>-1 && sIndOuterSeg1==sIndOuterSeg2) return 8;
	  if (sIndTOF1>-1 && sIndTOF2>-1 && sIndTOF1==sIndTOF2) return 9;
	  if (sIndShower1>-1 && sIndShower2>-1 && sIndShower1==sIndShower2) return 10;
	}

    return 0; // no double hits
}

// --------------------------------------------

const TString& HPairData::getVarNames()
{
    return cVars;
}
// ---------------------------------------------------------------------------



