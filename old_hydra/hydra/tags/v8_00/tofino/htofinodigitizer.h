#ifndef HTOFINODITIZER_H
#define HTOFINODITIZER_H
using namespace std;
#include "hreconstructor.h"
#include <iostream>
#include <iomanip>
#include "hlocation.h"
#include "htofinodigitpar.h"

class HIterator;
class HCategory;
class HTofinoDigitPar;

class HTofinoDigitizer : public HReconstructor {

private:
  HLocation fLoc;               //! Location for new object
  HCategory* fGeantCat;         //! Pointer to HGeant data category
  HCategory* fCalCat;           //! Pointer to Cal data category
  HIterator* iterGeant;         //! Iterator over HGeantTof category
  HIterator* iterCal;           //! Iterator over HTofinoCalSim category
  HTofinoDigitPar *pTofinoDigitPar; //! Digit parameters

public:
  HTofinoDigitizer(void);
  HTofinoDigitizer(Text_t* name,Text_t* title);
  ~HTofinoDigitizer(void);
  Bool_t initParContainer();
  Bool_t init(void);
  Bool_t finalize(void); 
  Int_t execute(void);

public:
  ClassDef(HTofinoDigitizer,0) // Digitizer for TOFINO HGeant hits
};

#endif
