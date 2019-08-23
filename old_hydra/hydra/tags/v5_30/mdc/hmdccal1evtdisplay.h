#ifndef HMDCCAL1EVTDISPLAY_H
#define HMDCCAL1EVTDISPLAY_H

#include "hcatevtdisplay.h"
#include "TObjArray.h"

class HCategory;
class HMdcNodes;
class HIterator;
 
class HMdcCal1EvtDisplay : public HCatEvtDisplay {
  HMdcNodes* nodes;
  HIterator* iter;      //! iterator on cal1 data.  
  Float_t tabtLim1, tabtLim2;  // limits for time above threshold
public:
  HMdcCal1EvtDisplay(HCategory*,HMdcNodes*);
  ~HMdcCal1EvtDisplay(void) {}
  Bool_t init(void) {return kTRUE;}
  Bool_t reinit(void);
  Int_t execute();     
  void clear(void);
  void setTabtLim1(Float_t t) {tabtLim1=t;}
  void setTabtLim2(Float_t t) {tabtLim2=t;}
  Float_t getTabtLim1() {return tabtLim1;} 
  Float_t getTabtLim2() {return tabtLim2;} 
  ClassDef(HMdcCal1EvtDisplay,0) // base class for the event display of categories
};

#endif  /* !HMDCCAL1EVTDISPLAY_H */
