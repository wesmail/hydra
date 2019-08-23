#ifndef HGEOMFWALL_H
#define HGEOMFWALL_H

#include "hgeomset.h"

class  HGeomFWall : public HGeomSet {
protected:
  char modName[5];  // name of module
  char eleName[2];  // substring for elements in module
public:
  HGeomFWall();
  ~HGeomFWall() {}
  const char* getModuleName(Int_t) {return modName;}
  const char* getEleName(Int_t) {return eleName;}
  ClassDef(HGeomFWall,0) // Class for Forward Wall
};

#endif  /* !HGEOMFWALL_H */
