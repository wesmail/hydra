#ifndef HHODORAW_H
#define HHODORAW_H

#include "hlocateddataobject.h"

class HHodoRaw : public HLocatedDataObject {
protected:
  Int_t nHits;    // number of hits
  Int_t module;   // module number
  Int_t fiber;    // fiber number
  Int_t time1;    // tdc time of 1st hit
  Int_t width1;   // width of 1st hit
  Int_t time2;    // tdc time of 2nd hit
  Int_t width2;   // width of 2nd hit
  Int_t time3;    // tdc time of 3rd hit
  Int_t width3;   // width of 3rd hit
  Int_t time4;    // tdc time of 4th hit
  Int_t width4;   // width of 4th hit
public:
  HHodoRaw(void) {clear();}
  ~HHodoRaw(void) {}
  void clear(void) {
    nHits=0;
    module=fiber=time1=width1=time2=width2=time3=width3=time4=width4=-1;
  } 
  Int_t getNHits(void) const { return nHits; }
  Int_t getModule(void) const {return module;}  
  Int_t getFiber(void) const {return fiber;}  
  void getAddress(Int_t& m,Int_t& f) {
      m=module;
      f=fiber;
  }
  Int_t getTime(const Int_t n) const;
  Int_t getWidth(const Int_t n) const;
  void getTimeAndWidth(const Int_t,Int_t&,Int_t&);
  void setModule(Int_t m) {module=m;}
  void setFiber(Int_t f) {fiber=f;}
  void setAddress(const Int_t m,const Int_t f) {
    module=m;
    fiber=f;
  }
  Bool_t fill(const Int_t,const Int_t);
	Bool_t fill_lead(const Int_t time);
	Bool_t fill_trail(const Int_t time);

  ClassDef(HHodoRaw,1) // Raw data of Pion Hodoscopes
};

#endif /* !HHODORAW_H */
