#ifndef HSTARTHIT_H
#define HSTARTHIT_H

#include <TObject.h>

class HStartHit : public TObject {
protected:
    Float_t time;     // start time
    Int_t module;     // module number
    Int_t strip;      // strip number
    Int_t startMult;  // Start det. Multip. 
    Bool_t flag;      // kTRUE if true start time has been found
public:
    HStartHit(void) { clear(); }
    ~HStartHit(void) {;}
    void setTime(const Float_t t) { time=t; }
    void setModule(const Int_t n) { module=n; }
    void setStrip(const Int_t n)    { strip=n; }
    void setFlag(Bool_t sflag)  {flag=sflag;}
    void setStartMult(Int_t mult) {startMult=mult;}
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
    Int_t getStartMult(void) const { return startMult; }
    Bool_t getFlag(void) {return flag;}
    void getAddress(Int_t& m,Int_t& s) {
      m=module;
      s=strip;
    }
    void clear(void) {
      time=-1;
      module=-1;
      strip=-1;
    }
    ClassDef(HStartHit,2)  // START detector hit data
};

#endif /* ! HSTARTHIT_H */
