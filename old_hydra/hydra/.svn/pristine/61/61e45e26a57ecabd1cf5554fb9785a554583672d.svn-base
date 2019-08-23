#ifndef HMDCTIMECUT_H
#define HMDCTIMECUT_H

#include "hparset.h"
#include "hmdccal1.h"
#include "hmessagemgr.h"
#include <stdio.h>
#include <TObjArray.h>

class HMdcDetector;

class HMdcTimeCutMod : public TObject {
 protected:
  Float_t cutT1L;       // Cut 1:  cutT1L<time1<cutT1R
  Float_t cutT1R;       // cutT1L>=cutT1R - no cut
  Float_t cutT2L;       // Cut 2:  cutT2L<time2<cutT2R
  Float_t cutT2R;       // cutT2L>=cutT2R - no cut
  Float_t cutLeft;      // Cut 3: cutLeft<(time2-time1)<cutRight
  Float_t cutRight;     // cutLeft>=cutRight - no cut
public:
  HMdcTimeCutMod() { clear(); }
  HMdcTimeCutMod(HMdcTimeCutMod &tc) {fill(tc);}
  ~HMdcTimeCutMod() {}

  void setCutTime1(Float_t left,Float_t right) {
    cutT1L = left;
    cutT1R = right;
  }
  void setCutTime2(Float_t left,Float_t right) { 
    cutT2L = left;
    cutT2R = right;
  }
  void setCutTime(Float_t left,Float_t right) { 
    cutLeft = left;
    cutRight = right;
  } 

  Float_t getCutTime1Left(void) { return cutT1L; }
  Float_t getCutTime1Right(void){ return cutT1R; }  
  Float_t getCutTime2Left(void) { return cutT2L; }   
  Float_t getCutTime2Right(void){ return cutT2R; }  
  Float_t getCutTimeLeft(void)  { return cutLeft; }    
  Float_t getCutTimeRight(void) { return cutRight; }   
  
  void fill(HMdcTimeCutMod &tc) {
    cutT1L = tc.cutT1L; 
    cutT1R = tc.cutT1R; 
    cutT2L = tc.cutT2L; 
    cutT2R = tc.cutT2R; 
    cutLeft = tc.cutLeft; 
    cutRight = tc.cutRight;
  }
  void fill(Float_t cT1L,Float_t cT1R,Float_t cT2L,Float_t cT2R,Float_t cLeft,
	    Float_t cRight) {
    cutT1L = cT1L; 
    cutT1R = cT1R; 
    cutT2L = cT2L; 
    cutT2R = cT2R; 
    cutLeft = cLeft; 
    cutRight = cRight;
  }
  Bool_t cutTime1(Float_t time1) {
    return (cutT1L < cutT1R && (time1<=cutT1L || time1>cutT1R));
  }
  Bool_t cutTime2(Float_t time2) {
    return (cutT2L<cutT2R && (time2<=cutT2L || time2>cutT2R));
  }
  Bool_t cutTimesDif(Float_t dTime) {
    return (cutLeft<cutRight && (dTime<=cutLeft || dTime>cutRight));
  }
  Bool_t cutComStop(Float_t time1) {
    return (cutT1L<cutT1R && time1>cutT1R);
  }

  void clear(void) {
    fill(0., 0., 0., 0., 0., 0. );
  }
  void print(void) {
      gHades->getMsg()->info(10,HMessageMgr::DET_MDC,GetName()
			     ,"time1: (%f, %f), time1: (%f, %f), tab: (%f, %f)\n"
			     , cutT1L,cutT1R,cutT2L,cutT2R,cutLeft,cutRight);
  }
  ClassDef(HMdcTimeCutMod,1)
};

class HMdcTimeCutSec : public TObject {
 protected:
  TObjArray fMod;
 public:
  HMdcTimeCutSec(Int_t mods=4) : fMod(mods) {
    for (Int_t i=0; i<mods; i++) fMod.AddAt(new HMdcTimeCutMod,i);
  }
  ~HMdcTimeCutSec(void) {}
  HMdcTimeCutMod& operator[](Int_t i) {
    return *( static_cast<HMdcTimeCutMod*>(fMod[i]) );
  }
  Int_t getSize() {return fMod.GetEntries();}
  ClassDef(HMdcTimeCutSec,1)
};

