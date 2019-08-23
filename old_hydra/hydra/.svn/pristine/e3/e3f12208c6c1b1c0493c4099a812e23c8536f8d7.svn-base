#ifndef HMDCCALIBRATER1_H
#define HMDCCALIBRATER1_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "TRandom.h"

class HCategory;
class HIterator;
class HMdcCalParRaw;
class HMdcLookupGeom;

class HMdcCalibrater1 : public HReconstructor {
protected:
  HCategory* rawCat;       //! pointer to the raw data
  HCategory* calCat;       //! pointer to the cal data
  HLocation loc;           //! location for new object.
  HIterator* iter;         //! iterator on raw data.
  HMdcCalParRaw* calparraw;      //! calibration parameters
  HMdcLookupGeom* lookup;  //! lookup table for mapping
public:
  HMdcCalibrater1(void);
  HMdcCalibrater1(Text_t* name,Text_t* title);
  ~HMdcCalibrater1(void);
  Bool_t init(void);
  void setParContainers(void);
  Bool_t finalize(void) {return kTRUE;}
  Int_t execute(void);
  ClassDef(HMdcCalibrater1,0) // Calibrater raw->cal1 for Mdc data
};

#endif /* !HMDCCALIBRATER1_H */

