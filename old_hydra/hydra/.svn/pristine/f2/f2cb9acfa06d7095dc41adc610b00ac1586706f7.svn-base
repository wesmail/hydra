#ifndef HTOFINOPARSET_H
#define HTOFINOPARSET_H

#include "hparset.h"
#include "hparhadasciifileio.h"

class HParIo;

class HTofinoParSet : public HParSet {
protected:
public:
  HTofinoParSet();
  virtual ~HTofinoParSet() {}

  virtual Bool_t init(HParIo* inp,Int_t* set);  
  virtual Int_t write(HParIo*);

  ClassDef(HTofinoParSet,1) // Base class for all Tofino parameter containers
};

#endif  /* !HTOFINOPARSET_H */

