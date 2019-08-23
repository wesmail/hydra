#ifndef HGEOMCONS_H
#define HGEOMCONS_H

#include "hgeombasicshape.h"

class HGeomVolume;

class HGeomCons : public HGeomBasicShape {
protected:
  HGeomRotation* intrinsicRot; // intrinsic rotation of ROOT shape
public:
  HGeomCons();
  ~HGeomCons();
  void calcVoluParam(HGeomVolume*);
  void calcVoluPosition(HGeomVolume*,
          const HGeomTransform&,const HGeomTransform&);
  TShape* createRootVolume(HGeomVolume*);
  Int_t readPoints(fstream*,HGeomVolume*);   
  Bool_t writePoints(fstream*,HGeomVolume*);   
  ClassDef(HGeomCons,0) // class for geometry shape CONS
};

#endif  /* !HGEOMCONS_H */
