#ifndef HMDCLOOKUPGEOM_H
#define HMDCLOOKUPGEOM_H

#include "TNamed.h"
#include "TObjArray.h"
#include "hparset.h"

class HParIo;

class HMdcLookupChan: public TObject {
protected:
  Int_t nLayer;  // number of the layer
  Int_t nCell;   // number of the cell
public:
  HMdcLookupChan(Int_t nl=-1,Int_t nc=-1) {
    nLayer=nl;
    nCell=nc;
  }
  ~HMdcLookupChan() {}
  void setNLayer(Int_t l) {nLayer=l;}
  void setNCell(Int_t c) {nCell=c;}
  void fill(Int_t l,Int_t c) {
    nLayer=l;
    nCell=c;
  }
  void fill(HMdcLookupChan& r) {
    nLayer=r.getNLayer();
    nCell=r.getNCell();
  }
  Int_t getNLayer() {return nLayer;}
  Int_t getNCell() {return nCell;}
  void clear() {
    nLayer=-1;
    nCell=-1;
  }
  ClassDef(HMdcLookupChan,1) // Channel level of MDC lookup table (channel->wire)
};


class HMdcLookupMoth: public TNamed {
protected:
  TObjArray* array;
public:
  HMdcLookupMoth(Int_t n=0,Text_t* name="");
  ~HMdcLookupMoth();
  HMdcLookupChan& operator[](Int_t i) {
    return *static_cast<HMdcLookupChan*>((*array)[i]);
  }
  Int_t getSize() {return array->GetEntries();}
  ClassDef(HMdcLookupMoth,1) // Motherboard level of MDC lookup table (channel->wire)
};


class HMdcLookupGMod: public TObject {
protected:
  TObjArray* array;
public:
  HMdcLookupGMod(Int_t sec=0,Int_t mod=0,Int_t n=16);
  ~HMdcLookupGMod();
  HMdcLookupMoth& operator[](Int_t i) {
    return *static_cast<HMdcLookupMoth*>((*array)[i]);
  }
  Int_t getSize() {return array->GetEntries();}
  ClassDef(HMdcLookupGMod,1) // Module level of MDC lookup table (channel->wire)
};


class HMdcLookupGSec: public TObject {
protected:
  TObjArray* array;
  Int_t sector;
public:
  HMdcLookupGSec(Int_t sec=0,Int_t n=4);
  ~HMdcLookupGSec();
  HMdcLookupGMod& operator[](Int_t i) {
    return *static_cast<HMdcLookupGMod*>((*array)[i]);
  }
  Int_t getSize() {return array->GetEntries();}
  ClassDef(HMdcLookupGSec,1) // Sector level of MDC lookup table (channel->wire)
};


class HMdcLookupGeom : public HParSet {
protected:
  TObjArray* array;     // array of pointers
public:
  HMdcLookupGeom(Int_t n=6);
  ~HMdcLookupGeom();
  HMdcLookupGSec& operator[](Int_t i) {
    return *static_cast<HMdcLookupGSec*>((*array)[i]);
  }
  Bool_t init(HParIo* input,Int_t* set);
  Int_t write(HParIo* output);
  void readline(const char*, Int_t*);
  void putAsciiHeader(TString&);
  Bool_t writeline(char *, Int_t, Int_t, Int_t, Int_t);
  Int_t getSize() {return array->GetEntries();}
  void clear();
  ClassDef(HMdcLookupGeom,1) //  MDC lookup table (channel->wire)
};

#endif /* !HMDCLOOKUPGEOM_H */














