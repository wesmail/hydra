#ifndef HWALLHIT_H
#define HWALLHIT_H

#include <TObject.h>

//F. Krizek 11.8.2005

class HWallHit : public TObject {
protected:
    Float_t timeCal;    // tdc time
    Float_t chargeCal;  // adc charge  
    Int_t cell;      // paddle number
    // Int_t index;
public:
    HWallHit(void);
    ~HWallHit(void) {;}

    void setTime(const Float_t v)   { timeCal=v; }
    void setCharge(const Float_t v) { chargeCal=v; }
    void setCell(const Int_t n)     { cell=n; }
    void setAddress(const Int_t c);
    //void setIndex(Int_t i)    { index=i; } //FK//    

    void clear(void);
    //void fill(const Float_t t, const Float_t a, const Int_t c, Int_t i);
    void fill(const Float_t t, const Float_t a, const Int_t c);

    Float_t getTime(void) const { return timeCal; }
    Float_t getCharge(void) const { return chargeCal; }
    Int_t getCell(void) const { return cell; }
    void getAddress(Int_t& c);
    //Int_t getIndex(void){ return index; } //FK//

    ClassDef(HWallHit,1)  // WALL detector calibrated data
};

#endif /* ! HWALLHIT_H */
