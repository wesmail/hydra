#ifndef HGEOMBRIK_H
#define HGEOMBRIK_H

#include "hgeombasicshape.h"

class HGeomVolume;

class HGeomBrik : public HGeomBasicShape {
public:
  HGeomBrik();
  ~HGeomBrik();
  HGeomBasicShape* calcVoluParam(HGeomVolume*);
  HGeomBasicShape* calcVoluPosition(HGeomVolume*,
          const HGeomTransform&,const HGeomTransform&);
  ClassDef(HGeomBrik,0) // class for geometry shape BOX or BRIK
};

#endif  /* !HGEOMBRIK_H */
