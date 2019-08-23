#ifndef HWALLRAW_H
#define HWALLRAW_H

#include "hlocateddataobject.h"

class HWallRaw : public HLocatedDataObject {
protected:
  Int_t   cell;   // location
  Float_t time;   // tdc time
  Float_t charge; // adc charge
public:
  HWallRaw(void) : cell(-1), time(0.), charge(0.) {}
  ~HWallRaw(void) {}
  Int_t getCell(void) {return  cell;}  
  Float_t getTime(void) {return time;}
  Float_t getCharge(void) {return charge;}
  void setCell(Int_t c) {cell=c;}
  void setTime(Float_t t) {time=t;}
  void setCharge(Float_t c) {charge=c;}
  void clear(void) {
    cell=-1;
    time=charge=0.;
  } 
  ClassDef(HWallRaw,1) // Raw data class of Forward Wall
};

#endif /* !HWallRAW_H */
