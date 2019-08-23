// $Id: hhitmatchsim.cc,v 1.9 2003-02-05 14:25:11 fabietti Exp $
// Last update by Thomas Eberl: 02/09/25 17:28:42
//
//**********************************************************
// this class is a container class meant to hold one
// "correlation" between different detectors 
//**********************************************************
using namespace std;
#include "hhitmatchsim.h"
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
 }
 
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






