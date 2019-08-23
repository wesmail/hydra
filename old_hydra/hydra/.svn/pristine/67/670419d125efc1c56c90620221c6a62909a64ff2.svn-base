#ifndef HMDCCALIBRATER_H
#define HMDCCALIBRATER_H

#include "hreconstructor.h"
#include "hlocation.h"

class HCategory;
class HIterator;
class HMdcCalPar;
class HMdcLookupGeom;

class HMdcCalibrater : public HReconstructor {
private:
  HCategory* rawCat;       //! pointer to the raw data
  HCategory* calCat;       //! pointer to the cal data
  HLocation loc;           //! location for new object.
  HIterator* iter;         //! iterator on raw data.
  HMdcCalPar* calpar;      //! calibration parameters
  HMdcLookupGeom* lookup;  //! lookup table for mapping
public:
  HMdcCalibrater(void);
  HMdcCalibrater(Text_t* name,Text_t* title);
  ~HMdcCalibrater(void);
  Bool_t init(void);
  void setParContainers();
  Bool_t finalize(void) {return kTRUE;}
  Int_t execute(void);
  ClassDef(HMdcCalibrater,0) //Calibrater of Mdc data.
};

#endif /* !HMDCCALIBRATER_H */

