#ifndef HMDCMBOREADOUT_H
#define HMDCMBOREADOUT_H

#include "TNamed.h"
#include "TObjArray.h"
#include "TArrayI.h"
#include "hparset.h"

class HParIo;

class HMdcMboReadoutMoth: public TNamed {
protected:
  Int_t bus;    // position in LVL1 bus
  Int_t busPos; // bus position in the readout controller (ROC)
  Int_t roc;    // number of the ROC
  Int_t samPos; // position in the SAM modul
  Int_t sam;    // number of the SAM module
public:
  HMdcMboReadoutMoth() {clear();}
  ~HMdcMboReadoutMoth() {}
  void setBus(Int_t n) {bus=n;}
  void setBusPos(Int_t n) {busPos=n;}
  void setRoc(Int_t n) {roc=n;}
  void setSam(Int_t n) {sam=n;}
  void setSamPos(Int_t n) {samPos=n;}
  void fill(Int_t,Int_t,Int_t,Int_t,Int_t);
  void fill(HMdcMboReadoutMoth&);
  Int_t getBus() {return bus;}
  Int_t getBusPos() {return busPos;}
  Int_t getRoc() {return roc;}
  Int_t getSam() {return sam;}
  Int_t getSamPos() {return samPos;}
  void clear() {
    bus=busPos=roc=sam=samPos=-1;
  }
  ClassDef(HMdcMboReadoutMoth,1) // MBO level of MdcMboReadout parameter container
};


class HMdcMboReadoutMod: public TObject {
protected:
  TObjArray* array;
public:
  HMdcMboReadoutMod(Int_t n=16);
  ~HMdcMboReadoutMod();
  HMdcMboReadoutMoth& operator[](Int_t i) {
    return *static_cast<HMdcMboReadoutMoth*>((*array)[i]);
  }
  Int_t getSize() {return array->GetEntries();}
  ClassDef(HMdcMboReadoutMod,1) // motherboard level of parameter container MdcMboReadout
};


class HMdcMboReadoutSec: public TObject {
protected:
  TObjArray* array;
public:
  HMdcMboReadoutSec(Int_t n=4);
  ~HMdcMboReadoutSec();
  HMdcMboReadoutMod& operator[](Int_t i) {
    return *static_cast<HMdcMboReadoutMod*>((*array)[i]);
  }
  Int_t getSize() {return array->GetEntries();}
  ClassDef(HMdcMboReadoutSec,1) // Sector level of parameter container MdcMboReadout
};


class HMdcMboReadout : public HParSet {
protected:
  TObjArray* array;     // array of pointers
public:
  HMdcMboReadout(const Char_t* name="MdcMboReadout",
                 const Char_t* title="Mdc Mbo readout addresses",
                 const Char_t* context="",
                 Int_t n=6);
  ~HMdcMboReadout();
  HMdcMboReadoutSec& operator[](Int_t i) {
    return *static_cast<HMdcMboReadoutSec*>((*array)[i]);
  }
  Bool_t init(HParIo* input,Int_t* set);
  Int_t write(HParIo* output);
  void readline(const Char_t*, Int_t*);
  void putAsciiHeader(TString&);
  Bool_t writeline(Char_t *, Int_t, Int_t, Int_t);
  Int_t getSize() {return array->GetEntries();}
  void clear();
  void printParam();
  ClassDef(HMdcMboReadout,1) // container for readout addresses of the MDC motherboards
};

#endif /* !HMDCMBOREADOUT_H */







