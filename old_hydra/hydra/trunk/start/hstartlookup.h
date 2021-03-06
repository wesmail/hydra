#ifndef HSTARTLOOKUP_H
#define HSTARTLOOKUP_H

#include "TObject.h"
#include "TObjArray.h"
#include "hparset.h"

class HStartLookupChan : public TObject {
protected:
  Int_t module;  // module number
  Int_t strip;   // strip number
  Char_t cType;    // Left,right,meantimer
public:
  HStartLookupChan() { clear(); }
  ~HStartLookupChan() {;}
  Int_t getModule() { return module; }
  Int_t getStrip()  { return strip;  }
  Char_t getType()    { return cType;  }
  void getAddress(Int_t& m,Int_t& s) {
    m=module;
    s=strip;
  }
  void fill(const Int_t m,const Int_t s) {
    module=m;
    strip=s;
  }
  void fill(const Int_t m,const Int_t s,Char_t c) {
    module=m;
    strip=s;
    cType=c;
  }
  void fill(HStartLookupChan& r) {
    module=r.getModule();
    strip=r.getStrip();
    cType=r.getType();
  }
  void setModule(const Int_t n) { module=n; }
  void setStrip(const Int_t n)  { strip=n; }
  void setType(Char_t c){ cType=c; }
  void clear() {
    module=-1;
    strip=-1;
    cType='\0';
  }
  ClassDef(HStartLookupChan,2) // Channel level of the lookup table for the START unpacker
};

class HStartLookupTdc: public TObject {
  friend class HStartLookup;
protected:
  TObjArray* array; // pointer array containing HStartLookupChan objects
  Int_t crate;      // crate number
  Int_t slot;       // slot number
  Int_t nChan;          // number of TDC channels
public:
  HStartLookupTdc(Int_t n=32);
  ~HStartLookupTdc();
  HStartLookupChan* getChannel(Int_t c) {
    if (c>=0 && c<=nChan) return &((*this)[c]);
    else return 0;
  }
  HStartLookupChan& operator[](Int_t i) {
      return *static_cast<HStartLookupChan*>((*array)[i]);
  }
  Int_t getSize()  { return nChan; }
  Int_t getCrate() { return crate; }
  Int_t getSlot()  { return slot; }
void setAddress(Int_t c, Int_t s) {
    crate=c;
    slot=s;
  }
  void clear();
  ClassDef(HStartLookupTdc,1) // Tdc level of  the lookup table for the START unpacker
};

class HStartLookup : public HParSet {
protected:
  TObjArray* array;     // array of pointers of type HStartLookupTdc
  Int_t nTdc;           // number of TDCs
public:
  HStartLookup(const Char_t* name="StartLookup",
               const Char_t* title="Lookup table for Start unpacker",
               const Char_t* context="StartLookupProduction",
               Int_t numTdc=1, Int_t numChan=32);
  ~HStartLookup();
  HStartLookupTdc* getTdc(Int_t crate,Int_t slot) {
    for(Int_t i=0;i<nTdc;i++) {
      HStartLookupTdc& tdc=(*this)[i];
      if (tdc.crate==crate && tdc.slot==slot) return &tdc;
    }
    return 0;
  }
  HStartLookupTdc& operator[](Int_t i) {
    return *static_cast<HStartLookupTdc*>((*array)[i]);
  }
  Int_t getSize() { return nTdc; }
  Bool_t init(HParIo* input,Int_t* set);
  Int_t write(HParIo* output);
  void clear();
  void printParam();
  void setAddress(Int_t,Int_t,Int_t,Int_t,Int_t);
  void setAddress(Int_t,Int_t,Int_t,Int_t,Int_t,char);
  void readline(const Char_t*, Int_t*);
  void putAsciiHeader(TString&);
  Bool_t writeline(Char_t*, Int_t, Int_t);
  ClassDef(HStartLookup,1) // Lookup table for the START unpacker
};

#endif  /*!HSTARTLOOKUP_H*/









