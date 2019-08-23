//*-- AUTHOR : Vladimir Pechenov
//*-- Modified : 24/05/2000

///////////////////////////////////////////////////////////////
//HMdcTimeCut
//
//  class for cut time1, time2 and time2-time1
//  used for beam data only (time1: leading edge, time2
//  trailing edge, see http://www-hades.gsi.de/docs/mdc/mdcana).
//
//  Useing: create the object HMdcTimeCut in a macros
//  and set the edges for a cut (see hmdctimecut.h)
//  Default seting: 0<time1<200. (ns), 0<time2<200.
//  20.<time2-time1<200.
//
//  If you don't need in a cut don't create this object
//  or set all edges = 0.
//
//  Examples are in mdcTrackD.C and mdcEfficiency.C
//
//////////////////////////////////////////////////////////////

#include "hmdctimecut.h"
#include "hades.h"

ClassImp(HMdcTimeCut) 

HMdcTimeCut::HMdcTimeCut() : HParSet() {
  fName="MdcTimeCut";
  fTitle="cut on time1, time2 & tim2-tim1";
  strcpy(detName,"Mdc");
  cutT1L=0.;
  cutT1R=200.;
  cutT2L=0.;
  cutT2R=200.;
  cutLeft=20.;
  cutRight=200.; 
}

HMdcTimeCut::HMdcTimeCut(Float_t cT1L,  Float_t cT1R, 
                         Float_t cT2L,  Float_t cT2R,
                         Float_t cLeft, Float_t cRight): HParSet() {
  fName="MdcTimeCut";
  fTitle="cut on time1, time2 & tim2-tim1";
  strcpy(detName,"Mdc");
  cutT1L=cT1L;
  cutT1R=cT1R;
  cutT2L=cT2L;
  cutT2R=cT2R;
  cutLeft=cLeft;
  cutRight=cRight;
  isContainer=kTRUE; 
}

void HMdcTimeCut::setCut(Float_t cT1L, Float_t cT1R, 
                         Float_t cT2L, Float_t cT2R,
                         Float_t cLeft, Float_t cRight) {
  cutT1L=cT1L;
  cutT1R=cT1R;
  cutT2L=cT2L;
  cutT2R=cT2R;
  cutLeft=cLeft;
  cutRight=cRight;
  isContainer=kTRUE; 
}

void HMdcTimeCut::setCutTime1(Float_t cT1L, Float_t cT1R) {
  cutT1L=cT1L;
  cutT1R=cT1R;
}

void HMdcTimeCut::setCutTime2(Float_t cT2L, Float_t cT2R) {
  cutT2L=cT2L;
  cutT2R=cT2R;
}

void HMdcTimeCut::setCutDTime21(Float_t cLeft, Float_t cRight) {
  cutLeft=cLeft;
  cutRight=cRight;
}

void HMdcTimeCut::getCut(Float_t &cT1L, Float_t &cT1R, 
                         Float_t &cT2L, Float_t &cT2R,
                         Float_t &cLeft, Float_t &cRight) {
 
  cT1L=cutT1L;
  cT1R=cutT1R;
  cT2L=cutT2L;
  cT2R=cutT2R;
  cLeft=cutLeft;
  cRight=cutRight;
}

Bool_t HMdcTimeCut::cut(HMdcCal1* cal) {
//  Int_t nTm=TMath::Abs(cal->getNHits());
  Float_t time1=cal->getTime1();
  Float_t time2=cal->getTime2();
  Float_t dTime=time2-time1;  
  if(cutT1L<cutT1R && (time1<=cutT1L || time1>cutT1R)) return kTRUE;
  if(cutT2L<cutT2R && (time2<=cutT2L || time2>cutT2R)) return kTRUE;
  if(cutLeft<cutRight && (dTime<=cutLeft || dTime>cutRight)) return kTRUE;   
  return kFALSE;
}
  









