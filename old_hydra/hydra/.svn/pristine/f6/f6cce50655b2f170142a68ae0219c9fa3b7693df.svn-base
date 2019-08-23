// File: hhitmatch.cc
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 01/10/08 17:08:22
//
//**********************************************************
// this class is a container class meant to hold one
// "correlation" between different detectors 
//**********************************************************
#include "hhitmatch.h"
#include <iostream.h>
ClassImp(HHitMatch)

HHitMatch::HHitMatch() {
    reset();
}

HHitMatch::~HHitMatch() {}

HHitMatch::HHitMatch(const HHitMatch& source) {

}

HHitMatch& HHitMatch::operator=(const HHitMatch& source) {
 if (this != &source) {
nSector= source.nSector;
fRichPhi  = source.fRichPhi;
fRichTheta = source.fRichTheta;
nRingX = source.nRingX;
nRingY = source.nRingY;
nRingPatMat = source.nRingPatMat;
nRingHouTra = source.nRingHouTra;
nRingAmplitude = source.nRingAmplitude;
nRingPadNr = source.nRingPadNr;
fRingRadius = source.fRingRadius;
fRingCentroid = source.fRingCentroid;
nCentX = source.nCentX;
 nCentY = source.nCentY;
nRingLocalMax4 = source.nRingLocalMax4;
fMaxClusterSize = source.fMaxClusterSize;
fMaxThrClusterSize = source.fMaxThrClusterSize;
fTests = source.fTests;
indRich = source.indRich;
fMdcTheta = source.fMdcTheta;
fMdcPhi = source.fMdcPhi;
//fSecMdcSegThetaCut = source.fSecMdcSegThetaCut;
//fSecMdcSegPhiCut = source.fSecMdcSegPhiCut;
fClusterSize = source.fClusterSize;
nHitLay = source.nHitLay;
indMdc = source.indMdc;
fTofTof = source.fTofTof;
fTofTheta = source.fTofTheta;
fTofPhi   = source.fTofPhi; 
 indTof = source.indTof;
 fShowerTheta = source.fShowerTheta;
 fShowerPhi = source.fShowerPhi;
fTofinoTof = source.fTofinoTof;
m_fSum0 = source.m_fSum0;
 m_fSum1 = source.m_fSum1;
m_fSum2 = source.m_fSum2; 
iTofino_Mult = source.iTofino_Mult;
indShower = source.indShower;
// fRichMdcThetaCut = source.fRichMdcThetaCut;
// fRichTofThetaCut = source.fRichTofThetaCut;
// fMdcTofThetaCut = source.fMdcTofThetaCut;
// fMdcShowerThetaCut = source.fMdcShowerThetaCut;
// fRichMdcPhiCut = source.fRichMdcPhiCut;
// fRichTofPhiCut = source.fRichTofPhiCut;
// fRichShowerPhiCut = source.fRichShowerPhiCut;
// fMdcTofPhiCut = source.fMdcTofPhiCut;
//fMdcShowerPhiCut = source.fMdcShowerPhiCut;
//fTofMin = source.fTofMin;
//fTofMax = source.fTofMax;
//fTofinoMin = source.fTofinoMin;
//fTofinoMax = source.fTofinoMax;
//fShowerAmplification = source.fShowerAmplification;
//nTofinoMultCut = source.nTofinoMultCut;
//nShowerCond = source.nShowerCond;
nRichMdcCorr = source.nRichMdcCorr;
nRichTofCorr = source.nRichTofCorr; 
nRichShowerCorr = source.nRichShowerCorr;
nMdcTofCorr = source.nMdcTofCorr;
nMdcShowerCorr = source.nMdcShowerCorr;
 }
 
 return *this;
}
void HHitMatch::dumpToStdout()
{
    cout<<"***************** HHITMATCH ************************"<<endl;
    cout<<"RICH-MDC     : "<<getMatchedRichMdc()<<endl;
    cout<<"RICH-TOF     : "<<getMatchedRichTof()<<endl;
    cout<<"RICH-SHOWER  : "<<getMatchedRichShower()<<endl;
    cout<<"MDC-TOF      : "<<getMatchedMdcTof()<<endl;
    cout<<"MDC-SHOWER   : "<<getMatchedMdcShower()<<endl;
    cout<<"***************************************************"<<endl;
    cout<<"*** Polar (theta) and Azimuthal (phi) hit angle ***"<<endl;
    cout<<"Sector              : "<<getSector()<<endl;
    cout<<"RICH    theta       : "<<getRichTheta()
	<<" / RICH    phi         : "<<getRichPhi()<<endl;
    cout<<"MDC     theta       : "<<getMdcTheta()
	<<" / MDC     phi         : "<<getMdcPhi()<<endl;
    cout<<"TOF     theta       : "<<getTofTheta()
	<<" / TOF     phi         : "<<getTofPhi()<<endl;
    cout<<"SHOWER  theta       : "<<getShowerTheta()
	<<" / SHOWER  phi         : "<<getShowerPhi()<<endl;
  //   cout<<"***************************************************"<<endl;
//     cout<<"*** Time-of-flight ***"<<endl;
//     cout<<"TOF [ns]    : "<<getTofTof()<<endl;
//     cout<<"TOF Cut --- min : "<<getTofMin()
// 	<<"   max : "<<getTofMax()<<endl;
//     cout<<"TOFINO [ns] : "<<getTofinoTof()<<endl;
//     cout<<"TOFINO Cut --- min : "<<getTofinoMin()
// 	<<"   max : "<<getTofinoMax()<<endl;
    cout<<"***************************************************"<<endl;
    cout<<"*** RICH Hit Properties (Rings) ***"<<endl;
    cout<<"Ring X (azim)   : "<<getRingCenterX()
        <<" / Ring Y (polar)  : "<<getRingCenterY()<<endl;
    cout<<"Ring HT         : "<<getRingHouTra()
	<<" / Ring PM         :"<<getRingPatMat()<<endl;
    cout<<"Ring Ampl :"<<getRingAmplitude()
	<<" / Ring PadNr  :"<<getRingPadNr()
	<<" / Ring LocMax4  :"<<getRingLocalMax4()<<endl;
//     cout<<"***************************************************"<<endl;
//     cout<<"*** Close Track Properties ***"<<endl;
//     cout<<"Close track MDC theta cut : "<<getClosePairThetaCut()<<endl;
//     cout<<"Close track MDC phi cut   : "<<getClosePairPhiCut()<<endl;
    cout<<"***************************************************"<<endl;
//     cout<<"*** Angular Correlation Windows ***"<<endl;
//     cout<<"RICH MDC    theta   : +/- "<<getRichMdcThetaCut()
// 	<<" / RICH MDC    phi     : +/- "<<getRichMdcPhiCut()<<endl;
//     cout<<"RICH TOF    theta   : +/- "<<getRichTofThetaCut()
// 	<<" / RICH TOF    phi     : +/- "<<getRichTofPhiCut()<<endl;
//     cout<<"RICH SHOWER theta   : +/- "<<getRichShowerThetaCut()
// 	<<" / RICH SHOWER phi     : +/- "<<getRichShowerPhiCut()<<endl;
//     cout<<"MDC  TOF    theta   : +/- "<<getMdcTofThetaCut()
// 	<<" / MDC  TOF    phi     : +/- "<<getMdcTofPhiCut()<<endl;
//     cout<<"MDC  SHOWER theta   : +/- "<<getMdcShowerThetaCut()
// 	<<" / MDC  SHOWER phi     : +/- "<<getMdcShowerPhiCut()<<endl;
    //    cout<<"***************************************************"<<endl;
}

