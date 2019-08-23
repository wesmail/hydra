#ifndef HSTARTCALIBRATER_H
#define HSTARTCALIBRATER_H

#include "hreconstructor.h"
#include "hlocation.h"
 
class HCategory;
class HIterator;
class HStartCalPar;

class HStartCalibrater : public HReconstructor {
protected:
  HCategory *rawCat;          //! pointer to the raw data 
  HCategory *calCat;          //! pointer to the cal data
  HIterator *iter;            //! iterator on raw data.
  HLocation loc;              //! location for new cal object
  HStartCalPar* calpar;       //! calibration parameters
//  HCategory *fTBoxRaw;      //!
//  HCategory *fTBoxCal;      //!
//  HIterator *iter2;         //!
public: 
  HStartCalibrater(void);
  HStartCalibrater(const Text_t* name,const Text_t* title);
  ~HStartCalibrater(void);
  Bool_t init(void);
  Bool_t finalize(void) { return kTRUE; }
  Int_t execute(void);
public:
  ClassDef(HStartCalibrater,0) // Calibrater raw->cal for START data
};

#endif /* !HSTARTCALIBRATER_H */
















