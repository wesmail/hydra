#ifndef HKICKSHOWERTRANS
#define HKICKSHOWERTRANS

#include "hreconstructor.h" 
#include "hcategory.h"
#include "hiterator.h"
#include <TObject.h>

class HKickShowerTrans : public HReconstructor {
protected:
  HCategory *fShoInput; //!
  HCategory *fShoOutput; //!
  HIterator *fInIter; //!
  HLocation fLoc;
  Float_t fResolutionX;
  Float_t fResolutionY;

public:
  HKickShowerTrans(Text_t name[],Text_t title[]);
  ~HKickShowerTrans(void);
  Int_t execute(void);
  Bool_t init(void);
  Bool_t finalize(void);
  void setResolution(Float_t x, Float_t y) { fResolutionX=x; fResolutionY=y; }
  ClassDef(HKickShowerTrans,1)
};


#endif