void HHitMatch::reset(void) {
    indRich = -1;
    indShower =  -1;
    indMdc = -1;
    indTof = -1;

    nSector = -1;
    fRichPhi = -1.;
    fRichTheta = -1.;
    fMdcTheta = -1.; 
    fMdcPhi = -1.;
    fShowerTheta = -1.;
    fShowerPhi = -1.;
    fTofTheta = -1.;
    fTofPhi = -1.;
    nHitLay = -1;
    indMdc = -1;
    m_fSum0 = -1 ;
    m_fSum1 = -1 ;
    m_fSum2 = -1 ;
    iTofino_Mult = -1;
    // CLOSE MDC SEG HITS
    //fSecMdcSegThetaCut = -1.;
    //fSecMdcSegPhiCut = -1.;
    // nThirdMdcSeg;
    // fThirdMdcSegCut;
    // Time of flight
    fTofTof = -1;
    fTofinoTof = -1;
//     fTofMin = -1;
//     fTofMax = -1;
//     fTofinoMin = -1;
//     fTofinoMax = -1;
    // Ring values
    nRingX = -1;// RICH hit properties copied from HRichHit
    nRingY = -1;
    nRingPatMat = -1;
    nRingHouTra = -1;
    nRingAmplitude = -1;
    nRingPadNr = -1;
    fRingRadius = -1;
    fRingCentroid = -1; 
    nCentX = -1;
    nCentY = -1;
    nRingLocalMax4 = -1;
    fMaxClusterSize = -1;
    fMaxThrClusterSize = -1;
    fTests = -10;
    // CUTS
//     fRichMdcThetaCut = -1.; 
//     fRichTofThetaCut = -1.; 
//     fRichShowerThetaCut = -1.;
//     fMdcTofThetaCut = -1.;
//     fMdcShowerThetaCut = -1.;

    
//     fRichMdcPhiCut = -1.;
//     fRichTofPhiCut = -1.;
//     fRichShowerPhiCut = -1.; 
//     fMdcTofPhiCut = -1.;
//     fMdcShowerPhiCut = -1.;
//     nTofinoMultCut = -1;
    // CORRELATION FLAGS
    nRichMdcCorr = -1;
    nRichTofCorr = -1;
    nRichShowerCorr = -1;
    nMdcTofCorr = -1;
    nMdcShowerCorr = -1;
    // SWITCHES
    //    nShowerCond = -1;
}






