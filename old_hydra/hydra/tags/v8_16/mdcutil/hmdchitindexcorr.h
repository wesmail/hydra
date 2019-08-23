#ifndef  HMDCHITINDEXCORR_H
#define  HMDCHITINDEXCORR_H


#include "hreconstructor.h"

class HCategory;
class HIterator;

class HMdcHitIndexCorr: public HReconstructor {

protected: 
  HCategory *fMdcSegCat;
  HIterator *fMdcSegIter;

public: 
  HMdcHitIndexCorr(void);
  HMdcHitIndexCorr(Text_t *name,Text_t *title);
  ~HMdcHitIndexCorr(void);
  Bool_t init(void);
  Bool_t finalize(void);
  Int_t execute(void);
  
  ClassDef(HMdcHitIndexCorr,0)
    };

#endif /* !HMDCHITINDEXCORR_H */
