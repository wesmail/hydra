#ifndef HTOFINOCALSIMFILTER_H
#define HTOFINOCALSIMFILTER_H

#include "hfilter.h"
#include "htofinocalsim.h"

class HTofinoCalSimFilter : public HFilter{

 public:
  HTofinoCalSimFilter(void){;}
  ~HTofinoCalSimFilter(void){;}
  Bool_t check(TObject* raw);

ClassDef(HTofinoCalSimFilter,1)   // Filter class for Tofino digitizer
};
#endif
