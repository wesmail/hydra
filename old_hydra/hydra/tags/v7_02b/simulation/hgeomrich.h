#ifndef HGEOMRICH_H
#define HGEOMRICH_H

#include "hgeomset.h"

class  HGeomRich : public HGeomSet {
protected:
  char modName[5];  // name of module
  char eleName[2];  // substring for elements in module
public:
  HGeomRich();
  ~HGeomRich() {}
  const char* getModuleName(Int_t) {return modName;}
  const char* getEleName(Int_t) {return eleName;}
  Bool_t read(fstream&,HGeomMedia*);
  void addRefNodes();
  ClassDef(HGeomRich,0) // Class for Rich
};

#endif  /* !HGEOMRICH_H */
