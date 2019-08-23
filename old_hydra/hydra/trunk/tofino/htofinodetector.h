#ifndef HTOFINODETECTOR_H
#define HTOFINODETECTOR_H

#include "hdetector.h"

class HTofinoDetector : public HDetector {
public:
  HTofinoDetector();
  ~HTofinoDetector();
  Bool_t init(void);
  void activateParIo(HParIo* io);
  Bool_t write(HParIo* io);
  HCategory* buildLinearCategory(const Text_t* name);
  HCategory* buildMatrixCategory(const Text_t* name, Float_t fillFactor=1);
  HCategory* buildCategory(Cat_t);

  ClassDef(HTofinoDetector,1) // Tofino detector class
};

#endif  /* !HTOFINODETECTOR_H */
