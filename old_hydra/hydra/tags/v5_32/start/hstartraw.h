#ifndef HSTARTRAW_H
#define HSTARTRAW_H

#include "hdataobject.h"
 
class HStartRaw : public HDataObject {
protected:
    Short_t time;  // tdc time
    Int_t module;  // module number
    Int_t strip;   // strip number
public:
    HStartRaw(void) { clear(); }
    ~HStartRaw(void) {;}
    void setTime(const Short_t t) { time=t; }
    void setTime(const Int_t t) { time=(Short_t)t; }
    void setModule(const Int_t n) { module=n; }
    void setStrip(const Int_t n)  { strip=n; }
    void setAddress(const Int_t m,const Int_t s) {
      module=m;
      strip=s;
    }
    void fill(const Short_t t, const Int_t m,const Int_t s) {
      time=t;
      module=m;
      strip=s;
    }
    Short_t getTime(void) const { return time; }
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
     
    ClassDef(HStartRaw,1)  // START detector raw data
};

#endif /* ! HSTARTRAW_H */










