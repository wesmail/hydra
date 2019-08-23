#ifndef HKICKPARINFO_H
#define HKICKPARINFO_H

#include "TObject.h"
#include "TString.h"
#include "hgeomvector.h"

class HKickParInfo : public TObject {
 protected:
  TString fDate;
  TString fAuthor;
  TString fComment;
  HGeomVector fVertex;
  Float_t fFieldStrength;
  TString fMode;
 public:
  HKickParInfo(void) { fFieldStrength=0.;}
  ~HKickParInfo(void) {}
  void setDate(TString d) { fDate = d; }
  void setAuthor(TString d) { fAuthor = d; }
  void setComment(TString d) { fComment = d; }
  void setVertex(Float_t x,Float_t y,Float_t z) {
    fVertex.setXYZ(x,y,z);
  }
  void setFieldStrength(Float_t f) { fFieldStrength = f; }
  void setMode(TString mode) { fMode = mode; }
  void print(void);
  ClassDef(HKickParInfo,1)
};

#endif
