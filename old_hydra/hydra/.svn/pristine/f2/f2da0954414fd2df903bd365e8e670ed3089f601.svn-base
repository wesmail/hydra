#ifndef HGEOMSPHE_H
#define HGEOMSPHE_H

#include "hgeombasicshape.h"

class HGeomVolume;

class HGeomSphe : public HGeomBasicShape {
protected:
  HGeomRotation* intrinsicRot; // intrinsic rotation of ROOT shape
public:
  HGeomSphe();
  ~HGeomSphe();
  void calcVoluParam(HGeomVolume*);
  void calcVoluPosition(HGeomVolume*,
          const HGeomTransform&,const HGeomTransform&);
  TShape* createRootVolume(HGeomVolume*);
  Int_t readPoints(fstream*,HGeomVolume*);   
  Bool_t writePoints(fstream*,HGeomVolume*);   
  ClassDef(HGeomSphe,0) // class for geometry shape SPHE
};

#endif  /* !HGEOMSPHE_H */
