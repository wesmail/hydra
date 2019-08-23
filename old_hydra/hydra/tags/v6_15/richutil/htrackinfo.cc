// $Id: htrackinfo.cc,v 1.10 2003-07-03 12:23:37 eberl Exp $
// Last update by Thomas Eberl: 03/07/02 20:46:24
//
using namespace std;
#include "htrackinfo.h"
#include <iostream> 
#include <iomanip>

ClassImp(HTrackInfo)

HTrackInfo::HTrackInfo()
{
    reset();
}

HTrackInfo::HTrackInfo(const HTrackInfo& source) {

}

HTrackInfo& HTrackInfo::operator=(const HTrackInfo& source)
{
    reset();
    if (this != &source)
    {

	for (Int_t i=0;i<MAXPARTICLES;i++)
	{
// 	    parId[i]=source.getParId(i);
// 	    trackNr[i]=source.getTrkNb(i);
// 	    creaId[i]=source.getCreaId(i);
// 	    creaNb[i]=source.getCreaTrkNb(i);
// 	    mech[i]=source.getMech(i);
// 	    med[i]=source.getMed(i);
// 	    totMom[i]=source.getTotMom(i);
// 	    isPi0Dalitz[i]=source.getPi0Dalitz(i);
// 	    isConvLep[i]=source.getConvLep(i);
// 	    richWeight[i]=source.getRichWeight(i);
// 	    mdcWeight[i]=source.getMdcWeight(i);
// 	    iMatchedRM[i]=source.getMatchedRM(i);
// 	    iMatchedRT_S[i]=source.getMatchedRT_S(i);
// 	    iMatchedMT_S[i]=source.getMatchedMT_S(i);
// 	    iMatchedRMT_S[i]=source.getMatchedRMT_S(i);
// 	    numPhot[i]=source.getNumPhot(i);
// 	    isGCLepInMDC[i]=source.getGCLepInMDC(i);

	    parId[i]=source.parId[i];
	    trackNr[i]=source.trackNr[i];
	    creaId[i]=source.creaId[i];
	    creaNb[i]=source.creaNb[i];
	    mech[i]=source.mech[i];
	    med[i]=source.med[i];
	    totMom[i]=source.totMom[i];
	    isPi0Dalitz[i]=source.isPi0Dalitz[i];
	    isConvLep[i]=source.isConvLep[i];
	    richWeight[i]=source.richWeight[i];
	    mdcWeight[i]=source.mdcWeight[i];
	    iMatchedRM[i]=source.iMatchedRM[i];
	    iMatchedRT_S[i]=source.iMatchedRT_S[i];
	    iMatchedMT_S[i]=source.iMatchedMT_S[i];
	    iMatchedRMT_S[i]=source.iMatchedRMT_S[i];
	    numPhot[i]=source.numPhot[i];
	    isGCLepInMDC[i]=source.isGCLepInMDC[i];
	}
    }

    return *this;
}

