// Last update: 21.04.06 --- B.Spruck

#ifndef HHODOCAL_H
#define HHODOCAL_H

#include <TObject.h>
#include "hhodoraw.h"

class HHodoCal : public TObject {
protected:
   Int_t nHits;  // number of hits
   Int_t module; // module number
   Int_t strip;  // fiber number
   Float_t time1;  // tdc time of 1st hit
   Float_t adc1;   // adc of 1st hit
   Float_t time2;  // tdc time of 2nd hit
   Float_t adc2;   // adc of 2nd hit
   Float_t time3;  // tdc time of 3rd hit
   Float_t adc3;   // adc of 3rd hit
   Float_t time4;  // tdc time of 4th hit
   Float_t adc4;   // adc of 4th hit

public:
   HHodoCal(void) { clear(); }
   ~HHodoCal(void) {;}
   void clear(void);
  inline Int_t getMaxMult(void){ return(4);}// return number of multiplicity supported (constant)

   void setModule(const Int_t n) { module=n; }
   void setStrip(const Int_t n)    { strip=n; }
   void setAddress(const Int_t m,const Int_t s) {
      module=m;
      strip=s;
   };

   Bool_t setTimeAdc(const Float_t t,const Float_t a);

   Int_t getNHits(void) const { return nHits; }
   Float_t getTime(const Int_t n) const;
   Float_t getAdc(const Int_t n) const;
   void getTimeAndAdc(const Int_t n, Float_t& time, Float_t& adc );

   Int_t   getModule(void)const { return module;}
   Int_t   getStrip(void) const { return strip; }
   void getAddress(Int_t& m,Int_t& s) {
      m=module;
      s=strip;
   };

   ClassDef(HHodoCal,1)  // HODO detector cal data
};

#endif /* ! HHODOCAL_H */
