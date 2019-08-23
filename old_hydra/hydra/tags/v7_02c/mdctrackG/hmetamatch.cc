//*-- Author : Anar Rustamov
//HMetaMatch
//Class for Matching
#include "hmetamatch.h"
using namespace std;

HMetaMatch::HMetaMatch() {
  trkCandInd=-1;
  showerHitInd=tofHitInd=-1; 
  nCandForMeta=-1;
  nextCandForMeta=-1;
  firstCandForMeta=-1;
  splineInd=-1;
  
  qualityTOF=-1000;
  qualitySHOWER=-1000;
  
  rtInd=-1;
  kickInd=kick123Ind=-1;
  sector=-1;
  for(Int_t i = 0; i < RICH_TAB_SIZE; i++) aRichIndTable[i] = -1;
  for(Int_t i = 0; i < RICH_TAB_SIZE; i++) aRichIPUIndTable[i] = -1;
  nRichId=nRichIPUId=0;
  flag=0;
}

HMetaMatch::HMetaMatch(Short_t sec, Short_t tkInd, Short_t ind) {
  sector=sec;
  firstCandForMeta=ind;
  trkCandInd=tkInd;
  
  qualityTOF=-1000;
  qualitySHOWER=-1000;
  
  showerHitInd=tofHitInd=-1; 
  nCandForMeta=-1;
  nextCandForMeta=-1;
  splineInd=-1;
  rtInd=-1;
  kickInd=kick123Ind=-1;
  for(Int_t i = 0; i < RICH_TAB_SIZE; i++) aRichIndTable[i] = -1;
  for(Int_t i = 0; i < RICH_TAB_SIZE; i++) aRichIPUIndTable[i] = -1;
  nRichId=nRichIPUId=0;
  flag=0;
}
  
HMetaMatch::HMetaMatch(HMetaMatch* fMetaMatch, Short_t ind) {
  // ind - index of this object
  fMetaMatch->nextCandForMeta=ind;
  
  sector=fMetaMatch->sector;
  trkCandInd=fMetaMatch->trkCandInd;
  firstCandForMeta=fMetaMatch->firstCandForMeta;
  nCandForMeta=-1;
  
  qualityTOF=-1000;
  qualitySHOWER=-1000;
  
  showerHitInd=tofHitInd=-1;
  nextCandForMeta=-1;
  splineInd=-1;
  rtInd=-1;           //?????????
  kickInd=kick123Ind=-1;
  nRichId=fMetaMatch->nRichId;
  nRichIPUId=fMetaMatch->nRichIPUId;
  for(Int_t i = 0; i < RICH_TAB_SIZE; i++) {
    aRichIndTable[i] = fMetaMatch->aRichIndTable[i];
    aRichIPUIndTable[i] = fMetaMatch->aRichIPUIndTable[i];
  }
  flag=0;
}

ClassImp(HMetaMatch)
  
   