HTrackInfo::~HTrackInfo()
{
}
void HTrackInfo::dumpToStdout()
{
    cout<<"***** HTRACKINFO *******************************"<<endl;
    cout<<"***** parID:\t\t\t";
    for (Int_t i=0;i<MAXPARTICLES;i++) {if (parId[i]!=-1) cout<<parId[i]<<"\t";}
    cout<<endl;
    cout<<"***** trackNr:\t\t\t";
    for (Int_t i=0;i<MAXPARTICLES;i++) {if (trackNr[i]!=-1) cout<<trackNr[i]<<"\t";}
    cout<<endl;
    cout<<"***** parent id:\t\t";
    for (Int_t i=0;i<MAXPARTICLES;i++) {if (creaId[i]!=-1) cout<<creaId[i]<<"\t";}
    cout<<endl;
    cout<<"***** parent track nr:\t\t";
    for (Int_t i=0;i<MAXPARTICLES;i++) {if (creaNb[i]!=-1) cout<<creaNb[i]<<"\t";}
    cout<<endl;
    cout<<"***** mechanism:\t\t";
    for (Int_t i=0;i<MAXPARTICLES;i++) {if (mech[i]!=-1) cout<<mech[i]<<"\t";}
    cout<<endl;
    cout<<"***** medium:\t\t\t";
    for (Int_t i=0;i<MAXPARTICLES;i++) {if (med[i]!=-1) cout<<med[i]<<"\t";}
    cout<<endl;
    cout<<"***** tot mom:\t\t\t";
    for (Int_t i=0;i<MAXPARTICLES;i++) {if (totMom[i]!=-1) cout<<totMom[i]<<"\t";}
    cout<<endl;
    cout<<"***** pion dalitz:\t\t";
    for (Int_t i=0;i<MAXPARTICLES;i++) {if (isPi0Dalitz[i]!=-1) cout<<isPi0Dalitz[i]<<"\t";}
cout<<endl;
cout<<"***** conv lepton:\t\t";
for (Int_t i=0;i<MAXPARTICLES;i++) {if (isConvLep[i]!=-1) cout<<isConvLep[i]<<"\t";}
    cout<<endl;
    cout<<"***** richWeight:\t\t";
    for (Int_t i=0;i<MAXPARTICLES;i++) {if (richWeight[i]!=-1) cout<<richWeight[i]<<"\t";}
    cout<<endl;
    cout<<"***** mdcWeight:\t\t";
    for (Int_t i=0;i<MAXPARTICLES;i++) {if (mdcWeight[i]!=-1) cout<<mdcWeight[i]<<"\t";}
    cout<<endl;
    cout<<"***** iMatchedRM:\t\t";
    for (Int_t i=0;i<MAXPARTICLES;i++) {if (iMatchedRM[i]!=-1) cout<<iMatchedRM[i]<<"\t";}
    cout<<endl;
    cout<<"***** iMatchedRT_S:\t\t";
    for (Int_t i=0;i<MAXPARTICLES;i++) {if (iMatchedRT_S[i]!=-1) cout<<iMatchedRT_S[i]<<"\t";}
    cout<<endl;
    cout<<"***** iMatchedMT_S:\t\t";
    for (Int_t i=0;i<MAXPARTICLES;i++) {if (iMatchedMT_S[i]!=-1) cout<<iMatchedMT_S[i]<<"\t";}
    cout<<endl;
    cout<<"***** iMatchedRMT_S:\t\t";
    for (Int_t i=0;i<MAXPARTICLES;i++) {if (iMatchedRMT_S[i]!=-1) cout<<iMatchedRMT_S[i]<<"\t";}
    cout<<endl;
}
void HTrackInfo::reset()
{
    //  cout << "  in HTrackInfo::reset()  "<<endl;
    for(Int_t i=0 ; i<MAXPARTICLES; i++)
    {
	parId[i]= -1 ;
	trackNr[i]= -1 ;
	creaId[i]= -1 ;
	creaNb[i]= -1 ;
	mech[i]= -1 ;
	med[i]= -1 ;
	totMom[i]= -1. ;
	isPi0Dalitz[i]= -1 ;
	isConvLep[i]= -1 ;
	richWeight[i]= -1 ;
	mdcWeight[i]= -1 ;
	iMatchedRM[i]= -1 ;
	iMatchedRT_S[i]= -1 ;
	iMatchedMT_S[i]= -1 ;
	iMatchedRMT_S[i]= -1 ; 	
	numPhot[i] = -1;
	isGCLepInMDC[i] = -1;
    }
}
Int_t HTrackInfo::getPartNr()
{
    Int_t j = 0;
    for(Int_t i =0; i<MAXPARTICLES; i++)
    {
	if(parId[i]>0) j++;
    }
    return j;
}

Int_t HTrackInfo::calcCorrCode(Int_t i)
{
    //Rich-Mdc Rich-Meta Mdc-Meta  CorrCode
    //    0        0        1     :   1 
    //    0        1        0     :   2 
    //    0        1        1     :   3 
    //    1        0        0     :   4 
    //    1        0        1     :   5 
    //    1        1        0     :   6 
    //    1        1        1     :   7 

    Int_t RM   = getMatchedRM(i);
    Int_t RT_S = getMatchedRT_S(i);
    Int_t MT_S = getMatchedMT_S(i);
    Int_t RMT_S= getMatchedRMT_S(i);
    if (RM!=1 && RT_S!=1 && MT_S==1 && RMT_S != 1) return 1;
    if (RM!=1 && RT_S==1 && MT_S!=1 && RMT_S != 1) return 2;
    if (RM!=1 && RT_S==1 && MT_S==1 && RMT_S != 1) return 3;
    if (RM==1 && RT_S!=1 && MT_S!=1 && RMT_S != 1) return 4;
    if (RM==1 && RT_S!=1 && MT_S==1 && RMT_S != 1) return 5;
    if (RM==1 && RT_S==1 && MT_S!=1 && RMT_S != 1) return 6;
    if (RM==1 && RT_S==1 && MT_S==1 && RMT_S == 1) return 7;
    return -1;
}
