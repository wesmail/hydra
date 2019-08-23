//*-- AUTHOR : Vladimir Pechenov
//*-- Modified : 18.09.2003 by Vladimir Pechenov


/////////////////////////////////////////////////////////////////////////
//
//  HMdcTrkCand (MDC track candidate)
//
//
//  this class keep information about matching between
//  inner and outer segments:
//    seg1Ind - index of inner segment (HMdcSeg object) in category.
//    seg2Ind - index of outer segment in category. 
//              If seg2Ind==-1  - no outer segments (!).
//    nCandForSeg1 - number of outer segments for this inner
//                   (for each combination exist HMdcTrkCand object).     
//                   If nCandForSeg1==0 - no outer segments               
//                   If nCandForSeg1>0 this container is first one from
//                   nCandForSeg1 objects.
//                   If nCandForSeg1==-1 - one of the next objects.
//    nextCandInd  - index of next HMdcTrkCand object.
//                   If nextCandInd==-1 this object is latest from
//                   nCandForSeg1 objects.
//   splineTrkInd  - index of object filled by Anar's method
//   rtTrkInd      - index of object filled by Manuel's method
//
//  The HMdcTrkCand objects are stored in the matrix category catMdcTrkCand.
//
//  Retrieving of HMdcTrkCand objects.
//    Let pMdcTrkCandCat is poiner to catMdcTrkCand category
//    and iterMdcTrkCand is corresponding iterator then:
//
//    iterMdcTrkCand->Reset();
//    while( (pMdcTrkCand=(HMdcTrkCand*)iterMdcTrkCand->Next()) !=0) {
//      Short_t nOuterSeg=pMdcTrkCand->getNCandForSeg1();
//      if(nOuterSeg==0) {    // no outer segments:
//        ...           
//      } else {
//
//        //----------------------------------------------------------------
//        // If you don't care about number of outer segments for inner one:
//        ...              // your code
//
//        //----------------------------------------------------------------
//        // Otherwise:
//        if(nOuterSeg==-1) continue;
//        Short_t nextObjInd=0;
//        while (nextObjInd>=0) {
//          ...              // your code
//          // Next HMdcTrkCand object geting:
//          nextObjInd = pMdcTrkCand->getNextCandInd();
//          if(nextObjInd>=0) pMdcTrkCand = 
//              (HMdcTrkCand*)pMdcTrkCandCat->getObject(nextObjInd);
//        }
//
//      }
//    }
//
/////////////////////////////////////////////////////////////////////////

#include "hmdctrkcand.h"
#include <stdlib.h> 

ClassImp(HMdcTrkCand)

HMdcTrkCand::HMdcTrkCand(Char_t sec, Short_t s1ind, Int_t ind) {
  // Setting inner segment
  sector=sec;
  seg1Ind=s1ind;
  seg2Ind=-1;
  nCandForSeg1=0;
  nextCandInd=-1;
  firstCandInd=ind;
  pFirstCand=this;
  splineTrkInd=rtTrkInd=-1;
}
  
HMdcTrkCand::HMdcTrkCand(HMdcTrkCand* fTrkCand, Short_t s2ind, Short_t ind) {
  // Setting outer segment
  fTrkCand->nextCandInd=ind;
  sector=fTrkCand->sector;
  seg1Ind=fTrkCand->seg1Ind;
  seg2Ind=s2ind;
  nCandForSeg1=-1;
  nextCandInd=-1;
  firstCandInd=fTrkCand->firstCandInd;
  pFirstCand=fTrkCand->pFirstCand;
  pFirstCand->nCandForSeg1++;
  splineTrkInd=rtTrkInd=-1;
}

void HMdcTrkCand::clear(void) {
  seg1Ind=seg2Ind=-1;
  nCandForSeg1=0;
  nextCandInd=-1;
  firstCandInd=-1;
  pFirstCand=this;
  splineTrkInd=rtTrkInd=-1;
}

void HMdcTrkCand::print(void) const {
  printf("HMdcTrkCand: %isec. Inner seg.ind.=%i Outer seg.ind.=%i",
      sector+1,seg1Ind,seg2Ind);
  if(nCandForSeg1==0) printf(" No outer segnets!");
  else if(nCandForSeg1>0) 
      printf(" Num.of.outer segments=%i.",nCandForSeg1);
  printf("\n");
}
