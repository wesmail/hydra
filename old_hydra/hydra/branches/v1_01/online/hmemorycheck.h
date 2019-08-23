#if !defined HMEMORYCHECK_H
#define HMEMORYCHECK_H

//-------------------------------------------------------------------------
//
// File name:       hmemorycheck.h
// Subject:         Memory checks.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            March 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the declaration of the class HMemoryCheck.
// This class serves to perform a check if the total amount of memory
// used by all histogrammes stays below the limit defined by the user.
//
//-------------------------------------------------------------------------

#include <TObject.h>

class TH1F;
class TH2F;

class HMemoryCheck : public TObject {

 protected:

  Int_t    fHiMemLimit;
  Bool_t   kAlarm;

 public:

  HMemoryCheck() : TObject() { fHiMemLimit = 50000000; check(); }
  virtual ~HMemoryCheck() {}
  void setMemLimit(Int_t alimit) { fHiMemLimit = alimit; check(); }
  Bool_t isAlarm() const { return kAlarm; }
  Int_t getMemLimit() const { return fHiMemLimit; }
  void check(Int_t *c = 0);
  Bool_t checkVirtual(Int_t nDim, Int_t bisx, Int_t binsy = 0);
  Int_t calc1d(TH1F *h1d);
  Int_t calc2d(TH2F *h2d);
  Int_t calc(Int_t nDim, Int_t binsx, Int_t binsy = 0);
  void alarm();

 public:

  ClassDef(HMemoryCheck, 0)

};

#endif
