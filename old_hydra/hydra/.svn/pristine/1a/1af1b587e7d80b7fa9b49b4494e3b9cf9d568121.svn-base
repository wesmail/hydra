#ifndef HSTARTGEOMPAR_H
#define HSTARTGEOMPAR_H

#include "hdetgeompar.h"

class HGeomShapes;

class HStartGeomPar : public HDetGeomPar {
public:
  HStartGeomPar(HGeomShapes* s=0);
  ~HStartGeomPar() {}
  Bool_t init(HParIo*,Int_t*);
  Int_t write(HParIo*);
  Int_t getModNumInMod(const TString&);
  Int_t getModNumInComp(const TString&);
  Int_t getCompNum(const TString&);
  ClassDef(HStartGeomPar,1) // Container for the Start geometry parameters
};

inline Int_t HStartGeomPar::getModNumInMod(const TString& name) {
  // returns the module index retrieved from the module name ISTx
  return (Int_t)(name[3]-'0');
}

inline Int_t HStartGeomPar::getModNumInComp(const TString& name) {
  // returns the module index retrieved from the  name Sxxx
  return (Int_t)(name[1]-'0');
}

inline Int_t HStartGeomPar::getCompNum(const TString& name) {
  // returns the layer index retrieved from the layer name TxxSx
  return ((Int_t)(name[2]-'0')*10+(Int_t)(name[3]-'0'));
}

#endif /* !HSTARTGEOMPAR_H */
