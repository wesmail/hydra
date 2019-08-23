#ifndef HMDCCAL_H
#define HMDCCAL_H

#include "hdataobject.h"

class HMdcCal : public HDataObject {
protected:
  Short_t sector;
  Short_t module;
  Short_t layer;
  Short_t cell;
  Int_t nrOfHits;
  Float_t time1;
  Float_t time2;
public:
  HMdcCal() {clear();}
  ~HMdcCal() {}
  void clear();
  void setSector(const Int_t s) { sector = s; }
  void setModule(const Int_t m) { module = m; }
  void setLayer(const Int_t l) { layer = l; }
  void setCell(const Int_t c) { cell = c; }
  void setTime(Float_t);

  Int_t getSector() const { return sector; }
  Int_t getModule() const { return module; }
  Int_t getLayer() const { return layer; }
  Int_t getCell() const { return cell; }
  Int_t getNrOfHits() const { return nrOfHits; }
  Float_t getTime(Int_t hit = 0) const;

  ClassDef(HMdcCal,1) // cal hit on a MDC
};

#endif  /* HMDCCAL_H */

