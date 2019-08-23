#ifndef HCATEVTDISPLAY_H
#define HCATEVTDISPLAY_H

#include <TNamed.h>

class HCategory;
 
class HCatEvtDisplay : public TNamed {
protected:
  HCategory* cat;
  Bool_t isActive;
public:
  HCatEvtDisplay(void);
  virtual ~HCatEvtDisplay(void) {}
  virtual Bool_t init(void)=0;
  virtual Bool_t reinit(void)=0;
  virtual Int_t execute()=0;     
  virtual void clear(void)=0;
  void setActive(Bool_t f=kTRUE);
  ClassDef(HCatEvtDisplay,0) // base class for the event display of categories
};

#endif  /* !_H */
