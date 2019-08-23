#ifndef HTOFINODITIZER_H
#define HTOFINODITIZER_H

#include "hreconstructor.h"
#include "iostream.h"
#include "hlocation.h"
#include "htofinocalpar.h"

class HIterator;
class HCategory;
class HTofinoCalPar;

class HTofinoDigitizer : public HReconstructor {

private:
  HLocation fLoc;               //! Location for new object
  HCategory* fGeantCat;         //! Pointer to HGeant data category
  HCategory* fCalCat;           //! Pointer to CAL data category
  HIterator* iter;              //! Iterator over GeantCell category
  HTofinoCalPar *pTofinoCalPar; //! Digit parameters 


public:
  HTofinoDigitizer(void);
  HTofinoDigitizer(Text_t* name,Text_t* title);
  ~HTofinoDigitizer(void);
  void initParContainer();
  Bool_t init(void);
  Bool_t finalize(void) {return kTRUE;}
  Int_t execute(void);

public:
  ClassDef(HTofinoDigitizer,0) // Digitizer for TOFINO HGeant hits
};

#endif
