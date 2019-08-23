#ifndef HGEOMMDC_H
#define HGEOMMDC_H

#include "hgeomset.h"
#include "TString.h"

class  HGeomMdc : public HGeomSet {
protected:
  char modName[6];  // name of module
  char eleName[3];  // substring for elements in module
public:
  HGeomMdc();
  ~HGeomMdc() {}
  const char* getModuleName(Int_t);
  const char* getEleName(Int_t);
  inline Int_t getSecNumInMod(const TString&);
  inline Int_t getModNumInMod(const TString&);
  ClassDef(HGeomMdc,0) // Class for geometry of Mdc
};

#endif  /* !HGEOMMDC_H */

inline Int_t HGeomMdc::getSecNumInMod(const TString& name) {
  // returns the sector index retrieved from DRxMx
  return (Int_t)(name[4]-'0')-1;
}

inline Int_t HGeomMdc::getModNumInMod(const TString& name) {
  // returns the module index retrieved from DRxMx
  return (Int_t)(name[2]-'0')-1;
}
