#ifndef HGEOMPGON_H
#define HGEOMPGON_H

#include "hgeombasicshape.h"

class HGeomVolume;

class HGeomPgon : public HGeomBasicShape {
protected:
  HGeomRotation* intrinsicRot; // intrinsic rotation of ROOT shape
public:
  HGeomPgon();
  ~HGeomPgon();
  void calcVoluParam(HGeomVolume*);
  void calcVoluPosition(HGeomVolume*,
          const HGeomTransform&,const HGeomTransform&);
  TShape* createRootVolume(HGeomVolume*);
  Int_t readPoints(fstream*,HGeomVolume*);   
  Bool_t writePoints(fstream*,HGeomVolume*);   
  ClassDef(HGeomPgon,0) // class for geometry shape PGON
};

#endif  /* !HGEOMPGON_H */
