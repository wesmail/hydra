#ifndef  HDILEPFILTER_H
#define  HDILEPFILTER_H

#include "hreconstructor.h"

class HCategory;
class HLinearCategory;

class HIterator;

class HDiLepFilter: public HReconstructor {
 protected: 
  HCategory *fPidDilepton;
  HIterator *fPidDileptonIter;
 public: 
  HDiLepFilter(void);
  HDiLepFilter(Text_t *name,Text_t *title);
  ~HDiLepFilter(void);

  Bool_t init(void);
  Bool_t finalize(void);
  Int_t execute(void);
  ClassDef(HDiLepFilter,0)
    };

#endif /* !HDILEPFILTER */
