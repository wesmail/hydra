// File: htrackinfo.cc
//
// Author: Laura Fabbietti <L.Fabbietti@physik.tu-muenchen.de>
// Last update by Laura Fabbietti: 01/10/06 21:28:26
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
	richWeight[i]= -1 ;
	mdcWeight[i]= -1 ;
	iMatchedRM[i]= -1 ;
	iMatchedRT_S[i]= -1 ;
	iMatchedMT_S[i]= -1 ;
	iMatchedRMT_S[i]= -1 ; 	
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
