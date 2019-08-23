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
//   
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
//--------------------------------------------------------------------------
//  dedx fom Mdc segments can beretrieved via
//  getdedxInnerSeg()         mean value of t2-t1 for inner segment
//  getdedxOuterSeg()         sigma of t2-t1 distribution in inner segment
//  getSigmadedxInnerSeg()    number of wires in inner segment
//                            before truncated mean procedure
//  getSigmadedxOuterSeg()    number of wires in inner segment
//                            cutted by truncated mean procedure
//  getNWirededxInnerSeg()    mean value of t2-t1 for outer segment
//  getNWirededxOuterSeg()    sigma of t2-t1 distribution in outer segment
//  getNWireCutdedxInnerSeg() number of wires in outer segment
//                            before truncated mean procedure
//  getNWireCutdedxOuterSeg() number of wires in outer segment
//                            cutted by truncated mean procedure
//
//  to do it inside a loop the following functions ar provided
//
//  getdedxSeg(Int_t seg)          seg==0: for inner seg , seg==1 for outer seg
//  getSigmadedxSeg(Int_t seg)     seg==0: for inner seg , seg==1 for outer seg
//  getNWirededxSeg(Int_t seg)     seg==0: for inner seg , seg==1 for outer seg
//  getNWireCutdedxSeg(Int_t seg)  seg==0: for inner seg , seg==1 for outer seg
//  setdedxSeg(Int_t seg,Float_t dedx)        seg==0: for inner seg , seg==1 for outer seg
//  setSigmadedxSeg(Int_t seg,Float_t sig)    seg==0: for inner seg , seg==1 for outer seg
//  setNWirededxSeg(Int_t seg,UChar_t nw)     seg==0: for inner seg , seg==1 for outer seg
//  setNWireCutdedxSeg(Int_t seg,UChar_t nw)  seg==0: for inner seg , seg==1 for outer seg
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
  metaMatchInd=-1;
  dedxInner=dedxOuter=-1;
  dedxSigmaInner=dedxSigmaOuter=-1;
  dedxNWireInner=dedxNWireOuter=0;
  dedxNWireCutInner=dedxNWireCutOuter=0;
  flag=0;
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
  metaMatchInd=-1;
  dedxInner=dedxOuter=-1;
  dedxSigmaInner=dedxSigmaOuter=-1;
  dedxNWireInner=dedxNWireOuter=0;
  dedxNWireCutInner=dedxNWireCutOuter=0;
  flag=0;
}

void HMdcTrkCand::clear(void) {
  seg1Ind=seg2Ind=-1;
  nCandForSeg1=0;
  nextCandInd=-1;
  firstCandInd=-1;
  pFirstCand=this;
  metaMatchInd=-1;
  dedxInner=dedxOuter=-1;
  dedxSigmaInner=dedxSigmaOuter=-1;
  dedxNWireInner=dedxNWireOuter=0;
  dedxNWireCutInner=dedxNWireCutOuter=0;
  flag=0;
}

void HMdcTrkCand::print(void) const {
  printf("HMdcTrkCand: %isec. Inner seg.ind.=%i Outer seg.ind.=%i",
      sector+1,seg1Ind,seg2Ind);
  if(nCandForSeg1==0) printf(" No outer segnets!");
  else if(nCandForSeg1>0) 
      printf(" Num.of.outer segments=%i.",nCandForSeg1);
  printf("\n");
}
void HMdcTrkCand::Streamer(TBuffer &R__b)
{
   // Stream an object of class HMdcTrkCand.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> sector;
      R__b >> seg1Ind;
      R__b >> seg2Ind;
      R__b >> nCandForSeg1;
      R__b >> nextCandInd;
      R__b >> firstCandInd;
      if(R__v>1)
      {
	  R__b >> dedxInner;
	  R__b >> dedxSigmaInner;
	  R__b >> dedxNWireInner;
	  R__b >> dedxNWireCutInner;
	  R__b >> dedxOuter;
	  R__b >> dedxSigmaOuter;
	  R__b >> dedxNWireOuter;
	  R__b >> dedxNWireCutOuter;
      }
      if(R__v<3) {
        Short_t splineTrkInd,rtTrkInd;
        R__b >> splineTrkInd;
        R__b >> rtTrkInd;
        metaMatchInd=-1;
      }
      if(R__v>=3){
	R__b >> metaMatchInd;
	R__b >> flag;
      }
      R__b.CheckByteCount(R__s, R__c, HMdcTrkCand::IsA());
   } else {
      R__c = R__b.WriteVersion(HMdcTrkCand::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << sector;
      R__b << seg1Ind;
      R__b << seg2Ind;
      R__b << nCandForSeg1;
      R__b << nextCandInd;
      R__b << firstCandInd;
      R__b << dedxInner;
      R__b << dedxSigmaInner;
      R__b << dedxNWireInner;
      R__b << dedxNWireCutInner;
      R__b << dedxOuter;
      R__b << dedxSigmaOuter;
      R__b << dedxNWireOuter;
      R__b << dedxNWireCutOuter;
      R__b << metaMatchInd;
      R__b << flag;
      R__b.SetByteCount(R__c, kTRUE);
   }
}
