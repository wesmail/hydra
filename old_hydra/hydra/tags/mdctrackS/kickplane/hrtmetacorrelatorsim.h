#ifndef HRTMETACORRELATORSIM_H
#define HRTMETACORRELATORSIM_H

#include "hrtmetacorrelator.h"

class HRtMetaCorrelatorSim : public HRtMetaCorrelator {
 public:
  HRtMetaCorrelatorSim(void) {}
  ~HRtMetaCorrelatorSim(void) {}
 protected:
  Bool_t getShowerCategory(HEvent *event);
 public:
  ClassDef(HRtMetaCorrelatorSim,1)
};

#endif
