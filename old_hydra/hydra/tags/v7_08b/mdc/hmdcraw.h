#ifndef HMDCRAW_H
#define HMDCRAW_H

using namespace std;

#include "TObject.h"
#include <cstdlib>
#include <iostream>

class HMdcRaw : public TObject {
protected:
    Int_t nHits;  // number of hits / Tdc mode
    Int_t time1;  // time of 1st hit [binnumber]
    Int_t time2;  // time of 2nd hit [binnumber]
    Int_t time3;  // time of 3rd hit [binnumber]
    Int_t time4;  // time of 4th hit [binnumber]
    Int_t time5;  // time of 5th hit [binnumber]
    Int_t time6;  // time of 6th hit [binnumber]
    Int_t sector; // sector number   [0-5]
    Int_t module; // module number   [0-3]
    Int_t mbo;    // motherboard number [0-15]
    Int_t tdc;    // tdc number * 8 + tdc channel number [0-95]

public:
    HMdcRaw(void) { clear(); }
    ~HMdcRaw(void){;}
    void clear(void) {
      // reset data member to default values
      nHits=0;
      sector=module=mbo=tdc=-1;
      time1=time2=time3=time4=time5=time6=-999;
    }
    void setSector(const Int_t n) { sector=n; }
    void setModule(const Int_t n) { module=n; }
    void setMbo(const Int_t n)    { mbo=n; }
    void setTdc(const Int_t n)    { tdc=n; }
    void setAddress(const Int_t s,const Int_t m,const Int_t mb,const Int_t t) {
      sector=s;
      module=m;
      mbo=mb;
      tdc=t;
    }
    Bool_t setTime(const Int_t,const Int_t mode=0, const Bool_t noComment = kFALSE);

    Bool_t setTimeNew(const Int_t time, const Int_t nrtime); 

    Int_t getSector(void) const { return sector; }
    Int_t getModule(void) const { return module; }
    Int_t getMbo(void) const    { return mbo; }
    Int_t getTdc(void) const    { return tdc; }
    void getAddress(Int_t& s,Int_t& m,Int_t& mb,Int_t& t) {
      s=sector;
      m=module;
      mb=mbo;
      t=tdc;
    }
    Int_t getNHits(void) const  { return nHits; }
    Int_t getTime(const Int_t n) const;

    ClassDef(HMdcRaw,1) // Raw hit on a MDC
};

#endif /* !HMDCRAW_H */
