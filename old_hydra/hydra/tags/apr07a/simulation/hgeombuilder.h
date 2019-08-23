#ifndef HGEOMBUILDER_H
#define HGEOMBUILDER_H

#include "TNamed.h"

class HGeomSet;
class HGeomNode;
class HGeomMedium;
class HGeomHit;

class HGeomBuilder : public TNamed {
protected:
  Int_t nRot;  // number of defined rotations
  Int_t nMed;  // number of defined media 

  HGeomBuilder();
  HGeomBuilder(const char* name,const char* title);
public:
  virtual ~HGeomBuilder() {}
  virtual Bool_t createNode(HGeomNode*)=0;
  virtual Int_t createMedium(HGeomMedium*)=0;
  virtual Bool_t createHit(HGeomHit*) {return kTRUE;}
  virtual void finalize() {}
  ClassDef(HGeomBuilder,0) // abstract base class for geometry builders
};

#endif /* !HGEOMBUILDER_H */
