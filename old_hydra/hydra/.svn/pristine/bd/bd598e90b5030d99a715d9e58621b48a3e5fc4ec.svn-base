//*-- AUTHOR : Vladimir Pechenov
//*-- Modified : 10/05/2001
//*-- Modified : 24/05/2000

///////////////////////////////////////////////////////////////
//HMdcTimeCutD
//
//  class for cut time1, time2 and time2-time1
//  used for beam data only (time1: leading edge, time2
//  trailing edge, see http://www-hades.gsi.de/docs/mdc/mdcana).
//
//  Creating and deleting of container.
//    HMdcTimeCutD* fCut=HMdcTimeCutD::getObject();
//    HMdcTimeCutD::deleteCont();
//    Static function getObject return the pointer to
//      HMdcTimeCutD object. If object is not existing
//      it will created.
//    Static function deleteCont() delete the object HMdcTimeCutD
//      if it exist.
//
//  Static function getExObject return the pointer to
//  HMdcTimeCutD object. If pointer=0, the object
//  not exist:
//  HMdcTimeCutD* fCut=HMdcTimeCutD::getExObject();
//
//  Using: create the object HMdcTimeCutD in a macros
//  and set the edges for a cut (see hmdctimecutd.h)
//  Default seting - cut is absent (all edges = 0).
//
//  If you don't need in a cut don't create this object.
//
//  cT1L[module],cT1R[module] - cut for time1
//  cT2L[module],cT2R[module] - cut for time2
//  cLeft[module],cRight[module] - cut for time2-time1
//
//  Member fuctions:
//  setCut(Float_t cT1L, Float_t cT1R, Float_t cT2L, Float_t cT2R,
//              Float_t cLeft, Float_t cRight);
//    seting identical cuts for all modules
//  setCut(Int_t mod, Float_t cT1L, Float_t cT1R, Float_t cT2L, Float_t cT2R,
//              Float_t cLeft, Float_t cRight);
//    seting cuts for module mod.
//  ...
//
//  Examples are in mdcEfficiency.C
//
//////////////////////////////////////////////////////////////

#include "hmdctimecutd.h"
#include "hades.h"
#include "hruntimedb.h"

ClassImp(HMdcTimeCutD) 

HMdcTimeCutD* HMdcTimeCutD::fMdcTimeCutD=0;

HMdcTimeCutD::HMdcTimeCutD(): HParSet() {
  fName="MdcTimeCutD";
  fTitle="cut on time1, time2 & tim2-tim1";
  strcpy(detName,"Mdc");
  setCut(0.,0.,0.,0.,0.,0.);
}

HMdcTimeCutD* HMdcTimeCutD::getExObject() {
  return fMdcTimeCutD;
}

HMdcTimeCutD* HMdcTimeCutD::getObject() {
  if(!fMdcTimeCutD) fMdcTimeCutD=new HMdcTimeCutD();
  return fMdcTimeCutD;
}

void HMdcTimeCutD::deleteCont() {
  if(fMdcTimeCutD) delete fMdcTimeCutD;
  fMdcTimeCutD=0;
}

void HMdcTimeCutD::setCut(Float_t cT1L, Float_t cT1R, 
                         Float_t cT2L, Float_t cT2R,
                         Float_t cLeft, Float_t cRight) {
  for(Int_t mod=0;mod<4;mod++) {
    cutT1L[mod]=cT1L;
    cutT1R[mod]=cT1R;
    cutT2L[mod]=cT2L;
    cutT2R[mod]=cT2R;
    cutLeft[mod]=cLeft;
    cutRight[mod]=cRight; 
  }
  isContainer=kTRUE;
}

void HMdcTimeCutD::setCut(Float_t* cT1L, Float_t* cT1R, 
                         Float_t* cT2L, Float_t* cT2R,
                         Float_t* cLeft, Float_t* cRight) {
  for(Int_t mod=0;mod<4;mod++) {
    cutT1L[mod]=cT1L[mod];
    cutT1R[mod]=cT1R[mod];
    cutT2L[mod]=cT2L[mod];
    cutT2R[mod]=cT2R[mod];
    cutLeft[mod]=cLeft[mod];
    cutRight[mod]=cRight[mod]; 
  }
  isContainer=kTRUE;
}

void HMdcTimeCutD::setCutTime1(Float_t cT1L, Float_t cT1R) {
  for(Int_t mod=0;mod<4;mod++) {
    cutT1L[mod]=cT1L;
    cutT1R[mod]=cT1R;
  }
}

