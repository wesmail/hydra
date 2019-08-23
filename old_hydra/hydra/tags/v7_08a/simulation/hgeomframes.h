#ifndef HGEOMFRAMES_H
#define HGEOMFRAMES_H

#include "hgeomset.h"

class  HGeomFrames : public HGeomSet {
protected:
  char modName[2];  // name of module
  char eleName[2];  // substring for elements in module
public:
  HGeomFrames();
  ~HGeomFrames() {}
  const char* getModuleName(Int_t) {return modName;}
  const char* getEleName(Int_t) {return eleName;}
  ClassDef(HGeomFrames,0) // Class for geometry of support structure
};

#endif  /* !HGEOMFRAMES_H */
