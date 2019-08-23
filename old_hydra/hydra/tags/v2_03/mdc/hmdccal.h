#ifndef HMDCCAL_H
#define HMDCCAL_H

#include "hdataobject.h"

class HMdcCal : public HDataObject {
protected:
  Short_t sector;  // sector number
  Short_t module;  // module number
  Short_t layer;   // layer number
  Short_t cell;    // cell number
  Int_t nrOfHits;  // number of hits in this cell
  Float_t time1;   // time of first hit
  Float_t time2;   // time of second hit
  Short_t status;  // status of cell (e.g. 1 if ok)
public:
  HMdcCal() {clear();}
  ~HMdcCal() {}
  void clear();
  void setSector(const Int_t s) { sector = s; }
  void setModule(const Int_t m) { module = m; }
  void setLayer(const Int_t l) { layer = l; }
  void setCell(const Int_t c) { cell = c; }
  void setTime(Float_t);
  void setStatus(const Int_t f) { status = f; }

  Int_t getSector() const { return sector; }
  Int_t getModule() const { return module; }
  Int_t getLayer() const { return layer; }
  Int_t getCell() const { return cell; }
  Int_t getNrOfHits() const { return nrOfHits; }
  Float_t getTime(Int_t hit = 0) const;
  Int_t getStatus() const {return status;}

  ClassDef(HMdcCal,1) // cal hit on a MDC
};

#endif  /* HMDCCAL_H */

