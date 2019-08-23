#ifndef HTOFINORAW_H
#define HTOFINORAW_H

#include <TObject.h>

class HTofinoRaw : public TObject {
protected:
    Float_t time;    // tdc time
    Float_t charge;  // adc charge
    Int_t sector;    // sector number
    Int_t module;    // module number
    Int_t cell;      // paddel number
public:
    HTofinoRaw(void) { clear(); }
    ~HTofinoRaw(void) {;}
    void setTime(const Float_t v) { time=v; }
    void setCharge(const Float_t v) { charge=v; }
    void setSector(const Int_t n) { sector=n; }
    void setModule(const Int_t n) { module=n; }
    void setCell(const Int_t n)  { cell=n; }
    void setAddress(const Int_t s, const Int_t m, const Int_t c) {
      sector=s;
      module=m;
      cell=c;
    }
    void fill(const Float_t t, const Float_t a,
              const Int_t s, const Int_t m, const Int_t c) {
      time=t;
      charge=a;
      sector=s;
      module=m;
      cell=c;
    }
    Float_t getTime(void) const { return time; }
    Float_t getCharge(void) const { return charge; }
    Int_t getSector(void) const { return sector; }
    Int_t getModule(void) const { return module; }
    Int_t getCell(void) const { return cell; }
    void getAddress(Int_t& s, Int_t& m, Int_t& c) {
      s=sector;
      m=module;
      c=cell;
    }
    void clear(void) {
      time=-200.;
      charge=-200.;
      sector=-1;
      module=-1;
      cell=-1;
    }

    ClassDef(HTofinoRaw,1)  // TOFINO detector raw data
};

#endif /* ! HTOFINORAW_H */
