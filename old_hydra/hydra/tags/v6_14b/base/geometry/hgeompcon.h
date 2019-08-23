#ifndef HGEOMPCON_H
#define HGEOMPCON_H

#include "hgeombasicshape.h"

class HGeomVolume;

class HGeomPcon : public HGeomBasicShape {
public:
  HGeomPcon();
  ~HGeomPcon();
  void calcVoluParam(HGeomVolume*);
  void calcVoluPosition(HGeomVolume*,
          const HGeomTransform&,const HGeomTransform&);
  TShape* createRootVolume(HGeomVolume*);
  Int_t readPoints(fstream*,HGeomVolume*);   
  Bool_t writePoints(fstream*,HGeomVolume*);   
  ClassDef(HGeomPcon,0) // class for geometry shape PCON
};

#endif  /* !HGEOMPCON_H */
