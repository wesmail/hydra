#ifndef HTOFINOLOOKUP_H
#define HTOFINOLOOKUP_H

#include "TObject.h"
#include "TObjArray.h"
#include "htofinoparset.h"

class HTofinoLookupChan : public TObject {
protected:
    Int_t sector;
    Int_t module;
    Int_t cell;
public:
    HTofinoLookupChan() { clear(); }
    ~HTofinoLookupChan() {;}
    Int_t getSector() { return sector; }
    Int_t getModule() { return module; }
    Int_t getCell()  { return cell; }
    void getAddress(Int_t& s,Int_t& m,Int_t& c) {
      s=sector;
      m=module;
      c=cell;
    }
    void fill(const Int_t s,const Int_t m,const Int_t c) {
      sector=s;
      module=m;
      cell=c;
    }
    void fill(HTofinoLookupChan& r) {
      sector=r.getSector();
      module=r.getModule();
      cell=r.getCell();
    }
    void setSector(const Int_t n) { sector=n; }
    void setModule(const Int_t n) { module=n; }
    void setCell(const Int_t n)  { cell=n; }
    void clear() {
      sector=-1;
      module=-1;
      cell=-1;
    }
    ClassDef(HTofinoLookupChan,1) // Channel level of the lookup table for the TOFINO unpacker 
};

class HTofinoLookupDConv: public TObject {
  friend class HTofinoLookup;
protected:
  TObjArray* array; // pointer array containing HTofinoLookupChan objects
  Int_t crate;      // crate number
  Int_t slot;       // slot number
  Char_t dConvType; // type of digital converter (A=ADC, T=TDC, U=undefined)
  Int_t nChan;      // number of  TDC/ADC channels
public:
  HTofinoLookupDConv(Int_t n=32);
  ~HTofinoLookupDConv();
  HTofinoLookupChan* getChannel(Int_t c) {
    if (c>=0 && c<=nChan) return &((*this)[c]);
    else return 0;
  }
   HTofinoLookupChan& operator[](Int_t i) {
      return *static_cast<HTofinoLookupChan*>((*array)[i]);
  }
  Int_t getSize()  { return nChan; }
  Int_t getCrate() { return crate; }
  Int_t getSlot()  { return slot; }
  Char_t getType() { return dConvType; }
  void setAddress(Int_t c, Int_t s, Char_t t) {
    crate=c;
    slot=s;
    dConvType=t;
  }
  void clear();
  ClassDef(HTofinoLookupDConv,1) // ADC/TDC level of  the lookup table for the TOFINO unpacker
};

class HTofinoLookup : public HTofinoParSet {
protected:
  TObjArray* array;     // array of pointers of type HTofinoLookupDConv
  Int_t nDConv;         // number of TDC/ADCs
public:
  HTofinoLookup(const char* name="TofinoLookup",
                const char* title="Lookup table for Tofino unpacker",
                const char* context="",
                Int_t numDConv=2, Int_t numChan=32);
  ~HTofinoLookup();
  HTofinoLookupDConv* getDConv(Int_t crate,Int_t slot) {
    for(Int_t i=0;i<nDConv;i++) {
      HTofinoLookupDConv& dconv=(*this)[i];
      if (dconv.crate==crate && dconv.slot==slot) return &dconv;
    }
    return 0;
  }
  HTofinoLookupDConv& operator[](Int_t i) {
    return *static_cast<HTofinoLookupDConv*>((*array)[i]);
  }
  Int_t getSize() { return nDConv; }
  void clear();
  void printParam();
  void setAddress(Int_t,Int_t,Char_t,Int_t,Int_t,Int_t,Int_t);
  void readline(const char*, Int_t*);
  void putAsciiHeader(TString&);
  Bool_t writeline(char*, Int_t, Int_t);
  ClassDef(HTofinoLookup,1) // Lookup table for the TOFINO unpacker
};

#endif  /*!HTOFINOLOOKUP_H*/









