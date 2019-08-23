#ifndef HTOFINOCAL_H
#define HTOFINOCAL_H

#include "hdataobject.h"

class HTofinoCal : public HDataObject {
protected:
    Float_t timeCal;    // tdc time
    Float_t chargeCal;  // adc charge
    Int_t sector;    // sector number
    Int_t module;    // module number
    Int_t cell;      // paddle number
public:
    HTofinoCal(void);
    ~HTofinoCal(void) {;}

    void setTime(const Float_t v) { timeCal=v; }
    void setCharge(const Float_t v) { chargeCal=v; }
    void setSector(const Int_t n) { sector=n; }
    void setModule(const Int_t n) { module=n; }
    void setCell(const Int_t n)  { cell=n; }
    void setAddress(const Int_t s, const Int_t m, const Int_t c);

    void clear(void);
    void fill(const Float_t t, const Float_t a,
              const Int_t s, const Int_t m, const Int_t c);

    Float_t getTime(void) const { return timeCal; }
    Float_t getCharge(void) const { return chargeCal; }
    Int_t getSector(void) const { return sector; }
    Int_t getModule(void) const { return module; }
    Int_t getCell(void) const { return cell; }
    void getAddress(Int_t& s, Int_t& m, Int_t& c);

    ClassDef(HTofinoCal,1)  // TOFINO detector calibrated data
};

#endif /* ! HTOFINOCAL_H */
