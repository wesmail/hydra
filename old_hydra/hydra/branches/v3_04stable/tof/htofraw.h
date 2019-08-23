#ifndef HTOFRAW_H
#define HTOFRAW_H
#pragma interface

#include "hlocateddataobject.h"

class HTofRaw : public HLocatedDataObject {
private:
  Float_t  rightT; //TDC: right hit
  Float_t  leftT; //TDC: left hit
  Float_t  rightQ; //QDC: right hit
  Float_t  leftQ; //QDC: left hit
  Short_t  nHit; //number of hit
  Char_t  sector, module, cell; //Location
public:
  HTofRaw(void);
  ~HTofRaw(void);
  Float_t getRightTime(void);
  Float_t getLeftTime(void);
  Float_t getRightCharge(void);
  Float_t getLeftCharge(void);
  Short_t getNHit(void);
  Short_t getAddress(void) {return  sector;}
  Char_t getSector(void) {return  sector;}
  Char_t getModule(void) {return  module;}
  Char_t getCell(void) {return  cell;}  
  void setRightTime(Float_t arightT);
  void setLeftTime(Float_t aleftT);
  void setRightCharge(Float_t arightQ);
  void setLeftCharge(Float_t aleftQ);
  void setNHit(Short_t anHit); 
  void incNHit(void) {nHit++;} 
  Int_t getNLocationIndex(void);
  Int_t getLocationIndex(Int_t i);
  void setSector(Char_t s) { sector=s;}
  void setModule(Char_t m) { module=m;}
  void setCell(Char_t c) { cell=c;}
  void setAddress(Short_t add) { sector=add;}
  void doit(void);
  ClassDef(HTofRaw,1) //TOF raw data class
};

#endif /* !HTOFRAW_H */
