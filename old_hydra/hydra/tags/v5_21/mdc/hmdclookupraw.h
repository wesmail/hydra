#ifndef HMDCLOOKUPRAW_H
#define HMDCLOOKUPRAW_H

#include "TNamed.h"
#include "TObjArray.h"
#include "hparset.h"

class HParIo;

class HMdcLookupCell: public TObject {
protected:
  Int_t nMoth;   // number of the Motherboard
  Int_t nChan;   // number of the TDC channel
public:
  HMdcLookupCell(Int_t m=-1,Int_t c=-1) {
    nMoth=m;
    nChan=c;
  }
  ~HMdcLookupCell() {}
  void setNMoth(Int_t m) {nMoth=m;}
  void setNChan(Int_t c) {nChan=c;}
  void fill(Int_t m,Int_t c) {
    nMoth=m;
    nChan=c;
  }
  Int_t getNMoth() {return nMoth;}
  Int_t getNChan() {return nChan;}
  void clear() {
    nMoth=-1;
    nChan=-1;
  }
  ClassDef(HMdcLookupCell,1) // Cell level of MDC lookup table (cell->channel)
};


class HMdcLookupLayer: public TObject {
protected:
  TObjArray* array;
public:
  HMdcLookupLayer(Int_t n=0);
  ~HMdcLookupLayer();
  HMdcLookupCell& operator[](Int_t i) {
    return *static_cast<HMdcLookupCell*>((*array)[i]);
  }
  Int_t getSize() {return array->GetEntries();}
  ClassDef(HMdcLookupLayer,1) // Layer level of MDC lookup table (cell->channel)
};


class HMdcLookupRMod: public TObject {
protected:
  TObjArray* array;
public:
  HMdcLookupRMod(Int_t sec=0,Int_t mod=0,Int_t n=6);
  ~HMdcLookupRMod();
  HMdcLookupLayer& operator[](Int_t i) {
    return *static_cast<HMdcLookupLayer*>((*array)[i]);
  }
  Int_t getSize() {return array->GetEntries();}
  ClassDef(HMdcLookupRMod,1) // Module level of MDC lookup table (cell->channel)
};


class HMdcLookupRSec: public TObject {
protected:
  TObjArray* array;
  Int_t sector;
public:
  HMdcLookupRSec(Int_t sec=0,Int_t n=4);
  ~HMdcLookupRSec();
  HMdcLookupRMod& operator[](Int_t i) {
    return *static_cast<HMdcLookupRMod*>((*array)[i]);
  }
  Int_t getSize() {return array->GetEntries();}
  ClassDef(HMdcLookupRSec,1) // Sector level of MDC lookup table (cell->channel)
};


class HMdcLookupRaw : public HParSet {
protected:
  TObjArray* array;     // array of pointers
public:
  HMdcLookupRaw(Int_t n=6);
  ~HMdcLookupRaw();
  HMdcLookupRSec& operator[](Int_t i) {
    return *static_cast<HMdcLookupRSec*>((*array)[i]);
  }
  Bool_t init(HParIo* input,Int_t* set);
  Int_t  write(HParIo* output);
  Bool_t writeline(char *, Int_t, Int_t, Int_t, Int_t);
  void putAsciiHeader(TString&);
  Int_t  getSize() {return array->GetEntries();}
  void clear();
  ClassDef(HMdcLookupRaw,1) //  MDC lookup table (cell->channel)
};

#endif /* !HMDCLOOKUPRAW_H */

