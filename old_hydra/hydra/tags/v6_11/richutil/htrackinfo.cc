// $Id: htrackinfo.cc,v 1.5 2002-09-25 16:19:53 eberl Exp $
// Last update by Thomas Eberl: 02/09/25 17:47:21
//
#include "htrackinfo.h"
#include <iostream.h>

ClassImp(HTrackInfo)

HTrackInfo::HTrackInfo()
{
    reset();
}
HTrackInfo::~HTrackInfo()
{
}
void HTrackInfo::dumpToStdout()
{
    cout<<"***** HTRACKINFO *******************************"<<endl;
    cout<<"***** parID: ";
    for (Int_t i=0;i<MAXPARTICLES;i++) cout<<parId[i]<<" ";
    cout<<endl;
    cout<<"***** trackNr: ";
    for (Int_t i=0;i<MAXPARTICLES;i++) cout<<trackNr[i]<<" ";
    cout<<endl;
    cout<<"***** parent id: ";
    for (Int_t i=0;i<MAXPARTICLES;i++) cout<<creaId[i]<<" ";
    cout<<endl;
    cout<<"***** mechanism: ";
    for (Int_t i=0;i<MAXPARTICLES;i++) cout<<mech[i]<<" ";
    cout<<endl;
    cout<<"***** medium: ";
    for (Int_t i=0;i<MAXPARTICLES;i++) cout<<med[i]<<" ";
    cout<<endl;
    cout<<"***** tot mom: ";
    for (Int_t i=0;i<MAXPARTICLES;i++) cout<<totMom[i]<<" ";
    cout<<endl;
    cout<<"***** pion dalitz: ";
    for (Int_t i=0;i<MAXPARTICLES;i++) cout<<isPi0Dalitz[i]<<" ";
    cout<<endl;
    cout<<"***** conv lepton: ";
    for (Int_t i=0;i<MAXPARTICLES;i++) cout<<isConvLep[i]<<" ";
    cout<<endl;
    cout<<"***** richWeight: ";
    for (Int_t i=0;i<MAXPARTICLES;i++) cout<<richWeight[i]<<" ";
    cout<<endl;
    cout<<"***** mdcWeight: ";
    for (Int_t i=0;i<MAXPARTICLES;i++) cout<<mdcWeight[i]<<" ";
    cout<<endl;
    cout<<"***** iMatchedRM: ";
    for (Int_t i=0;i<MAXPARTICLES;i++) cout<<iMatchedRM[i]<<" ";
    cout<<endl;
    cout<<"***** iMatchedRT_S: ";
    for (Int_t i=0;i<MAXPARTICLES;i++) cout<<iMatchedRT_S[i]<<" ";
    cout<<endl;
    cout<<"***** iMatchedMT_S: ";
    for (Int_t i=0;i<MAXPARTICLES;i++) cout<<iMatchedMT_S[i]<<" ";
    cout<<endl;
    cout<<"***** iMatchedRMT_S: ";
    for (Int_t i=0;i<MAXPARTICLES;i++) cout<<iMatchedRMT_S[i]<<" ";
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
