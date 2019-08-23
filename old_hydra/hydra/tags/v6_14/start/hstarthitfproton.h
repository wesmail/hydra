#ifndef HSTARTHITFPROTON_H
#define HSTARTHITFPROTON_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "htofinocalpar.h"
#include "hdetector.h"
 
class HCategory;
class HIterator;
class HSpectrometer;

class HStartHitFProton : public HReconstructor {
protected:
  HCategory *pCalCat;          // pointer to the cal data
  HCategory *pHitCat;          // pointer to the hit data
  HIterator *pIterCal;            // iterator over cal data.
  HLocation loc;              // location for new hit object

public: 
  HStartHitFProton(void);
  HStartHitFProton(Text_t* name,Text_t* title);
  ~HStartHitFProton(void);
  Bool_t init(void);
  Int_t execute(void);
  Bool_t finalize(void);
  void clear();

  ClassDef(HStartHitFProton,0) // Hit finder for START detector
};

#endif /* !HSTARTHITFPROTON_H */

