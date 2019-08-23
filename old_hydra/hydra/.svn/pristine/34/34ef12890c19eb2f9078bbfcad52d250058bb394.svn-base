#ifndef HSTARTCAL_H
#define HSTARTCAL_H

#include <TObject.h>
#include "hstartraw.h"
 
class HStartCal : public TObject {
protected:
    Float_t time;  // tdc time
	Float_t timeR;  // tdc right-side time - hodoscope
    Float_t timeL;  // tdc left-side time  - hodoscope
    Float_t timeM;  // meantimer time      - hardware meantimer
	Float_t fTof;
	Float_t fPos;
	Int_t nType;
	Int_t nMult[6];
	Int_t module;  // module number
    Int_t strip;   // strip number
	
public:
    HStartCal(void) { clear(); }
    ~HStartCal(void) {;}
    void setTime(const Float_t t) { time=t; }
	void setTimeR(const Float_t t) { timeR=t; }
    void setTimeL(const Float_t t) { timeL=t; }
    void setTimeM(const Float_t t) { timeM=t; }
	void setType(const  Int_t n)  { nType=n; }
	void setTof(const  Float_t  t)  { fTof=t; }
	void setPos(const  Float_t p)  { fPos=p; }
	void setMult(Int_t nMod, const  Int_t m)  { nMult[nMod] = m;}
    void setModule(const Int_t n) { module=n; }
    void setStrip(const Int_t n)    { strip=n; }
    void setAddress(const Int_t m,const Int_t s) {
      module=m;
      strip=s;
    }
    void fill(const Float_t t, const Int_t m,const Int_t s) {
      time=t;
      module=m;
      strip=s;
    }
	void fill(const Float_t t, const Int_t m,const Int_t s, Int_t n);

    Float_t getTime(void)  const { return time;  }
	Float_t getTimeR(void) const { return timeR; }
    Float_t getTimeL(void) const { return timeL; }
    Float_t getTimeM(void) const { return timeM; }
    Float_t getTof(void)   const { return fTof;  }
    Float_t getPos(void)   const { return fPos;  }
	Int_t   getType(void)  const { return nType; }
    Int_t   getMult(Int_t n)  const { return nMult[n]; }

    Int_t   getModule(void)const { return module;}
    Int_t   getStrip(void) const { return strip; }
    void getAddress(Int_t& m,Int_t& s) {
      m=module;
      s=strip;
    }
    void clear(void) {
		time=-1000;
		module=-1;
		strip=-1;
		timeR=-1000;
		timeL=-1000;
		timeM=-1000;
		fTof = -1000;
		fPos = -1000;
		for(Int_t i=0;i<6;i++){
			nMult[i] = 0;
		}
    }

    ClassDef(HStartCal,2)  // START detector cal data
};

#endif /* ! HSTARTCAL_H */











