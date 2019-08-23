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
#include "hmdcparasciifileio.h"
#include "hpario.h"
#include "hmdcdetector.h"
#include "hspectrometer.h"


HMdcTimeCut::HMdcTimeCut(Int_t s,Int_t m) : fSecs(s) {
  for (Int_t i=0;i<s; i++) fSecs.AddAt(new HMdcTimeCutSec(m),i);
  setNameTitle();
  if (gHades) {
    fMdc = (HMdcDetector*)gHades->getSetup()->getDetector("Mdc");
  } else {
    fMdc = 0;
  }
}

void HMdcTimeCut::clear(void) {
  setCut(0.,0.,0.,0.,0.,0.);
}

void HMdcTimeCut::setNameTitle() {
  fName="MdcTimeCut";
  fTitle="cut on time1, time2 & tim2-tim1";
  strcpy(detName,"Mdc");
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
  for (Int_t s=0;s<getSize();s++) {
    HMdcTimeCutSec &sector=this->operator[](s);
    
    for(Int_t mod=0;mod<sector.getSize();mod++) {
      sector[mod].fill(cT1L,cT1R,cT2L,cT2R,cLeft,cRight);
    }
  }
  isContainer=kTRUE;
}

void HMdcTimeCut::setCut(Float_t* cT1L, Float_t* cT1R, 
                         Float_t* cT2L, Float_t* cT2R,
                         Float_t* cLeft, Float_t* cRight) {

  for (Int_t s=0;s<getSize();s++) {
    HMdcTimeCutSec &sector=this->operator[](s);
    
    for(Int_t mod=0;mod<sector.getSize();mod++) {
      sector[mod].fill(cT1L[mod], cT1R[mod], cT2L[mod], cT2R[mod],
		       cLeft[mod], cRight[mod]);
    }
  }
  isContainer=kTRUE;
}

void HMdcTimeCut::setCutTime1(Float_t cT1L, Float_t cT1R) {
  for (Int_t s=0;s<getSize();s++) {
    HMdcTimeCutSec &sector=this->operator[](s);
    
    for(Int_t mod=0;mod<sector.getSize();mod++) {
      sector[mod].setCutTime1(cT1L, cT1R);
    }
  }
}

void HMdcTimeCut::setCutTime2(Float_t cT2L, Float_t cT2R) {
  for (Int_t s=0;s<getSize();s++) {
    HMdcTimeCutSec &sector=this->operator[](s);
    
    for(Int_t mod=0;mod<sector.getSize();mod++) {
      sector[mod].setCutTime2(cT2L, cT2R);
    }
  }
}

void HMdcTimeCut::setCutDTime21(Float_t cLeft, Float_t cRight) {
  for (Int_t s=0;s<getSize();s++) {
    HMdcTimeCutSec &sector=this->operator[](s);
    
    for(Int_t mod=0;mod<sector.getSize();mod++) {
      sector[mod].setCutTime(cLeft, cRight);
    }
  }
}

void HMdcTimeCut::setCut(Int_t mod, Float_t cT1L, Float_t cT1R, 
                         Float_t cT2L, Float_t cT2R,
                         Float_t cLeft, Float_t cRight) {
  for (Int_t s=0;s<getSize();s++) {
    HMdcTimeCutSec &sector=(*this)[s];
    
    if (mod>-1 && mod<sector.getSize()) {
      sector[mod].fill(cT1L,cT1R,cT2L,cT2R,cLeft,cRight);
    }
  }
}

void HMdcTimeCut::setCutTime1(Int_t mod, Float_t cT1L, Float_t cT1R) {
  for (Int_t s=0;s<getSize();s++) {
    HMdcTimeCutSec &sector=(*this)[s];
    
    if (mod>-1 && mod<sector.getSize()) {
      sector[mod].setCutTime1(cT1L,cT1R);
    }
  }
}

void HMdcTimeCut::setCutTime2(Int_t mod, Float_t cT2L, Float_t cT2R) {
  for (Int_t s=0;s<getSize();s++) {
    HMdcTimeCutSec &sector=(*this)[s];
    
    if (mod>-1 && mod<sector.getSize()) {
      sector[mod].setCutTime2(cT2L,cT2R);
    }
  }  
}

void HMdcTimeCut::setCutDTime21(Int_t mod, Float_t cLeft, Float_t cRight) {
  for (Int_t s=0;s<getSize();s++) {
    HMdcTimeCutSec &sector=(*this)[s];
    
    if (mod>-1 && mod<sector.getSize()) {
      sector[mod].setCutTime(cLeft,cRight);
    }
  } 
}

void HMdcTimeCut::getCut(Int_t s,Int_t mod, Float_t &cT1L, Float_t &cT1R, 
                         Float_t &cT2L, Float_t &cT2R,
                         Float_t &cLeft, Float_t &cRight) { 
  HMdcTimeCutMod &m = (*this)[s][mod];
  cT1L   = m.getCutTime1Left();
  cT1R   = m.getCutTime1Right();
  cT2L   = m.getCutTime2Left();
  cT2R   = m.getCutTime2Right();
  cLeft  = m.getCutTimeLeft();
  cRight = m.getCutTimeRight();
}

Bool_t HMdcTimeCut::cut(HMdcCal1* cal) {
  Float_t time1 = cal->getTime1();
  Float_t time2 = cal->getTime2();
  Float_t dTime = time2-time1;
  Int_t mod = cal->getModule(); 
  Int_t sec = cal->getSector();
  HMdcTimeCutMod &m = (*this)[sec][mod];

  if (m.cutTime1(time1) || m.cutTime2(time2) || m.cutTimesDif(dTime)) 
    return kTRUE;

  return kFALSE;
}

