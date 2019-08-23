// $Id: hgeantparticleinfo.cc,v 1.1 2003-09-09 15:39:42 eberl Exp $
// Last update by Thomas Eberl: 03/07/07 10:48:21
//
using namespace std;
#include "hgeantparticleinfo.h"
#include <iostream> 
#include <iomanip>

ClassImp(HGeantParticleInfo)

HGeantParticleInfo::HGeantParticleInfo()
{
    reset();
}

HGeantParticleInfo::HGeantParticleInfo(const HGeantParticleInfo& source) {

}

HGeantParticleInfo& HGeantParticleInfo::operator=(const HGeantParticleInfo& source)
{
    reset();
    if (this != &source)
    {
	geantTrackNr = source.geantTrackNr;
	for (Int_t i=0;i<TRACKS;i++)
	{
	    tracks[i]=source.tracks[i];
	    corrCode[i]=source.corrCode[i];
	}
    }

    return *this;
}

HGeantParticleInfo::~HGeantParticleInfo()
{
}
void HGeantParticleInfo::dumpToStdout()
{
    cout<<"***** HGEANTPARTICLEINFO *************************"<<endl;
    cout<<"GEANT track number "<<geantTrackNr<<endl;
    cout<<"***** recon. Track:\t\t";
    for (Int_t i=0;i<TRACKS;i++) {if (tracks[i]!=-1) cout<<tracks[i]<<"\t";}
    cout<<endl;
    cout<<"***** corrCode:\t\t\t";
    for (Int_t i=0;i<TRACKS;i++) {if (corrCode[i]!=-1) cout<<corrCode[i]<<"\t";}
    cout<<endl;
}
void HGeantParticleInfo::reset()
{
    //  cout << "  in HGeantParticleInfo::reset()  "<<endl;
    for(Int_t i=0 ; i<TRACKS; i++)
    {
	tracks[i]= -1 ;
	corrCode[i]= -1 ;
    }
}
Int_t HGeantParticleInfo::getTrackNr()
{
    Int_t j = 0;
    for(Int_t i =0; i<TRACKS; i++)
    {
	if(tracks[i]>0) j++;
    }
    return j;
}

