#ifndef HGEOMELTU_H
#define HGEOMELTU_H

#include "hgeombasicshape.h"

class HGeomVolume;

class HGeomEltu : public HGeomBasicShape {
protected:
  HGeomRotation* intrinsicRot; // intrinsic rotation of ROOT shape
public:
  HGeomEltu();
  ~HGeomEltu();
  void calcVoluParam(HGeomVolume*);
  void calcVoluPosition(HGeomVolume*,
          const HGeomTransform&,const HGeomTransform&);
  TShape* createRootVolume(HGeomVolume*);
  Int_t readPoints(fstream*,HGeomVolume*);   
  Bool_t writePoints(fstream*,HGeomVolume*);   
  ClassDef(HGeomEltu,0) // class for geometry shape ELTU
};

#endif  /* !HGEOMELTU_H */
