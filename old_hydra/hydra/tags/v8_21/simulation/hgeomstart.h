#ifndef HGEOMSTART_H
#define HGEOMSTART_H

#include "hgeomset.h"
#include "TString.h"

class  HGeomStart : public HGeomSet {
protected:
  Char_t modName[5];  // name of module
  Char_t eleName[3];  // substring for elements in module
public:
  HGeomStart();
  ~HGeomStart() {}
  const Char_t* getModuleName(Int_t);
  const Char_t* getEleName(Int_t);
  inline Int_t getModNumInMod(const TString&);
  ClassDef(HGeomStart,0) // Class for geometry of Start detector
};

#endif  /* !HGEOMSTART_H */

inline Int_t HGeomStart::getModNumInMod(const TString& name) {
  // returns the sector index retrieved from ISTx
  return (Int_t)(name[3]-'0');
}
