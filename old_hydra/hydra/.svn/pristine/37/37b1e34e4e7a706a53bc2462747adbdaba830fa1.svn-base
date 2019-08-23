#include "TError.h"
#include "hmdctrackdset.h"
#include "hmdctrackfinder.h"
#include "hmdcclustertohit.h"
#include "hmdc12fit.h"
#include <iostream.h>

//*-- Author : Vladimir Pechenov

// HMdcTrackFinder param.:
HMdcTrackFinder* HMdcTrackDSet::pTrackFinder=0;
Bool_t HMdcTrackDSet::isCoiloff=kFALSE;
Int_t  HMdcTrackDSet::clFinderType=0; // =0 - segment finder, =1 - mdc finder
Int_t  HMdcTrackDSet::level4[2]={10,10};
Int_t  HMdcTrackDSet::level5[2]={50,30};
Int_t  HMdcTrackDSet::nLayersInMod[6][4]=
    {{6,6,6,6},{6,6,6,6},{6,6,6,6},{6,6,6,6},{6,6,6,6},{6,6,6,6}};

// HMdcClusterToHit param.:
HMdcClusterToHit* HMdcTrackDSet::pClusterToHit=0;
Int_t HMdcTrackDSet::modForSeg1=1;
Int_t HMdcTrackDSet::modForSeg2=3;

// HMdc12Fit param.:
HMdc12Fit* HMdcTrackDSet::p12Fit=0;
Int_t HMdcTrackDSet::fitAuthor=1;     // 0 - Hejdar's fit, 1 - Alexander's fit
Int_t HMdcTrackDSet::distTimeVer=1;   // 0-calc.time by pol., 1-J.Market's func.
Int_t HMdcTrackDSet::fitType=0;       // 0 - segment fit, 1 - mdc fit
Bool_t HMdcTrackDSet::fntuple=kFALSE; // kTRUE - fill debug. inf.
Bool_t HMdcTrackDSet::fprint=kFALSE;  // kTRUE - debug. print.

Bool_t HMdcTrackDSet::isSetCall=kFALSE;

HReconstructor* HMdcTrackDSet::hMdcTrackFinder(char* name, char* title) {
  if(pTrackFinder) exitSetup("hMdcTrackFinder","HMdcTrackFinder");
  isSetCall=kTRUE;
  pTrackFinder=new HMdcTrackFinder(name,title);
  isSetCall=kFALSE;
  return pTrackFinder;
}

HReconstructor* HMdcTrackDSet::hMdcTrackFinder(char* name, char* title, 
    Bool_t coil,Int_t type) {
  setMdcTrackFinder(coil,type);
  return hMdcTrackFinder(name,title);
}
    
HReconstructor* HMdcTrackDSet::hMdcClusterToHit(char* name, char* title) {
  if(pClusterToHit)  exitSetup("hMdcClusterToHit","HMdcClusterToHit");
  isSetCall=kTRUE;
  pClusterToHit=new HMdcClusterToHit(name,title);
  isSetCall=kFALSE;
  return pClusterToHit;
}

HReconstructor* HMdcTrackDSet::hMdcClusterToHit(char* name, char* title,
    Int_t mSeg1, Int_t mSeg2) {
  setMdcClusterToHit(mSeg1,mSeg2);
  return hMdcClusterToHit(name,title);
}

HReconstructor* HMdcTrackDSet::hMdc12Fit(char* name, char* title) {
  if(p12Fit) exitSetup("hMdc12Fit","HMdc12Fit");
  isSetCall=kTRUE;
  p12Fit=new HMdc12Fit(name,title);
  isSetCall=kFALSE;
  return p12Fit;
}
    
HReconstructor* HMdcTrackDSet::hMdc12Fit(char* name, char* title, 
    Int_t author, Int_t version, Int_t indep, Bool_t nTuple, Bool_t prnt) {
  setMdc12Fit(author,version,indep,nTuple,prnt);
  return hMdc12Fit(name,title);
}

void HMdcTrackDSet::setMdcTrackFinder(Bool_t coilSt,Int_t type) {
  if(isSetCall) return;
  setCoilStatus(coilSt);
  setClFinderType(type);
}

void HMdcTrackDSet::setClFinderType(Int_t type) {
  if(isSetCall) return;
  if(type<0 || type>1) exitSetup("setClFinderType(Int_t):",1,"0 or 1",type);
  clFinderType=type;
}

void HMdcTrackDSet::setTrFnLevel(Int_t l4s1,Int_t l5s1,Int_t l4s2,Int_t l5s2) {
  if(isSetCall) return;
  setTrFnSegLevel(0,l4s1,l5s1);
  setTrFnSegLevel(1,l4s2,l5s2);
}

