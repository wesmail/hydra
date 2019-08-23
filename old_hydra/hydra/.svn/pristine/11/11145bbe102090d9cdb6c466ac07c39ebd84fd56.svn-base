#ifndef HTOFDIGITIZER_H
#define HTOFDIGITIZER_H

#include "hreconstructor.h"
#include "iostream.h"
#include "hlocation.h"

class HIterator;
class HCategory;
class HTofDigiPar;

class HTofDigitizer : public HReconstructor {

private:
  HLocation fLoc;                       //! Location for new object
  HCategory* fGeantCat;                 //! Pointer to Geant data category
  HCategory* fRawCat;                   //! Pointer to raw data category
  HTofDigiPar *fDigiPar;                //! Digitization parameters
  HIterator* iterGeant;                 //! Iterator over GeantCell category
  HIterator* iterTofRaw;                // Iterator over HTofRawSim category
public:
  HTofDigitizer(void);
  HTofDigitizer(Text_t* name,Text_t* title);
  ~HTofDigitizer(void);
  void initParContainer();
  Bool_t init(void);
  Bool_t finalize(void) {return kTRUE;}
  Int_t execute(void);

public:
  ClassDef(HTofDigitizer,0) //Digitizer of TOF data.
};

#endif









