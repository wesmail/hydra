#ifndef HMDCCALIBRATER2_H
#define HMDCCALIBRATER2_H

#include "hreconstructor.h"
#include "hlocation.h"

class HCategory;
class HIterator;
class HMdcCal1;
class HMdcCal2;

class HMdcCalibrater2 : public HReconstructor {
protected:
  HCategory* cal1Cat;      //! pointer to the cal1 data
  HCategory* cal2Cat;      //! pointer to the cal2 data
  HLocation loc;           //! location for new object.
  HIterator* iter;         //! iterator on raw data.
public:
  HMdcCalibrater2(void);
  HMdcCalibrater2(Text_t* name,Text_t* title);
  virtual ~HMdcCalibrater2(void);
  virtual Bool_t init(void);
  Bool_t finalize(void) {return kTRUE;}
  Int_t execute(void);
  virtual void getNewObject(HMdcCal2*);
  virtual void calibrate(HMdcCal1*,HMdcCal2*);  
  ClassDef(HMdcCalibrater2,0) // Calibrater2 of Mdc data.
};

#endif /* !HMDCCALIBRATER2_H */
