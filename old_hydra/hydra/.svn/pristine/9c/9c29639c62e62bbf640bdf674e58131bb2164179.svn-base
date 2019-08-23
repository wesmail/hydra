#ifndef HKICKSTARTC_H
#define HKICKSTARTC_H

#include "hreconstructor.h"
#include "hcategory.h"

class HKickStartC : public HReconstructor {
 protected:
  HCategory *fStartCat;
  Int_t fMaxMult;
  Int_t fSelectedStrip;
 public:
  HKickStartC(Text_t name[], Text_t title[]);
  ~HKickStartC(void);
  Bool_t init(void);
  Int_t execute(void);
  Bool_t finalize(void);
  void setMaxMult(Int_t m) { fMaxMult = m; }
  void setStrip(Int_t i) { fSelectedStrip = i; }
  ClassDef(HKickStartC,0)
};

#endif
