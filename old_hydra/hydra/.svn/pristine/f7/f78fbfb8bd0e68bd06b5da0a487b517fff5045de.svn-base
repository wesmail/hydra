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
  isTofCluster=100;
  rtInd=-1;
  kickInd=kick123Ind=-1;
  sector=-1;
  for(Int_t i = 0; i < RICH_TAB_SIZE; i++) aRichIndTable[i] = -1;
  for(Int_t i = 0; i < RICH_TAB_SIZE; i++) aRichIPUIndTable[i] = -1;
  nRichId=nRichIPUId=0;
  flag=0;
  tofClusterSize=0;
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
  isTofCluster=100;
  for(Int_t i = 0; i < RICH_TAB_SIZE; i++) aRichIndTable[i] = -1;
  for(Int_t i = 0; i < RICH_TAB_SIZE; i++) aRichIPUIndTable[i] = -1;
  nRichId=nRichIPUId=0;
  flag=0;
  tofClusterSize=0;
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
  
  isTofCluster=100;
  
  
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
   tofClusterSize=0;
   
}

void HMetaMatch::Streamer(TBuffer &R__b) {
  // Stream an object of class HMetaMatch.

  UInt_t R__s, R__c;
  if (R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
    TObject::Streamer(R__b);
    R__b >> trkCandInd;
    R__b >> showerHitInd;
    R__b >> tofHitInd;
    R__b >> nCandForMeta;
    R__b >> nextCandForMeta;
    R__b >> firstCandForMeta;
    R__b >> qualityTOF;
    R__b >> qualitySHOWER;
    R__b >> splineInd;
    R__b >> rtInd;
    R__b >> kickInd;
    R__b >> kick123Ind;
    R__b >> sector;
    R__b >> nRichId;
    R__b >> nRichIPUId;
    R__b.ReadStaticArray((int*)aRichIndTable);
    R__b.ReadStaticArray((int*)aRichIPUIndTable);
    R__b >> flag;
    if (R__v > 1) {
      R__b >>tofClusterSize;
      R__b >>isTofCluster;
    } else {
      tofClusterSize=0;
      isTofCluster=0;
    }
    

    
    R__b.CheckByteCount(R__s, R__c, HMetaMatch::IsA());
  } else {
    R__c = R__b.WriteVersion(HMetaMatch::IsA(), kTRUE);
    TObject::Streamer(R__b);
    R__b << trkCandInd;
    R__b << showerHitInd;
    R__b << tofHitInd;
    R__b << nCandForMeta;
    R__b << nextCandForMeta;
    R__b << firstCandForMeta;
    R__b << qualityTOF;
    R__b << qualitySHOWER;
    R__b << splineInd;
    R__b << rtInd;
    R__b << kickInd;
    R__b << kick123Ind;
    R__b << sector;
    R__b << nRichId;
    R__b << nRichIPUId;
    R__b.WriteArray(aRichIndTable, 3);
    R__b.WriteArray(aRichIPUIndTable, 3);
    R__b << flag;
    R__b << tofClusterSize;
    R__b << isTofCluster;
    R__b.SetByteCount(R__c, kTRUE);
  }
}


ClassImp(HMetaMatch)
  
   
