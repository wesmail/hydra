#ifndef HGEOMSHAPES_H
#define HGEOMSHAPES_H

#include "hgeomtransform.h"
#include "TObject.h"
#include "TString.h"
#include "TList.h"
#include <fstream.h>

class HGeomVolume;
class HGeomBasicShape;

class  HGeomShapes : public TObject {
protected:
  TList* shapes;  // list of already created shape classes
public :
  HGeomShapes();
  ~HGeomShapes();
  HGeomBasicShape* selectShape(HGeomVolume *);  
  HGeomBasicShape* selectShape(const TString&);  
  Int_t readPoints(fstream*,HGeomVolume*);   
  Bool_t writePoints(fstream*,HGeomVolume*);   
  HGeomBasicShape* calcVoluParam(HGeomVolume*); 
  HGeomBasicShape* calcVoluPosition(HGeomVolume*,const HGeomTransform&);
  HGeomBasicShape* calcVoluPosition(HGeomVolume*,const HGeomTransform&,
                                    const HGeomTransform&);
  ClassDef(HGeomShapes,0) // manager class for geometry shapes
};

#endif  /* !HGEOMSHAPES_H */
