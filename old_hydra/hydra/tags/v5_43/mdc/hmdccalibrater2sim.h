#ifndef HMDCCALIBRATER2SIM_H
#define HMDCCALIBRATER2SIM_H

#include "hmdccalibrater2.h"

class HMdcCal1;
class HMdcCal2;

class HMdcCalibrater2Sim : public HMdcCalibrater2 {
protected:
HMdcCal2Par *cal2par;
public:
  HMdcCalibrater2Sim(void) {}
  HMdcCalibrater2Sim(Text_t* name,Text_t* title)
    : HMdcCalibrater2(name,title) {}
  ~HMdcCalibrater2Sim(void);
  Bool_t init();
  void getNewObject(HMdcCal2*);
  void setParContainers();
  void calibrate(HMdcCal1*,HMdcCal2*);
  Int_t execute(void);
  ClassDef(HMdcCalibrater2Sim,0) // Calibrater2 of Mdc data.
};

#endif /* !HMDCCALIBRATER2SIM_H */

