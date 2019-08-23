#ifndef HGEOMTARGET_H
#define HGEOMTARGET_H

#include "hgeomset.h"

class  HGeomTarget : public HGeomSet {
protected:
  char modName[2];  // name of module
  char eleName[2];  // substring for elements in module
public:
  HGeomTarget();
  ~HGeomTarget() {}
  const char* getModuleName(Int_t) {return modName;}
  const char* getEleName(Int_t) {return eleName;}
  ClassDef(HGeomTarget,0) // Class for geometry of Target
};

#endif  /* !HGEOMTARGET_H */
