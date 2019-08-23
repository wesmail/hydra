#ifndef HSTARTSELFCOPAR_H
#define HSTARTSELFCOPAR_H

#include "TObject.h"
#include "TObjArray.h"
#include "hparset.h"

class HStartSelfCoParChan : public TObject {
protected:
    Float_t time;
    Float_t halfwidth;
public:
    HStartSelfCoParChan() { clear(); }
    ~HStartSelfCoParChan() {;}
    Float_t getTime() { return time; }
    Float_t getHalfWidth() { return halfwidth; }
    void fill(Float_t t,Float_t hw) {
      time=t;
      halfwidth=hw;
    }
    void fill(HStartSelfCoParChan& r) {
      time=r.getTime();
      halfwidth=r.getHalfWidth();
    }
    void setTime(Float_t t) { time=t; }
    void setHalfWidth(Float_t hw) { halfwidth=hw; }
    void clear() {
      time=0.F;
      halfwidth=0.F;
    }
    ClassDef(HStartSelfCoParChan,1) // Chan level of the START selfcoincidence parameters
};

class HStartSelfCoParMod: public TObject {
protected:
  TObjArray* array;  // pointer array containing HStartSelfCoParChan objects
public:
  HStartSelfCoParMod(Int_t n=8);
  ~HStartSelfCoParMod();
  HStartSelfCoParChan& operator[](Int_t i) {
      return *static_cast<HStartSelfCoParChan*>((*array)[i]);
  }
  Int_t getSize() { return array->GetEntries(); }
  ClassDef(HStartSelfCoParMod,1) // Module level of Start selfcoincidence parameters
};

class HStartSelfCoPar : public HParSet {
protected:
  TObjArray* array;     // array of pointers of type HStartSelfCoParMod
public:
  HStartSelfCoPar(const char* name="StartSelfCoPar",
                  const char* title="Selfcoincidence parameters for Start",
                  const char* context="StartSelfCoProduction");
  ~HStartSelfCoPar();
  HStartSelfCoParMod& operator[](Int_t i) {
    return *static_cast<HStartSelfCoParMod*>((*array)[i]);
  }
  Int_t getSize() { return array->GetEntries(); }
  Bool_t init(HParIo* input,Int_t* set);
  Int_t write(HParIo* output);
  void clear();
  void printParam();
  void readline(const char*, Int_t*);
  void putAsciiHeader(TString&);
  Bool_t writeline(char*, Int_t, Int_t);
  ClassDef(HStartSelfCoPar,1) // Container for the START selfcoincidence parameters
};

#endif  /*!HSTARTSELFCOPAR_H*/
