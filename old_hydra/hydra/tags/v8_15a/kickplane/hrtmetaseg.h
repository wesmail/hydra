#ifndef HRTMETASEG_H
#define HRTMETASEG_H

#include <TObject.h>
#include "hgeomvector.h"

class HRtMetaSeg : public TObject {
 public:
  HRtMetaSeg(void) {}
  ~HRtMetaSeg(void) {}
  void setPosition(const HGeomVector &pos) {fPos = pos; }
  void setDirection(const HGeomVector &dir) { fDir = dir; }
  void setTof(Float_t f) { fTof = f; }
  void setPull(Float_t f) { fPull = f; }
  void setResXY(Float_t x, Float_t y) { fResX = x; fResY = y; }
  void setSystem(Int_t s) { fSystem = s; }
  HGeomVector &getPosition(void) { return fPos; }
  HGeomVector &getDirection(void) { return fDir;}
  Float_t getTof(void) { return fTof; }
  Float_t getPull(void) { return fPull; }
  Int_t getSystem(void) { return fSystem; }
  Float_t getResX(void) { return fResX; }
  Float_t getResY(void) { return fResY; }
 protected:
  HGeomVector fPos;
  HGeomVector fDir;
  Float_t fTof;
  Float_t fPull;
  Float_t fResX;
  Float_t fResY;
  Int_t fSystem;
 public:
  ClassDef(HRtMetaSeg,1)
};
#endif
