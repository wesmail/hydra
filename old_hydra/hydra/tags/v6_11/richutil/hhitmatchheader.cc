// $Id: hhitmatchheader.cc,v 1.2 2002-09-25 16:19:52 eberl Exp $
// Last update by Thomas Eberl: 02/09/25 17:27:55
//
#include "hhitmatchheader.h"
#include <iostream.h>
ClassImp(HHitMatchHeader)

HHitMatchHeader::HHitMatchHeader() {
    reset();
}

HHitMatchHeader::~HHitMatchHeader() {}

HHitMatchHeader::HHitMatchHeader(const HHitMatchHeader& source) {

}

HHitMatchHeader& HHitMatchHeader::operator=(const HHitMatchHeader& source) {
 if (this != &source) {
 }
 
 return *this;
}
void HHitMatchHeader::dumpToStdout()
{
    if (nNb_CorrObjs>0){
    cout<<"-------------- HHitMatchHeader -----------------"<<endl;
    cout<<"Nb of tracklets in evt: "<<nNb_CorrObjs<<endl;
    if(nNb_Rich>0)   cout<<"Tot Nb of RICH hits   : "<<nNb_Rich<<endl;
    if(nNb_MdcSeg>0) cout<<"Tot Nb of MDCSEG hits : "<<nNb_MdcSeg<<endl;
    if(nNb_Tof>0)    cout<<"Tot Nb of TOF hits    : "<<nNb_Tof<<endl;
    if(nNb_Shower>0) cout<<"Tot Nb of SHOWER hits : "<<nNb_Shower<<endl;
    if(nNb_DRich>0)   cout<<"Nb of different RICH hits in tracklets   : "
			  <<nNb_DRich<<endl;
    if(nNb_DMdcSeg>0) cout<<"Nb of different MDCSEG hits in tracklets : "
			  <<nNb_DMdcSeg<<endl;
    if(nNb_DTof>0)    cout<<"Nb of different TOF hits in tracklets    : "
			  <<nNb_DTof<<endl;
    if(nNb_DShower>0) cout<<"Nb of different SHOWER hits in tracklets : "
			  <<nNb_DShower<<endl;
    if(nNb_RM>0) cout<<"Nb of only RM corr : "<<nNb_RM<<endl;
    if(nNb_RT>0) cout<<"Nb of only RT corr : "<<nNb_RT<<endl;
    if(nNb_RS>0) cout<<"Nb of only RS corr : "<<nNb_RS<<endl;
    if(nNb_MT>0) cout<<"Nb of only MT corr : "<<nNb_MT<<endl;
    if(nNb_MS>0) cout<<"Nb of only MS corr : "<<nNb_MS<<endl;
    if(nNb_minRMT>0) cout<<"Nb of min RMT indexes in tracklet : "<<nNb_minRMT<<endl;
    if(nNb_minRMS>0) cout<<"Nb of min RMS indexes in tracklet : "<<nNb_minRMS<<endl;
    if(nNb_RcMcT>0)   cout<<"Nb of exact RM MT    corr : "<<nNb_RcMcT<<endl;
    if(nNb_RcMcTcR>0) cout<<"Nb of exact RM MT RT corr : "<<nNb_RcMcTcR<<endl;
    if(nNb_RMcTcR>0)  cout<<"Nb of exact    MT RT corr : "<<nNb_RMcTcR<<endl;
    if(nNb_RcMTcR>0)  cout<<"Nb of exact RM    RT corr : "<<nNb_RcMTcR<<endl;
    if(nNb_RcMcS>0)   cout<<"Nb of exact RM MS    corr : "<<nNb_RcMcS<<endl;
    if(nNb_RcMcScR>0) cout<<"Nb of exact RM MS RS corr : "<<nNb_RcMcScR<<endl;
    if(nNb_RMcScR>0)  cout<<"Nb of exact   MS RS  corr : "<<nNb_RMcScR<<endl;
    if(nNb_RcMScR>0)  cout<<"Nb of exact RM    RS corr : "<<nNb_RcMScR<<endl;
    cout<<"------------------------------------------------"<<endl;
    }
}

void HHitMatchHeader::reset(void) {
    nNb_CorrObjs=0;
    nNb_RM=0;
    nNb_RT=0;
    nNb_RS=0;
    nNb_MT=0;
    nNb_MS=0;
    nNb_RcMcT=0;
    nNb_RcMcTcR=0;
    nNb_RMcTcR=0;
    nNb_RcMTcR=0;
    nNb_RcMcS=0;
    nNb_RcMcScR=0;
    nNb_RMcScR=0;
    nNb_RcMScR=0;
    nNb_minRMT=0;
    nNb_minRMS=0;
}






