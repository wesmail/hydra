#ifndef HGEOMCONE_H
#define HGEOMCONE_H

#include "hgeombasicshape.h"

class HGeomVolume;

class HGeomCone : public HGeomBasicShape {
public:
  HGeomCone();
  ~HGeomCone();
  void calcVoluParam(HGeomVolume*);
  void calcVoluPosition(HGeomVolume*,
          const HGeomTransform&,const HGeomTransform&);
  TShape* createRootVolume(HGeomVolume*);
  Int_t readPoints(fstream*,HGeomVolume*);   
  Bool_t writePoints(fstream*,HGeomVolume*);   
  ClassDef(HGeomCone,0) // class for geometry shape CONE
};

#endif  /* !HGEOMCONE_H */
