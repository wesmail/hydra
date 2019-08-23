#ifndef HGEOMTUBE_H
#define HGEOMTUBE_H

#include "hgeombasicshape.h"

class HGeomVolume;

class HGeomTube : public HGeomBasicShape {
protected:
  HGeomRotation* intrinsicRot; // intrinsic rotation of ROOT shape
public:
  HGeomTube();
  ~HGeomTube();
  void calcVoluParam(HGeomVolume*);
  void calcVoluPosition(HGeomVolume*,
          const HGeomTransform&,const HGeomTransform&);
  TShape* createRootVolume(HGeomVolume*);
  Int_t readPoints(fstream*,HGeomVolume*);   
  Bool_t writePoints(fstream*,HGeomVolume*);   
  ClassDef(HGeomTube,0) // class for geometry shape TUBE
};

#endif  /* !HGEOMTUBE_H */
