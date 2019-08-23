#ifndef HRTMATCHINGPAR_H
#define HRTMATCHINGPAR_H

#include "TObject.h"
#include "TArrayC.h"
#include "TArrayF.h"
#include "TArrayI.h"
#include "hparcond.h"
#include <iostream>

class HRtMatchingPar : public HParCond {
public:
  HRtMatchingPar(const Char_t *name="RtMatchingPar",
		  const Char_t *title="Matching parameters for reft",
		  const Char_t *context="RtMatchingParProduction");
  ~HRtMatchingPar(void);
  Bool_t init(HParIo *inp, Int_t *set);
  Int_t write(HParIo *io);
  Char_t &bin(Int_t i[]);
  Char_t &bin(Float_t a[]);
  Char_t &bin(Int_t address) { return fCells[address]; }
  Char_t &getLastBin(void) { return fCells[fTotalBins]; }
  void setDimension(Int_t n,Int_t bins[]);
  void setRange(Int_t i,Float_t min, Float_t max);
  void putParams(HParamList *);
  Bool_t getParams(HParamList *);
  void readFrom(const HRtMatchingPar &par);
  void printTo(std::ostream &out);
protected:
  TArrayI fIdx;
  TArrayC fCells; //
  Int_t fTotalBins;
  TArrayF fMin; //
  TArrayF fMax; //
  TArrayF fBinSize; //
  TArrayI fBins; //
public:
  ClassDef(HRtMatchingPar,2)
};

#endif
