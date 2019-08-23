#include "hshowerconfig.h"

ClassImp(HShowerConfig)


HShowerConfig::HShowerConfig(const Char_t* name,const Char_t* title)
    :TNamed(name,title)
{
    setScale(1000);
    for(Int_t i=0;i<3;i++){
	for(Int_t n=0;n<7;n++){
	    nHits[i][n]=0;
	}
    }
}
HShowerConfig::HShowerConfig()
{
    setScale(1000);
    for(Int_t i=0;i<3;i++){
	for(Int_t n=0;n<7;n++){
	    nHits[i][n]=0;
	}
    }
}
HShowerConfig::~HShowerConfig(){}
void HShowerConfig::setHits(Int_t nInd,Int_t nHit)
{
    nHits[nInd][6]++;
    nHits[nInd][nHit]++;
}
void HShowerConfig::resetHits(Int_t nInd)
{
    for(Int_t i =0;i<8;i++){
	nHits[nInd][i]=0;}
}
Int_t*  HShowerConfig::getHits(Int_t nInd)
{
    return nHits[nInd];
}


