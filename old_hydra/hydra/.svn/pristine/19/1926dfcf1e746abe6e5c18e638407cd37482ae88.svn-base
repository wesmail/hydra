//*-- AUTHOR : Vladimir Pechenov
//*-- Modified : 24/05/2000

///////////////////////////////////////////////////////////////
//HMdcTimeCut
//
//  class for cut time1, time2 and time2-time1
//  used for beam data only (time1: leading edge, time2
//  trailing edge, see http://www-hades.gsi.de/docs/mdc/mdcana).
//
//  Using: create the object HMdcTimeCut in a macros
//  and set the edges for a cut (see hmdctimecut.h)
//  Default seting: 0<time1<200. (ns), 0<time2<200.
//  20.<time2-time1<200.
//
//  If you don't need in a cut don't create this object
//  or set all edges = 0.
//
//  Static function getExObject return the pointer to
//  HMdcTimeCut object. If pointer=0, the object
//  not exist:
//  HMdcTimeCut* fCut=HMdcTimeCut::getExObject();
//
//  Static function getObject return the pointer to
//  HMdcTimeCut object. If object is not existing
//  it will created:
//  HMdcTimeCut* fCut=HMdcTimeCut::getObject();
//
//  Examples are in mdcTrackD.C and mdcEfficiency.C
//
//////////////////////////////////////////////////////////////

#include "hmdctimecut.h"
#include "hades.h"
#include "hruntimedb.h"

ClassImp(HMdcTimeCut) 

HMdcTimeCut::HMdcTimeCut() : HParSet() {
  setCut(0.,200.,0.,200.,20.,200.);
  setNameTitle();
}

HMdcTimeCut::HMdcTimeCut(Float_t cT1L,  Float_t cT1R, 
                         Float_t cT2L,  Float_t cT2R,
                         Float_t cLeft, Float_t cRight): HParSet() {
  setCut(cT1L, cT1R, cT2L, cT2R, cLeft, cRight); 
  setNameTitle(); 
}

void HMdcTimeCut::setNameTitle() {
  fName="MdcTimeCut";
  fTitle="cut on time1, time2 & tim2-tim1";
  strcpy(detName,"Mdc");
  if(gHades) gHades->getRuntimeDb()->addContainer(this);
}

HMdcTimeCut* HMdcTimeCut::getExObject() {
  HMdcTimeCut* fcut=0;
  if( gHades ) {
    HRuntimeDb* rdb=gHades->getRuntimeDb();
    if( rdb ) fcut=(HMdcTimeCut*)rdb->getContainer("MdcTimeCut");
  }
  return fcut;
}

HMdcTimeCut* HMdcTimeCut::getObject() {
  HMdcTimeCut* fcut=0;
  if( gHades ) {
    HRuntimeDb* rdb=gHades->getRuntimeDb();
    if( rdb ) {
      fcut=(HMdcTimeCut*)rdb->getContainer("MdcTimeCut");
      if( !fcut ) fcut=new HMdcTimeCut();
    }
  }
  return fcut;
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
  Float_t time1=cal->getTime1();
  Float_t time2=cal->getTime2();
  Float_t dTime=time2-time1;  
  if(cutT1L<cutT1R && (time1<=cutT1L || time1>cutT1R)) return kTRUE;
  if(cutT2L<cutT2R && (time2<=cutT2L || time2>cutT2R)) return kTRUE;
  if(cutLeft<cutRight && (dTime<=cutLeft || dTime>cutRight)) return kTRUE;   
  return kFALSE;
}

Bool_t HMdcTimeCut::cutTime1(HMdcCal1* cal) {
  Float_t time1=cal->getTime1();  
  return (cutT1L<cutT1R && (time1<=cutT1L || time1>cutT1R)) ? kTRUE:kFALSE;
}

Bool_t HMdcTimeCut::cutTime2(HMdcCal1* cal) {
  Float_t time2=cal->getTime2();
  return (cutT2L<cutT2R && (time2<=cutT2L || time2>cutT2R)) ? kTRUE:kFALSE;
}

Bool_t HMdcTimeCut::cutTimesDif(HMdcCal1* cal) {
  Float_t dTime=cal->getTime2()-cal->getTime1();
  return (cutLeft<cutRight && (dTime<=cutLeft||dTime>cutRight)) ? kTRUE:kFALSE;
}