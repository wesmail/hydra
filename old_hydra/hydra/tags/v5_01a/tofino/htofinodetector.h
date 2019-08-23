#ifndef HTOFINODETECTOR_H
#define HTOFINODETECTOR_H

#include "hdetector.h"

class HParIo;

class HTofinoDetector : public HDetector {
public:
  HTofinoDetector();
  ~HTofinoDetector();
  Bool_t init(void);
  void activateParIo(HParIo* io);
  Bool_t write(HParIo* io);
  HCategory* buildLinearCategory(Text_t*);
  HCategory* buildMatrixCategory(Text_t*);
  HCategory* buildCategory(Cat_t);

  ClassDef(HTofinoDetector,1) // Tofino detector class
};

#endif  /* !HTOFINODETECTOR_H */