void HMdcTimeCutD::setCutTime2(Float_t cT2L, Float_t cT2R) {
  for(Int_t mod=0;mod<4;mod++) {
    cutT2L[mod]=cT2L;
    cutT2R[mod]=cT2R;
  }
}

void HMdcTimeCutD::setCutDTime21(Float_t cLeft, Float_t cRight) {
  for(Int_t mod=0;mod<4;mod++) {
    cutLeft[mod]=cLeft;
    cutRight[mod]=cRight;
  }
}

void HMdcTimeCutD::setCut(Int_t mod, Float_t cT1L, Float_t cT1R, 
                         Float_t cT2L, Float_t cT2R,
                         Float_t cLeft, Float_t cRight) {
  if(mod<0 || mod>3) return;
  cutT1L[mod]=cT1L;
  cutT1R[mod]=cT1R;
  cutT2L[mod]=cT2L;
  cutT2R[mod]=cT2R;
  cutLeft[mod]=cLeft;
  cutRight[mod]=cRight; 
}

void HMdcTimeCutD::setCutTime1(Int_t mod, Float_t cT1L, Float_t cT1R) {
  if(mod<0 || mod>3) return;
  cutT1L[mod]=cT1L;
  cutT1R[mod]=cT1R;
}

void HMdcTimeCutD::setCutTime2(Int_t mod, Float_t cT2L, Float_t cT2R) {
  if(mod<0 || mod>3) return;
  cutT2L[mod]=cT2L;
  cutT2R[mod]=cT2R;
}

void HMdcTimeCutD::setCutDTime21(Int_t mod, Float_t cLeft, Float_t cRight) {
  if(mod<0 || mod>3) return;
  cutLeft[mod]=cLeft;
  cutRight[mod]=cRight;
}

void HMdcTimeCutD::getCut(Int_t mod, Float_t &cT1L, Float_t &cT1R, 
                         Float_t &cT2L, Float_t &cT2R,
                         Float_t &cLeft, Float_t &cRight) { 
  cT1L=cutT1L[mod];
  cT1R=cutT1R[mod];
  cT2L=cutT2L[mod];
  cT2R=cutT2R[mod];
  cLeft=cutLeft[mod];
  cRight=cutRight[mod];
}

Bool_t HMdcTimeCutD::cut(HMdcCal1* cal) {
  Float_t time1=cal->getTime1();
  Float_t time2=cal->getTime2();
  Float_t dTime=time2-time1;
  Int_t mod=cal->getModule(); 
  if(cutT1L[mod]<cutT1R[mod] && (time1<=cutT1L[mod] || time1>cutT1R[mod]))
    return kTRUE;
  if(cutT2L[mod]<cutT2R[mod] && (time2<=cutT2L[mod] || time2>cutT2R[mod]))
    return kTRUE;
  if(cutLeft[mod]<cutRight[mod] && (dTime<=cutLeft[mod] || dTime>cutRight[mod]))
    return kTRUE;   
  return kFALSE;
}

Bool_t HMdcTimeCutD::cutTime1(HMdcCal1* cal) {
  Float_t time1=cal->getTime1(); 
  Int_t mod=cal->getModule(); 
  return (cutT1L[mod]<cutT1R[mod] && 
      (time1<=cutT1L[mod] || time1>cutT1R[mod])) ? kTRUE:kFALSE;
}

Bool_t HMdcTimeCutD::cutTime2(HMdcCal1* cal) {
  Float_t time2=cal->getTime2();
  Int_t mod=cal->getModule(); 
  return (cutT2L[mod]<cutT2R[mod] && 
      (time2<=cutT2L[mod] || time2>cutT2R[mod])) ? kTRUE:kFALSE;
}

Bool_t HMdcTimeCutD::cutTimesDif(HMdcCal1* cal) {
  Float_t dTime=cal->getTime2()-cal->getTime1();
  Int_t mod=cal->getModule(); 
  return (cutLeft[mod]<cutRight[mod] && 
      (dTime<=cutLeft[mod]||dTime>cutRight[mod])) ? kTRUE:kFALSE;
}

Bool_t HMdcTimeCutD::cutComStop(HMdcCal1* cal) {
  Float_t time1=cal->getTime1();
  Int_t mod=cal->getModule();
  return (cutT1L[mod]<cutT1R[mod] && time1>cutT1R[mod]) ? kTRUE:kFALSE;
}
