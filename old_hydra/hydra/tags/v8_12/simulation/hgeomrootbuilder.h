#ifndef HGEOMROOTBUILDER_H
#define HGEOMROOTBUILDER_H

#include "hgeombuilder.h"

class TGeoManager;

class HGeomRootBuilder : public HGeomBuilder {
protected:
  TGeoManager* geoManager; // ROOT geometry manager
public:
  HGeomRootBuilder();
  HGeomRootBuilder(const char*,const char*);
  ~HGeomRootBuilder() {}
  void setGeoManager(TGeoManager* m) {geoManager=m;}
  Bool_t createNode(HGeomNode*);
  Int_t createMedium(HGeomMedium*);
  void finalize();
  void checkOverlaps(Double_t ovlp=0.0001);
  ClassDef(HGeomRootBuilder,0) // class to create geometry in ROOT
};

#endif /* !HGEOMROOTBUILDER_H */
