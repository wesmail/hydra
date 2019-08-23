#ifndef HGEOMBRIK_H
#define HGEOMBRIK_H

#include "hgeombasicshape.h"

class HGeomVolume;

class HGeomBrik : public HGeomBasicShape {
protected:
  HGeomRotation* intrinsicRot; // intrinsic rotation of ROOT shape
public:
  HGeomBrik();
  ~HGeomBrik();
  void calcVoluParam(HGeomVolume*);
  void calcVoluPosition(HGeomVolume*,
          const HGeomTransform&,const HGeomTransform&);
  TShape* createRootVolume(HGeomVolume*);
  ClassDef(HGeomBrik,0) // class for geometry shape BOX or BRIK
};

#endif  /* !HGEOMBRIK_H */
