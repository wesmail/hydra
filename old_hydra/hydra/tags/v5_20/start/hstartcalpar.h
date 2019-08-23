#ifndef HSTARTCALPAR_H
#define HSTARTCALPAR_H

#include "TObject.h"
#include "TObjArray.h"
#include "hparset.h"

class HStartCalParChan : public TObject {
protected:
    Float_t slope;
    Float_t offset;
public:
    HStartCalParChan() { clear(); }
    ~HStartCalParChan() {;}
    Float_t getSlope() { return slope; }
    Float_t getOffset() { return offset; }
    void fill(Float_t s,Float_t o) {
      slope=s;
      offset=o;
    }
    void fill(HStartCalParChan& r) {
      slope=r.getSlope();
      offset=r.getOffset();
    }
    void setSlope(Float_t s) { slope=s; }
    void setOffset(Float_t o) { offset=o; }
    void clear() {
      slope=0.F;
      offset=0.F;
    }
    ClassDef(HStartCalParChan,1) // Chan level of the START calibration parameters 
};

class HStartCalParMod: public TObject {
protected:
  TObjArray* array;  // pointer array containing HStartCalParChan objects
public:
  HStartCalParMod(Int_t n=8);
  ~HStartCalParMod();
  HStartCalParChan& operator[](Int_t i) {
      return *static_cast<HStartCalParChan*>((*array)[i]);
  }
  Int_t getSize() { return array->GetEntries(); }
  ClassDef(HStartCalParMod,1) // Module level of Start Calibration parameters
};

class HStartCalPar : public HParSet {
protected:
  TObjArray* array;     // array of pointers of type HStartCalParMod 
public:
  HStartCalPar();
  ~HStartCalPar();
  HStartCalParMod& operator[](Int_t i) {
    return *static_cast<HStartCalParMod*>((*array)[i]);
  }
  Int_t getSize() { return array->GetEntries(); }
  Bool_t init(HParIo* input,Int_t* set);
  Int_t write(HParIo* output);
  void clear();
  void printParam();
  void readline(const char*, Int_t*);
  void putAsciiHeader(TString&);
  Bool_t writeline(char*, Int_t, Int_t);
  ClassDef(HStartCalPar,1) // Container for the START calibration parameters
};

#endif  /*!HSTARTCALPAR_H*/