void HMdcTrackDSet::setTrFnSegLevel(Int_t seg, Int_t l4, Int_t l5) {
  if(isSetCall) return;
  if(seg<0||seg>1) 
    exitSetup("setTrFnSegLevel(Int_t,Int_t,Int_t):",1,"0 or 1",seg);
  level4[seg]=l4;
  level5[seg]=l5;
}

void HMdcTrackDSet::setTrFnNLayers(const Int_t* lst) {
  if(isSetCall) return;
  if(!lst) {
    Error("setTrFnNLayers(const Int_t*)"," lst-pointer = 0");
    exit(1);
  }
  for(Int_t s=0;s<6;s++) 
    for(Int_t m=0;m<4;m++) setTrFnNLayers(s,m,lst[s*4+m]);
}

void HMdcTrackDSet::setTrFnNLayers(Int_t s, Int_t m,Int_t nl) {
  if(isSetCall) return;
  if(s<0 || s>5) exitSetup("setTrFnNLayers(Int_t,Int_t,Int_t):",1,"0,...,5",s);
  if(m<0 || m>3) exitSetup("setTrFnNLayers(Int_t,Int_t,Int_t):",2,"0,...,3",m);
  if(nl<0||nl>6) exitSetup("setTrFnNLayers(Int_t,Int_t,Int_t):",3,"0,...,6",nl);
  nLayersInMod[s][m]=nl;
}

void HMdcTrackDSet::setMdcClusterToHit(Int_t mSeg1,Int_t mSeg2) {
  if(isSetCall) return;
  if(mSeg1<-1 || mSeg1>1) 
    exitSetup("setMdcClusterToHit(Int_t,Int_t):",1,"-1, 0 or 1",mSeg1);
  modForSeg1=mSeg1;
  if(mSeg2!=-1 && mSeg2!=2 && mSeg2!=3) 
    exitSetup("setMdcClusterToHit(Int_t,Int_t):",2,"-1, 2 or 3",mSeg2);
  modForSeg2=mSeg2;
}

void HMdcTrackDSet::setMdc12Fit(Int_t author, Int_t version, Int_t indep, 
        Bool_t nTuple, Bool_t prnt) {
  if(isSetCall) return;
  setFitAuthor(author);
  setFitVersion(version);
  setFitType(indep);
  setFitOutput(nTuple,prnt);
}

void HMdcTrackDSet::setFitAuthor(Int_t author) {
  if(isSetCall) return;
  if(author<0 || author>1) exitSetup("setFitAuthor(Int_t):",1,"0 or 1",author);
  fitAuthor=author; 
}

void HMdcTrackDSet::setFitVersion(Int_t ver) {
  if(isSetCall) return;
  if(ver<0||ver>1) exitSetup("setFitVersion(Int_t):",1,"0 or 1",ver);
  distTimeVer=ver;
}

void HMdcTrackDSet::setFitType(Int_t type) {
  if(isSetCall) return;
  if(type<0 || type>1) exitSetup("setFitType(Int_t):",1,"0 or 1",type);
  fitType=type;
}

void HMdcTrackDSet::setFitOutput(Bool_t nTuple, Bool_t prnt) {
  if(isSetCall) return;
  fntuple=nTuple;
  fprint=prnt;
}

void HMdcTrackDSet::exitSetup(char* funNm, Int_t npar, char* use, Int_t inst) {
  Error(funNm,"\n unknown argument %i: use %s instead %i",npar,use,inst);
  exit(1);
}

void HMdcTrackDSet::getTrFnLevels(Int_t& l4s1,Int_t& l5s1,Int_t& l4s2,Int_t& l5s2) {
  l4s1=level4[0];
  l4s2=level4[1];
  l5s1=level5[0];
  l5s2=level5[1];
}

void HMdcTrackDSet::printStatus(void) {
  if(pTrackFinder) pTrackFinder->printStatus();
  if(pClusterToHit) pClusterToHit->printStatus();
  if(p12Fit) p12Fit->printStatus();
}

void HMdcTrackDSet::setTrackFinder(HMdcTrackFinder* tr) {
  if(isSetCall) return;
  if(pTrackFinder) exitSetup("setTrackFinder","HMdcTrackFinder");
  pTrackFinder=tr;
}

void HMdcTrackDSet::setClusterToHit(HMdcClusterToHit* cl) {
  if(isSetCall) return;
  if(pClusterToHit) exitSetup("setClusterToHit","HMdcClusterToHit");
  pClusterToHit=cl;
}

void HMdcTrackDSet::set12Fit(HMdc12Fit* fit) {
  if(isSetCall) return;
  if(p12Fit) exitSetup("set12Fit","HMdc12Fit");
  p12Fit=fit;
}

void HMdcTrackDSet::exitSetup(char* funNm,char* className) {
  Error(funNm,"%s object exist already.",className);
  exit(1);
}

ClassImp(HMdcTrackDSet)
