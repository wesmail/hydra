#ifndef HTOFINORAW_H
#define HTOFINORAW_H

#include "hdataobject.h"

class HTofinoRaw : public HDataObject {
protected:
    Short_t time;    // tdc time
    Short_t charge;  // adc charge
    Int_t sector;    // sector number
    Int_t module;    // module number
    Int_t cell;      // paddel number
public:
    HTofinoRaw(void) { clear(); }
    ~HTofinoRaw(void) {;}
    void setTime(const Short_t v) { time=v; }
    void setCharge(const Short_t v) { charge=v; }
    void setSector(const Int_t n) { sector=n; }
    void setModule(const Int_t n) { module=n; }
    void setCell(const Int_t n)  { cell=n; }
    void setAddress(const Int_t s, const Int_t m, const Int_t c) {
      sector=s;
      module=m;
      cell=c;
    }
    void fill(const Short_t t, const Short_t a,
              const Int_t s, const Int_t m, const Int_t c) {
      time=t;
      charge=a;
      sector=s;
      module=m;
      cell=c;
    }
    Short_t getTime(void) const { return time; }
    Short_t getCharge(void) const { return charge; }
    Int_t getSector(void) const { return sector; }
    Int_t getModule(void) const { return module; }
    Int_t getCell(void) const { return cell; }
    void getAddress(Int_t& s, Int_t& m, Int_t& c) {
      s=sector;
      m=module;
      c=cell;
    }
    void clear(void) {
      time=-2;
      charge=-2;
      sector=-1;
      module=-1;
      cell=-1;
    }

    ClassDef(HTofinoRaw,1)  // TOFINO detector raw data
};

#endif /* ! HTOFINORAW_H */
