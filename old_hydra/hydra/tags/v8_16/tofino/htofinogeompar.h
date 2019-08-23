#ifndef HTOFINOGEOMPAR_H
#define HTOFINOGEOMPAR_H

#include "hdetgeompar.h"

class HTofinoGeomPar : public HDetGeomPar {
public:
  HTofinoGeomPar(const char* name="TofinoGeomPar",
                 const char* title="Tofino geometry parameters",
                 const char* context="GeomProduction");
  ~HTofinoGeomPar() {}
  Bool_t init(HParIo*,Int_t*);
  Int_t write(HParIo*);
  inline Int_t getSecNum(const TString&);
  inline Int_t getModNumInMod(const TString&);
  inline Int_t getModNumInComp(const TString&);
  inline Int_t getCompNum(const TString&);
  ClassDef(HTofinoGeomPar,1) // Container for the Tofino geometry parameters
};

inline Int_t HTofinoGeomPar::getSecNum(const TString& name) {
  // returns the sector index retrieved from the module name TFNx
  return (Int_t)(name[3]-'0')-1;
}

inline Int_t HTofinoGeomPar::getModNumInMod(const TString& name) {
  // returns the module index 0
  return 0;
}

inline Int_t HTofinoGeomPar::getModNumInComp(const TString& name) {
  // returns the module index 0
  return 0;
}

inline Int_t HTofinoGeomPar::getCompNum(const TString& name) {
  // returns the cell index retrieved from the component name TxxS1
  return ((Int_t)(name[1]-'0')*10+(Int_t)(name[2]-'0')-23);
}

#endif /* !HTOFINOGEOMPAR_H */
