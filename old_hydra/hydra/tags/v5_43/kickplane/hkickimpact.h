#ifndef HKICKIMPACT_H
#define HKICKIMPACT_H

#include <TObject.h>
#include "hmdcseg.h"
#include "hgeomvector.h"

class HKickImpact : public TObject {
protected:
  HGeomVector fPos;
  HGeomVector fDir;
  HMdcSeg *fOrigSeg; //Original segment
  Int_t fSector;
public:
  HKickImpact(void) {}
  ~HKickImpact(void) {}
  HGeomVector &position(void) {return fPos;}
  HGeomVector &direction(void) {return fDir; }
  Int_t getSector(void) { return fSector; }
  void setSector(Int_t s) { fSector=s; }
  HMdcSeg *getOriginalSegment(void) { return fOrigSeg; }
  void setOriginalSegment(HMdcSeg *seg) { fOrigSeg=seg; }
  ClassDef(HKickImpact,0) //Impact in kick plane
};

#endif
