#ifndef HHODOHIT_H
#define HHODOHIT_H

#include "TObject.h"

class HHodoHit : public TObject {
protected:
    Float_t time;     // hodo time
    Float_t amp;      // amplitude value
    Int_t module;     // module number
    Int_t strip;      // strip number
    Int_t hodoMult;  // Hodo det. Multip. 
    Bool_t flag;      // kTRUE if true hodo time has been found
public:
    HHodoHit(void) { clear(); }
    ~HHodoHit(void) {;}
    void setTime(const Float_t t) { time=t; }
    void setAmp(const Float_t a) { amp=a; }
    void setModule(const Int_t n) { module=n; }
    void setStrip(const Int_t n)    { strip=n; }
    void setFlag(Bool_t sflag)  {flag=sflag;}
    void setHodoMult(Int_t mult) {hodoMult=mult;}
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
    Float_t getAmp(void) const { return amp; }
    Int_t getModule(void) const { return module; }
    Int_t getStrip(void) const { return strip; }
    Int_t getHodoMult(void) const { return hodoMult; }
    Bool_t getFlag(void) {return flag;}
    void getAddress(Int_t& m,Int_t& s) {
      m=module;
      s=strip;
    }
    void clear(void) {
      time=-1.;
      amp=-1.;
      module=-1;
      strip=-1;
    }
    ClassDef(HHodoHit,2)  // HODO detector hit data
};

#endif /* ! HHODOHIT_H */
