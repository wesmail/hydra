#ifndef HGEOMSTART_H
#define HGEOMSTART_H

#include "hgeomset.h"
#include "TString.h"

class  HGeomStart : public HGeomSet {
protected:
  char modName[5];  // name of module
  char eleName[3];  // substring for elements in module
public:
  HGeomStart();
  ~HGeomStart() {}
  const char* getModuleName(Int_t);
  const char* getEleName(Int_t);
  inline Int_t getModNumInMod(const TString&);
  ClassDef(HGeomStart,0) // Class for geometry of Start detector
};

#endif  /* !HGEOMSTART_H */

inline Int_t HGeomStart::getModNumInMod(const TString& name) {
  // returns the sector index retrieved from ISTx
  return (Int_t)(name[3]-'0');
}
