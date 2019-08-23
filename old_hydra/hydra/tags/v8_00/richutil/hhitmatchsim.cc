// $Id: hhitmatchsim.cc,v 1.11 2004-10-15 08:25:17 eberl Exp $
// Last update by Thomas Eberl: 03/07/09 17:51:24
//
//**********************************************************
// this class is a container class meant to hold one
// "correlation" between different detectors 
//**********************************************************
using namespace std;
#include "hhitmatchsim.h"
#include "hlinearcategory.h"
#include "hrichhitsim.h"
#include "hrichutilfunc.h"
#include "richdef.h"
#include <iostream> 
#include <iomanip>
ClassImp(HHitMatchSim)

HHitMatchSim::HHitMatchSim() {
    reset();
}

HHitMatchSim::~HHitMatchSim() {}

HHitMatchSim::HHitMatchSim(const HHitMatchSim& source) {

}

HHitMatchSim& HHitMatchSim::operator=(const HHitMatchSim& source) {
 if (this != &source) {
     reset();
     resetSim();
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
     fMdcR   = source.fMdcR;
     fMdcZ   = source.fMdcZ;
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

     // sim part
     isFakeFlag = source.isFakeFlag;
     isLeptonFlag = source.isLeptonFlag;
     isGLeptonRing = source.isGLeptonRing;
     isGCLeptonRing  = source.isGCLeptonRing;
     nNbGPart = source.nNbGPart;
     isLeptonOnMirror = source.isLeptonOnMirror;

     trackMatched = source.trackMatched;

 }
 //dumpToStdoutSim();
 return *this;
}
void HHitMatchSim::dumpToStdoutSim()
{
    HHitMatch::dumpToStdout();
    //    cout<<"in HHitMatchSim::dumpToStdout()"<<endl;
    if(isFake()) cout<<"FAKE !!!!!"<<endl;
    else
    {
	cout<<"CONFIRMED CORRELATION"<<endl;
	if(isLepton()) cout<<"is confirmed Lepton !!!!"<<endl;
    }
    trackMatched.dumpToStdout();
}

void HHitMatchSim::resetSim(void) 
{
    
    isFakeFlag=1;//1:yes
    isLeptonFlag=0;//1:yes
    isGCLeptonRing=-1;//0:no ring;1:single;2:double in corr
    isGLeptonRing=-1;
    nNbGPart=-1;//number of Geant particles in tracklet, given by HTrackInfo
    isGLepInMDC=-1;
    numPhot1 = -1;
    numPhot2 = -1;
    isLeptonOnMirror = 0;
    weightRatio = 0.;
}

HTrackInfo * HHitMatchSim::getTrackInfoObj()
{





    // example how to repair track info object before returning it

    // the HRichCorrelatorSim was changed ( on  2003-07-09 ) to clarify 
    // the GEANT track matching. 
    // A bug was not found, but had been suspected, which would have
    // prevented all GEANT track number matchings involving RICH,
    // when the first stored track number in RICH was negative (noise or feedback) 
//     HLinearCategory * cat = (HLinearCategory*) HRichUtilFunc::getCategory(catRichHit);
//     HRichHitSim* r = 0;
//     if (cat)
//     {
// 	r = (HRichHitSim*)cat->getObject(indRich);
//     }
//     else cout<<"HHitMatchSim::getTrackInfoObj, no HRichHit cat found for 
//                 trackinfo repair"<<endl;
//     Bool_t kPrint=kFALSE;

//     if (r && r->track1==-5)
//     {
// 	cout<<"RICH track/weight"<<endl;
// 	cout<<r->track1<<"/"<<r->weigTrack1<<" ";
// 	cout<<r->track2<<"/"<<r->weigTrack2<<" ";
// 	cout<<r->track3<<"/"<<r->weigTrack3<<endl;
// 	trackMatched.dumpToStdout();
	
	
//     }

//     for(Int_t i=0;i<MAXPARTICLES;i++)
//     {
// 	Int_t trkNb = trackMatched.getTrkNb(i);
// 	if ( trkNb > 0)
// 	{

	    
// 	    if (r && (r->track1==trkNb || r->track2==trkNb || r->track3==trkNb))
// 	    {
// 		// if it is found, check whether the matching was stored or not
// 		Int_t RM   = trackMatched.getMatchedRM(i);
// 		Int_t RTS  = trackMatched.getMatchedRT_S(i);
// 		Int_t MTS  = trackMatched.getMatchedMT_S(i);
// 		Int_t RMTS = trackMatched.getMatchedRMT_S(i);

// 		if(r)
// 		    {
// 			cout<<"RICH track/weight"<<endl;
// 			cout<<r->track1<<"/"<<r->weigTrack1<<" ";
// 			cout<<r->track2<<"/"<<r->weigTrack2<<" ";
// 			cout<<r->track3<<"/"<<r->weigTrack3<<" * ";
// 			cout<<trkNb<<" "<<RM<<" "<<RTS<<" "<< MTS<<" "<< RMTS<<endl;
// 		    }
	       

// 		if (MTS==1 && RM==-1 && RTS==-1 && RMTS==-1) 
// 		{
// 		    if(r)
// 		    {
// 			cout<<"RICH track/weight"<<endl;
// 			cout<<r->track1<<"/"<<r->weigTrack1<<" ";
// 			cout<<r->track2<<"/"<<r->weigTrack2<<" ";
// 			cout<<r->track3<<"/"<<r->weigTrack3<<endl;
// 		    }
// 		    cout<<"before repair ***************************"<<endl;
// 		    trackMatched.dumpToStdout();

// 		    trackMatched.setMatchedRM(i);
// 		    trackMatched.setMatchedRT_S(i);
// 		    trackMatched.setMatchedRMT_S(i);
// 		    kPrint = kTRUE;
// 		}
// 	    }
// 	}
//     }


//     if (kPrint)
//     {
// 	trackMatched.dumpToStdout();
// 	cout<<"after repair ***************************"<<endl;
//     }

    return &trackMatched;
}




