#ifndef  HTOFMONITOR_H
#define  HTOFMONITOR_H

#include "hreconstructor.h"

class HCategory;

class HTofMonitor : public HReconstructor {
 private:
  HCategory *fRawCat; //!Raw data category
  HCategory *fHitCat; //!Pointer to the hits category
  HLocation fLoc;

 public:
  HTofMonitor(void);
  HTofMonitor(const Text_t *name,const Text_t *title);
  virtual ~HTofMonitor(void) {}
  Bool_t init(void);
  Bool_t finalize(void) {return kTRUE;}
  Int_t execute(void);

  ClassDef(HTofMonitor,1)
};


#endif /* !HTOFMONITOR_H */