Bool_t HMdcTimeCut::cutTime1(HMdcCal1* cal) {
  Float_t time1=cal->getTime1(); 
  Int_t mod=cal->getModule(); 
  Int_t sec=cal->getSector();
  HMdcTimeCutMod &m = (*this)[sec][mod];

  return m.cutTime1(time1);
}

Bool_t HMdcTimeCut::cutTime1(Int_t s,Int_t m,Float_t time1) {
    Float_t mytime=time1;
    Bool_t check=kTRUE;
    HMdcTimeCutMod &mod = (*this)[s][m];
    Float_t cuttimeleft =mod.getCutTime1Left();
    Float_t cuttimeright=mod.getCutTime1Right();
    if(mytime>cuttimeleft && mytime<cuttimeright)
    {
	check=kTRUE;
    }
    else
    {
	check=kFALSE;
    }

	return check;
}

Bool_t HMdcTimeCut::cutTime2(HMdcCal1* cal) {
  Float_t time2=cal->getTime2();
  Int_t mod=cal->getModule(); 
  Int_t sec=cal->getSector();
  HMdcTimeCutMod &m = (*this)[sec][mod];

  return m.cutTime2(time2);
}

Bool_t HMdcTimeCut::cutTime2(Int_t s,Int_t m,Float_t time2) {
    Float_t mytime=time2;
    Bool_t check=kTRUE;
    HMdcTimeCutMod &mod = (*this)[s][m];
    Float_t cuttimeleft =mod.getCutTime2Left();
    Float_t cuttimeright=mod.getCutTime2Right();
    if(mytime>cuttimeleft && mytime<cuttimeright)
    {
	check=kTRUE;
    }
    else
    {
	check=kFALSE;
    }

	return check;
}
Bool_t HMdcTimeCut::cutTimesDif(HMdcCal1* cal) {
  Float_t dTime=cal->getTime2()-cal->getTime1();
  Int_t mod=cal->getModule(); 
  Int_t sec=cal->getSector();
  HMdcTimeCutMod &m = (*this)[sec][mod];

  return m.cutTimesDif(dTime);
}
Bool_t HMdcTimeCut::cutTimesDif(Int_t s,Int_t m,Float_t time1,Float_t time2) {
    Float_t mytime1=time1;
    Float_t mytime2=time2;
    Float_t mytimediff=mytime2-mytime1;

    Bool_t check=kTRUE;
    HMdcTimeCutMod &mod = (*this)[s][m];
    Float_t cuttimeleft =mod.getCutTimeLeft();
    Float_t cuttimeright=mod.getCutTimeRight();

    if(mytimediff>cuttimeleft && mytimediff<cuttimeright)
    {
	check=kTRUE;
    }
    else
    {
	check=kFALSE;
    }

	return check;
}

Bool_t HMdcTimeCut::cutComStop(HMdcCal1* cal) {
  Float_t time1=cal->getTime1();
  Int_t mod=cal->getModule();
  Int_t sec=cal->getSector();
  HMdcTimeCutMod &m = (*this)[sec][mod];

  return m.cutComStop(time1);
}

Bool_t HMdcTimeCut::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HMdcParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HMdcTimeCut::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HMdcParIo");
  if (out) return out->write(this);
  return -1;
}

void HMdcTimeCut::readline(const char* buf, Int_t* set) {
  // decodes one line read from ascii file I/O 
  Int_t sec,mod,n;
  Float_t cT1L,cT1R,cT2L,cT2R,cLeft,cRight;

  sscanf(buf,"%i%i%f%f%f%f%f%f",
	 &sec,&mod,&cT1L,&cT1R,&cT2L,&cT2R,&cLeft,&cRight);

  n = sec*4 + mod; //FIXME
  if (set[n]) {
    if (sec>-1 && sec<6 && mod>-1 && mod<4) {
      (*this)[sec][mod].fill(cT1L,cT1R,cT2L,cT2R,cLeft,cRight);
    } else {
      Error("readLine","Addres: sector_%i, module_%i not known",sec,mod);
    }
    set[n] = 999;
  }
}

void HMdcTimeCut::putAsciiHeader(TString& b) {
  b =
    "#######################################################################\n"
    "# Drift time cuts for noise reduction of the MDC\n"
    "# Format:\n"
    "# sector mod time1Left time1Right time2Left time2Right tabLeft tabRight\n"
    "#######################################################################\n";    
}

Bool_t HMdcTimeCut::writeline(char*buf, Int_t s, Int_t m) {
  Bool_t r = kTRUE;

  if (fMdc)
    if (fMdc->getModule(s,m) != 0) {
      if  (s>-1 && s<getSize()) {
	HMdcTimeCutSec &sector = (*this)[s];
	if (m>-1 && m<sector.getSize()) {
	  sprintf(buf,"%i %i %6.0f %6.0f %6.0f %6.0f %6.0f %6.0f\n",
		  s,m,
		  (*this)[s][m].getCutTime1Left(), 
		  (*this)[s][m].getCutTime1Right(),
		  (*this)[s][m].getCutTime2Left(), 
		  (*this)[s][m].getCutTime2Right(),
		  (*this)[s][m].getCutTimeLeft(),  
		  (*this)[s][m].getCutTimeRight());
	  
	} else r = kFALSE;
      } else r = kFALSE;
    } else strcpy(buf,"");
  
  return r;
}

ClassImp(HMdcTimeCut) 
ClassImp(HMdcTimeCutMod)
ClassImp(HMdcTimeCutSec)
