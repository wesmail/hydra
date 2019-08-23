#ifndef HMDCSEGMENTSFILTER_H
#define HMDCSEGMENTSFILTER_H

# include "hfilter.h"
# include "hmdchitaux.h"

class HMdcHitsFilter : public HFilter{

 public:
  HMdcHitsFilter(void){;}
  ~HMdcHitsFilter(void){;}
  Bool_t check(HDataObject* segment);

ClassDef(HMdcHitsFilter,1)
};
#endif
