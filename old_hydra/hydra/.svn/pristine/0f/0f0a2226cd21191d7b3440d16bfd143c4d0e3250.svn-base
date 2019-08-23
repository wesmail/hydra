#ifndef HSTARTLOOKUP_H
#define HSTARTLOOKUP_H

#include "TObject.h"
#include "TObjArray.h"
#include "hparset.h"

class HStartLookupChan : public TObject {
protected:
    Int_t module;
    Int_t strip;
public:
    HStartLookupChan() { clear(); }
    ~HStartLookupChan() {;}
    Int_t getModule() { return module; }
    Int_t getStrip() { return strip; }
    void getAddress(Int_t& m,Int_t& s) {
      m=module;
      s=strip;
    }
    void fill(const Int_t m,const Int_t s) {
      module=m;
      strip=s;
    }
    void fill(HStartLookupChan& r) {
      module=r.getModule();
      strip=r.getStrip();
    }
    void setModule(const Int_t n) { module=n; }
    void setStrip(const Int_t n)    { strip=n; }
    void clear() {
      module=-1;
      strip=-1;
    }
    ClassDef(HStartLookupChan,1) // Channel level of the lookup table for the START unpacker 
};

class HStartLookupTdc: public TObject {
protected:
  TObjArray* array;  // pointer array containing HStartLookupChan objects
public:
  HStartLookupTdc(Int_t n=8);
  ~HStartLookupTdc();
  HStartLookupChan& operator[](Int_t i) {
      return *static_cast<HStartLookupChan*>((*array)[i]);
  }
  Int_t getSize() { return array->GetEntries(); }
  ClassDef(HStartLookupTdc,1) // Tdc level of  the lookup table for the START unpacker
};

class HStartLookup : public HParSet {
protected:
  TObjArray* array;     // array of pointers of type HStartLookupMod 
public:
  HStartLookup(Int_t nTdc=2, Int_t nChan=8);
  ~HStartLookup();
  HStartLookupTdc& operator[](Int_t i) {
    return *static_cast<HStartLookupTdc*>((*array)[i]);
  }
  Int_t getSize() { return array->GetEntries(); }
  Bool_t init(HParIo* input,Int_t* set);
  Int_t write(HParIo* output);
  void clear();
  void printParam();
  void readline(const char*, Int_t*);
  void putAsciiHeader(TString&);
  Bool_t writeline(char*, Int_t, Int_t);
  ClassDef(HStartLookup,1) // Lookup table for the START unpacker
};

#endif  /*!HSTARTLOOKUP_H*/