class HMdcTimeCut : public HParSet {
protected:
  TObjArray fSecs; // Sectors containins modules containing cuts
  Bool_t isContainer;  
  void setNameTitle();
  HMdcDetector *fMdc; 
  Bool_t isused;  // flag for used/not used
public:
  HMdcTimeCut(const Char_t* name="MdcTimeCut",
              const Char_t* title="cut on time1, time2 & time2-time1",
              const Char_t* context="MdcTimeCutProduction",
              Int_t secs=6,Int_t mods=4);
  ~HMdcTimeCut(void) {};
  void   setIsUsed(Bool_t flag)    {isused=flag;}
  Bool_t getIsUsed()             {return isused;}

  static HMdcTimeCut* getExObject();  // if HMdcTimeCut not existed = 0 
  static HMdcTimeCut* getObject();    // if HMdcTimeCut not existed it will cr.

  Bool_t cut(HMdcCal1* cal);          // if =kTRUE don't use this hit
  Bool_t cutComStop(HMdcCal1* cal);
  Bool_t cutTime1(HMdcCal1* cal);
  Bool_t cutTime1(Int_t,Int_t,Float_t);

  Bool_t cutTime2(HMdcCal1* cal);
  Bool_t cutTime2(Int_t,Int_t,Float_t);

  Bool_t cutTimesDif(HMdcCal1* cal);
  Bool_t cutTimesDif(Int_t,Int_t,Float_t,Float_t);

  void setCut(Float_t cT1L, Float_t cT1R, Float_t cT2L, Float_t cT2R,
              Float_t cLeft, Float_t cRight);
  void setCut(Float_t* cT1L, Float_t* cT1R, Float_t* cT2L, Float_t* cT2R,
              Float_t* cLeft, Float_t* cRight);
  void setCutTime1(Float_t cT1L, Float_t cT1R);
  void setCutTime2(Float_t cT2L, Float_t cT2R);
  void setCutDTime21(Float_t cLeft, Float_t cRight);
  void setCut(Int_t mod, Float_t cT1L, Float_t cT1R, 
      Float_t cT2L, Float_t cT2R, Float_t cLeft, Float_t cRight);
  void setCutTime1(Int_t mod, Float_t cT1L, Float_t cT1R);
  void setCutTime2(Int_t mod, Float_t cT2L, Float_t cT2R);
  void setCutDTime21(Int_t mod, Float_t cLeft, Float_t cRight);
  void getCut(Int_t s,Int_t mod, Float_t &cT1L, Float_t &cT1R, 
      Float_t &cT2L, Float_t &cT2R, Float_t &cLeft, Float_t &cRight);

  Float_t  getCutT1L(Int_t sec, Int_t mod) {
    return (*this)[sec][mod].getCutTime1Left();
  }
  Float_t  getCutT1R(Int_t sec, Int_t mod) {
    return (*this)[sec][mod].getCutTime1Right();
  }
  Float_t  getCutT2L(Int_t sec, Int_t mod) {
    return (*this)[sec][mod].getCutTime2Left();
  }
  Float_t  getCutT2R(Int_t sec, Int_t mod) {
    return (*this)[sec][mod].getCutTime2Right();
  }
  Float_t  getCutDTime21L(Int_t sec, Int_t mod) {
    return (*this)[sec][mod].getCutTimeLeft();
  }
  Float_t  getCutDTime21R(Int_t sec, Int_t mod) {
    return (*this)[sec][mod].getCutTimeRight();
  } 

  Bool_t init(HParIo* input,Int_t* set);
  Int_t write(HParIo* output) ;
  void readline(const Char_t* buf, Int_t* set) ;
  void putAsciiHeader(TString&);
  Bool_t writeline(Char_t*, Int_t, Int_t);

  HMdcTimeCutSec& operator[](Int_t i) {
    return *( static_cast<HMdcTimeCutSec*>(fSecs[i]) );
  }
  Int_t getSize() {return fSecs.GetEntries();}
  void clear(void);
  void printParam(void) {
      INFO_msg(10,HMessageMgr::DET_MDC,"TimeCut:\n");
      for (Int_t i=0;i<getSize();i++) {
	  HMdcTimeCutSec &sector = (*this)[i];
	  for (Int_t m=0; m<sector.getSize(); m++) {
	      HMdcTimeCutMod &mod = sector[m];
	      gHades->getMsg()->info(10,HMessageMgr::DET_MDC,GetName(),"%i %i ",i,m);
	      mod.print();
	  }
      }
  }

  ClassDef(HMdcTimeCut,1) 
};

#endif  /*!HMDCTIMECUT_H*/ 
