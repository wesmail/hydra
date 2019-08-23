#ifndef HTOFINODITIZER_H
#define HTOFINODITIZER_H

#include "hreconstructor.h"
#include "iostream.h"
#include "hlocation.h"

class HIterator;
class HCategory;

class HTofinoDigitizer : public HReconstructor {

private:
  HLocation fLoc;                        //! Location for new object
  HCategory* fGeantCat;                 //! Pointer to Geant data category
  HCategory* fRawCat;                   //! Pointer to raw data category
  HIterator* iter;                      //! Iterator over GeantCell category


public:
  HTofinoDigitizer(void);
  HTofinoDigitizer(Text_t* name,Text_t* title);
  ~HTofinoDigitizer(void);
  Bool_t init(void);
  Bool_t finalize(void) {return kTRUE;}
  Int_t execute(void);

public:
  ClassDef(HTofinoDigitizer,0) //Digitizer of TOFINO data.
};

#endif
