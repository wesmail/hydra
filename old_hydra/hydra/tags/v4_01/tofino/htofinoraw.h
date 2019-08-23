#ifndef HTOFINORAW_H
#define HTOFINORAW_H
#pragma interface

#include "hlocateddataobject.h"

class HTofinoRaw : public HLocatedDataObject {
private:
  Float_t  tdcTofino; //TDC
  Float_t  adcTofino; //ADC
  Short_t  nHit; //number of hits
  Char_t  sector, module, cell; //Location
public:
  HTofinoRaw(void);
  ~HTofinoRaw(void);
  Float_t getTdcT(void);
  Float_t getAdcT(void);
  Short_t getNHit(void);
  Short_t getAddress(void) {return  sector;}
  Char_t getSector(void) {return  sector;}
  Char_t getModule(void) {return  module;}
  Char_t getCell(void) {return  cell;}  
  void setTdcT(Float_t atdcTofino);
  void setAdcT(Float_t aadcTofino);
  void setNHit(Short_t anHit); 
  void incNHit(void) {nHit++;} 
  Int_t getNLocationIndex(void);
  Int_t getLocationIndex(Int_t i);
  void setSector(Char_t s) { sector=s;}
  void setModule(Char_t m) { module=m;}
  void setCell(Char_t c) { cell=c;}
  void setAddress(Short_t add) { sector=add;}
  void doit(void);
  ClassDef(HTofinoRaw,1) //TOFINO raw data class
};

#endif /* !HTOFINORAW_H */
