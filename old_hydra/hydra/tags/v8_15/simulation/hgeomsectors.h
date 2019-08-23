#ifndef HGEOMSECTORS_H
#define HGEOMSECTORS_H

#include "hgeomset.h"
#include "TString.h"

class HGeomMedia;

class  HGeomSectors : public HGeomSet {
protected:
  char modName[5];  // name of sector
public:
  HGeomSectors();
  ~HGeomSectors() {}
  const char* getModuleName(Int_t);  
  inline Int_t getModNumInMod(const TString&);
  Bool_t read(fstream&,HGeomMedia*);
  void addRefNodes();
  ClassDef(HGeomSectors,0) // Class for geometry of Sectors
};

#endif  /* !HGEOMSECTORS_H */

inline Int_t HGeomSectors::getModNumInMod(const TString& name) {
  // returns the sector index retrieved from SECx 
  return (Int_t)(name[3]-'0')-1;
}
