#ifndef HTOFINODETECTOR_H
#define HTOFINODETECTOR_H

#include "hdetector.h"

class HParIo;
class HDetGeomPar;
class HGeomShapes;
class HCategory;

class HTofinoDetector : public HDetector {
public:
  HTofinoDetector();
  ~HTofinoDetector();
  Bool_t init(void);
  void activateParIo(HParIo* io);
  Bool_t write(HParIo* io);
  HCategory* buildLinearCategory(Text_t* name);
  HCategory* buildMatrixCategory(Text_t* name, Float_t fillFactor=1);
  HCategory* buildCategory(Cat_t);
  HDetGeomPar* createDetGeomPar(HGeomShapes*);

  ClassDef(HTofinoDetector,1) // Tofino detector class
};

#endif  /* !HTOFINODETECTOR_H */
