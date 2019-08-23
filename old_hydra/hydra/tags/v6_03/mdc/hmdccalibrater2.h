#ifndef HMDCCALIBRATER2_H
#define HMDCCALIBRATER2_H

#include "hreconstructor.h"
#include "hlocation.h"

class HCategory;
class HIterator;
class HMdcCal1;
class HMdcCal2;
class HMdcCal2Par;

class HMdcCalibrater2 : public HReconstructor {
protected:
  HCategory* cal1Cat;            //! pointer to the cal1 data
  HCategory* cal2Cat;            //! pointer to the cal2 data
  HLocation loc;                 //! location for new object.
  HIterator* iter;               //! iterator on cal1 data.
  HMdcCal2Par* cal2par;          //! calibration parameters  time ->distance

public:
  HMdcCalibrater2(void);
  HMdcCalibrater2(Text_t* name,Text_t* title);
  ~HMdcCalibrater2(void);
  Bool_t init(void);
  void setParContainers(void);
  Bool_t finalize(void) {return kTRUE;}
  Int_t execute(void);
  void getNewObject(HMdcCal2*);
  void calibrate(HMdcCal1*,HMdcCal2*);
  ClassDef(HMdcCalibrater2,0) // Calibrater2 of Mdc data.
};

#endif /* !HMDCCALIBRATER2_H */
