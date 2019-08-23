// $Id: hhitmatch.cc,v 1.10 2002-10-21 10:40:25 eberl Exp $
// Last update by Thomas Eberl: 02/10/15 11:08:27
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
//RICH
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
nRingLocalMax4 = source.nRingLocalMax4;
nCentX = source.nCentX;
nCentY = source.nCentY;
fTests = source.fTests;
fMaxClusterSize = source.fMaxClusterSize;
fMaxThrClusterSize = source.fMaxThrClusterSize;
fRingFitRad=source.fRingFitRad;
fRingFitVar=source.fRingFitVar;
fRingFitTheta=source.fRingFitTheta;
fRingFitPhi=source.fRingFitPhi;
nRingFitNbCoords=source.nRingFitNbCoords;
indRich = source.indRich;
//MDC
fMdcTheta = source.fMdcTheta;
fMdcPhi = source.fMdcPhi;
fClusterSize = source.fClusterSize;
nHitLay = source.nHitLay;
iSegIOFlag = source.iSegIOFlag;
indMdc = source.indMdc;
//TOF
fTofTof = source.fTofTof;
fTofTheta = source.fTofTheta;
fTofPhi   = source.fTofPhi; 
indTof = source.indTof;
//SHOWER-TOFINO
fShowerTheta = source.fShowerTheta;
fShowerPhi = source.fShowerPhi;
fTofinoTof = source.fTofinoTof;
m_fSum0 = source.m_fSum0;
m_fSum1 = source.m_fSum1;
m_fSum2 = source.m_fSum2; 
iTofino_Mult = source.iTofino_Mult;
indShower = source.indShower;
//CORR FLAGS
nRichMdcCorr = source.nRichMdcCorr;
nRichKickCorr = source.nRichKickCorr;
nRichTofCorr = source.nRichTofCorr; 
nRichShowerCorr = source.nRichShowerCorr;
nMdcTofCorr = source.nMdcTofCorr;
nMdcShowerCorr = source.nMdcShowerCorr;
//TRACK
indKick = source.indKick;
mom = source.mom;
momt = source.momt;
charge = source.charge;
quality = source.quality;
mass = source.mass;
beta = source.beta;
pull = source.pull;
lvec = source.lvec;  
 }
 
 return *this;
}
void HHitMatch::dumpToStdout()
{
    cout<<"***************** HHITMATCH ************************"<<endl;
    cout<<"RICH-KICK    : "<<getMatchedRichKick()<<endl;
    cout<<"RICH-MDC     : "<<getMatchedRichMdc()<<endl;
    cout<<"RICH-TOF     : "<<getMatchedRichTof()<<endl;
    cout<<"RICH-SHOWER  : "<<getMatchedRichShower()<<endl;
    cout<<"MDC-TOF      : "<<getMatchedMdcTof()<<endl;
    cout<<"MDC-SHOWER   : "<<getMatchedMdcShower()<<endl;
    cout<<"***************************************************"<<endl;
    cout<<"*** Polar (theta) and Azimuthal (phi) hit angle ***"<<endl;
    cout<<"Sector              : "<<getSector()<<" - in/out: "
	<<getMdcFlagIO()<<endl;
    cout<<"RICH(index:"<<getRichInd()<<")    theta: "<<getRichTheta()
	<<" / RICH    phi: "<<getRichPhi()<<endl;
    cout<<"MDC(index:"<<getMdcInd()<<")     theta: "<<getMdcTheta()
	<<" / MDC     phi: "<<getMdcPhi()<<endl;
    cout<<"TOF(index:"<<getTofInd()<<")     theta: "<<getTofTheta()
	<<" / TOF     phi: "<<getTofPhi()
	<<" tof: "<<getTofTof()<<endl;
    cout<<"SHOWER(index:"<<getShowInd()<<")  theta: "<<getShowerTheta()
	<<" / SHOWER  phi: "<<getShowerPhi()
	<<" tof: "<<getTofinoTof()<<endl;
    cout<<"KICK(index:"<<getKickInd()<<")  mom (ptof): "<<getKickMom()
	<<"("<<getKickMomT()<<")"<<" / charge: "
	<<getKickCharge()<<endl;
    cout<<"mass: "<<getKickMass()<<" / pull: "<<getKickPull()
	<<" / beta: "<<getKickBeta()<<endl;
    if (getRichInd()!=-1){
    cout<<"***************************************************"<<endl;
    cout<<"*** RICH Hit Properties (Rings) ***"<<endl;
    cout<<"Ring X (azim): "<<getRingCenterX()
        <<" / Ring Y (polar): "<<getRingCenterY()<<endl;
    cout<<"Ring HT: "<<getRingHouTra()
	<<" / Ring PM: "<<getRingPatMat()<<endl;
    cout<<"Ring Ampl: "<<getRingAmplitude()
	<<" / Ring PadNr: "<<getRingPadNr()
	<<" / Ring LocMax4: "<<getRingLocalMax4()<<endl;
    cout<<"RingFit Rad: "<<getRingFitRadius()
	<<" / Var: "<<getRingFitVar()<<endl;
    cout<<"RingFit Theta: "<<getRingFitTheta()
	<<" / Phi: "<<getRingFitPhi()
	<<" / Nb_Coords: "<<getRingFitNbCoords()<<endl;

    cout<<"***************************************************"<<endl;
    }
}

void HHitMatch::reset(void) {
    indRich = -1;
    indShower =  -1;
    indMdc = -1;
    indTof = -1;
    indKick = -1;
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
    fTofTof = -1;
    fTofinoTof = -1;
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
    fRingFitRad = -1.;
    fRingFitVar = -1.;
    fRingFitTheta = -1.;
    fRingFitPhi = -1.;
    nRingFitNbCoords = -1;
    iSegIOFlag = -1;
    fClusterSize = -1.;
    nRichMdcCorr = -1;
    nRichKickCorr = -1;
    nRichTofCorr = -1;
    nRichShowerCorr = -1;
    nMdcTofCorr = -1;
    nMdcShowerCorr = -1;
    mom=-1.;
    charge=-10;
    momt = -1.;
    quality = -1;
    mass = -1.;
    beta = -1.;
    pull = -1.;

}






