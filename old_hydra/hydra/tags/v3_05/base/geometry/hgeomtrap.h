#ifndef HGEOMTRAP_H
#define HGEOMTRAP_H

#include "hgeombasicshape.h"

class HGeomVolume;

class HGeomTrap : public HGeomBasicShape {
protected:
  HGeomRotation* intrinsicRot; // intrinsic rotation of ROOT shape
public:
  HGeomTrap();
  ~HGeomTrap();
  void calcVoluParam(HGeomVolume*);
  void calcVoluPosition(HGeomVolume*,
            const HGeomTransform&,const HGeomTransform&);
  TShape* createRootVolume(HGeomVolume*);
  ClassDef(HGeomTrap,0) // class for geometry shape TRAP
};

#endif  /* !HGEOMTRAP_H */
