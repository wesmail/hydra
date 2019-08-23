#ifndef HMDCXY_H
#define HMDCXY_H

#include "hdataobject.h"

class HMdcXY : public HDataObject {
protected:
  Int_t sector;      // sector number
  Int_t module;      // module number
  Float_t xHit;      // x in sector frame
  Float_t yHit;      // y
  Float_t zHit;      // z
  Int_t nCells;      // number of cells in hit

public:
  HMdcXY(void) {clear();}
  ~HMdcXY(void) {}
  void clear() {
    sector=module=-1;
    xHit = yHit = zHit = 0.0;
    nCells = 0;
  }
  void setSector(const Int_t s) { sector = s; }
  void setModule(const Int_t m) { module = m; }
  void setAddress(const Int_t s,const Int_t m) {
    sector=s;
    module=m;
  }
  void setX(const Float_t x) {xHit=x;}
  void setY(const Float_t y) {yHit=y;}
  void setZ(const Float_t z) {zHit=z;}
  void setNBins(const Int_t n) {nCells=n;}

  Int_t getSector(void) const { return sector; }
  Int_t getModule(void) const { return module; }
  void getAddress(Int_t& s,Int_t& m) {
    s=sector;
    m=module;
  }
  Float_t getX(void) const {return xHit;}
  Float_t getY(void) const {return yHit;}
  Float_t getZ(void) const {return zHit;}
  Int_t getNBins(void) const {return nCells;}

  ClassDef(HMdcXY,1) // temporary MDC hit class (Nov.99 test beam)
};

#endif  /* HMDCXY_H */





