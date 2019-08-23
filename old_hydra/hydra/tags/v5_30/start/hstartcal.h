#ifndef HSTARTCAL_H
#define HSTARTCAL_H

#include "hdataobject.h"
 
class HStartCal : public HDataObject {
protected:
    Float_t time;  // tdc time
    Int_t module;  // module number
    Int_t strip;   // strip number
public:
    HStartCal(void) { clear(); }
    ~HStartCal(void) {;}
    void setTime(const Short_t t) { time=t; }
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
    Float_t getTime(void) const { return time; }
    Int_t getModule(void) const { return module; }
    Int_t getStrip(void) const { return strip; }
    void getAddress(Int_t& m,Int_t& s) {
      m=module;
      s=strip;
    }
    void clear(void) {
      time=-1;
      module=-1;
      strip=-1;
    }

    ClassDef(HStartCal,1)  // START detector cal data
};

#endif /* ! HSTARTCAL_H */











