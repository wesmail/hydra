//*-- Author : Anar Rustamov
//HMetaMatch
//Class for Matching
#include "hmetamatch.h"
using namespace std;
HMetaMatch::HMetaMatch() 
{
  trkCandInd=-1;
  metaHitInd=-1; 
  nCandForMeta=-1;
  nextCandForMeta=-1;
  firstCandForMeta=-1;
  splineInd=-1;
  richInd=-1;
  ipuInd=-1;
  nCandForRich=-1;
  nextCandForRich=-1;
  nCandForIpu=-1;
  nextCandForIpu=-1;
  firstCandForRich=-1;
  firstCandForIpu=-1;
  rtInd=-1;
  kickInd=-1;
//  sector=;
  system=-1;
}
HMetaMatch::~HMetaMatch()
{
   ;
}

ClassImp(HMetaMatch)
  
   
