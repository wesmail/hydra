// $Id: hhitmatchsim.cc,v 1.6 2002-09-25 16:19:52 eberl Exp $
// Last update by Thomas Eberl: 02/09/25 17:28:42
//
//**********************************************************
// this class is a container class meant to hold one
// "correlation" between different detectors 
//**********************************************************
#include "hhitmatchsim.h"
#include <iostream.h>
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
    numPhot=-1;
    isGLepInMDC=-1;
}






