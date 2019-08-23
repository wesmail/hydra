#ifndef HMDCRAW_H
#define HMDCRAW_H

#include "hdataobject.h"

class HMdcRaw : public HDataObject {
protected:
    Int_t nHits;  // number of hits / Tdc mode
    Int_t time1;  // time of 1st hit
    Int_t time2;  // time of 2nd hit
    Int_t time3;  // time of 3rd hit
    Int_t time4;  // time of 4th hit
    Int_t time5;  // time of 5th hit
    Int_t time6;  // time of 6th hit
    Int_t sector; // sector number
    Int_t module; // module number
    Int_t mbo;    // motherboard number
    Int_t tdc;    // tdc number * 8 + tdc channel number

public:
    HMdcRaw(void) { clear(); }
    ~HMdcRaw(void){;}
    void clear(void) {
      nHits=0;
      sector=module=mbo=tdc=-1;
      time1=time2=time3=time4=time5=time6=-99;
    }
    void setSector(const Int_t n) { sector=n; }
    void setModule(const Int_t n) { module=n; }
    void setMbo(const Int_t n)    { mbo=n; }
    void setTdc(const Int_t n)    { tdc=n; }
    void setAddress(const Int_t s,const Int_t m,const Int_t l,const Int_t c) {
      sector=s;
      module=m;
      mbo=l;
      tdc=c;
    }
    void setTime(const Int_t,const Int_t mode=0);

    Int_t getSector(void) const { return sector; }
    Int_t getModule(void) const { return module; }
    Int_t getMbo(void) const    { return mbo; }
    Int_t getTdc(void) const    { return tdc; }
    void getAddress(Int_t& s,Int_t& m,Int_t& l,Int_t& c) {
      s=sector;
      m=module;
      l=mbo;
      c=tdc;
    }
    Int_t getNHits(void) const  { return nHits; }
    Int_t getTime(const Int_t n) const;

    ClassDef(HMdcRaw,1) // Raw hit on a MDC
};

#endif /* !HMDCRAW_H */
