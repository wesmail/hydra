#ifndef HHODOLOOKUP_H
#define HHODOLOOKUP_H

#include "TObject.h"
#include "TObjArray.h"
#include "hparset.h"

class HHodoLookupChan : public TObject {
protected:
  Int_t module;    // module number
  Int_t fiber;     // fiber number
public:
  HHodoLookupChan(void) { clear(); }
  ~HHodoLookupChan(void) {;}
  Int_t getModule(void)  { return module; }
  Int_t getFiber(void)  { return fiber; }
  void getAddress(Int_t& m,Int_t& f) {
    m=module;
    f=fiber;
  }
  void fill(HHodoLookupChan& r) {
    module=r.getModule();
    fiber=r.getFiber();
  }
  void setModule(const Int_t n) { module=n; }
  void setFiber(const Int_t n) { fiber=n; }
  void fill(const Int_t m,const Int_t f) {
    module=m;
    fiber=f;
  }
  void clear(void) {
    module=fiber=-1;
  }
  ClassDef(HHodoLookupChan,1) // Channel level of the lookup table for the Hodoscopes unpacker 
};

class HHodoLookupSlot: public TObject {
protected:
  TObjArray* array; // pointer array containing HHodoLookupChan objects
  Int_t maxChannel; // actual maximum index of channel
  Int_t nChannels;  // maximum number of channels in a slot
public:
  HHodoLookupSlot(Int_t numChannels=1);
  ~HHodoLookupSlot(void);
  HHodoLookupChan& operator[](Int_t i) {
      return *static_cast<HHodoLookupChan*>((*array)[i]);
  }
  HHodoLookupChan* getChannel(Int_t c) {
    if (c>=0 && c<=maxChannel) return &((*this)[c]);
    else return 0;
  }
  Int_t getSize(void)  { return maxChannel+1; }
  Int_t getMaxSize(void) { return nChannels; }
  void fill(Int_t,Int_t,Int_t);
  void clear(void);
  ClassDef(HHodoLookupSlot,1) // TDC level of  the lookup table for the Hodoscopes unpacker
};

class HHodoLookupCrate: public TObject {
protected:
  TObjArray* array; // pointer array containing HHodoLookupSlot objects
  Int_t maxSlot;    // actual maximum index of slots
  Int_t nSlots;     // maximum number of slots in a crate
public:
  HHodoLookupCrate(Int_t numSlots=1);
  ~HHodoLookupCrate(void);
  HHodoLookupSlot& operator[](Int_t i) {
      return *static_cast<HHodoLookupSlot*>((*array)[i]);
  }
  HHodoLookupSlot* getSlot(Int_t s) {
    if (s>=0 && s<=maxSlot) return &((*this)[s]);
    else return 0;
  }
  Int_t getSize(void) { return maxSlot+1; }
  Int_t getMaxSize(void) { return nSlots; }
  void fill(Int_t,Int_t,Int_t,Int_t);
  ClassDef(HHodoLookupCrate,1) // Crate level of the lookup table for the Hodoscopes unpacker
};

class HHodoLookup : public HParSet {
protected:
  TObjArray* array;  // array of pointers of type HHodoLookupCrate
  Int_t maxCrate;    // actual maximum index of crates
public:
  HHodoLookup(const Char_t* name="HodoLookup",
             const Char_t* title="Lookup table for Hodoscopes unpacker",
             const Char_t* context="HodoLookupProduction",
             Int_t nCrates=1, Int_t nSlots=1);
  ~HHodoLookup(void);
  HHodoLookupCrate& operator[](Int_t i) {
    return *static_cast<HHodoLookupCrate*>((*array)[i]);
  }
  HHodoLookupSlot* getSlot(Int_t c, Int_t s) {
    if (c>=0 && c<=maxCrate) return (*this)[c].getSlot(s);
    else return 0;
  }
  Int_t getSize(void) { return maxCrate+1; }
  Bool_t init(HParIo*,Int_t*);
  Int_t write(HParIo*);
  void clear(void);
  void printParam(void);
  void fill(Int_t,Int_t,Int_t,Int_t,Int_t);
  void readline(const Char_t*, Int_t*);
  void putAsciiHeader(TString&);
  Bool_t writeline(Char_t*,Int_t,Int_t,Int_t);
  ClassDef(HHodoLookup,1) // Lookup table for the unpacker of the Pion Hodoscopes
};

#endif  /*!HHODOLOOKUP_H*/









