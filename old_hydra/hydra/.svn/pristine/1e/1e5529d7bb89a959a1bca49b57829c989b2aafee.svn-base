#ifndef HGEOMTUBS_H
#define HGEOMTUBS_H

#include "hgeombasicshape.h"

class HGeomVolume;

class HGeomTubs : public HGeomBasicShape {
public:
  HGeomTubs();
  ~HGeomTubs();
  void calcVoluParam(HGeomVolume*);
  void calcVoluPosition(HGeomVolume*,
          const HGeomTransform&,const HGeomTransform&);
  TShape* createRootVolume(HGeomVolume*);
  Int_t readPoints(fstream*,HGeomVolume*);   
  Bool_t writePoints(fstream*,HGeomVolume*);   
  ClassDef(HGeomTubs,0) // class for geometry shape TUBS
};

#endif  /* !HGEOMTUBS_H */
