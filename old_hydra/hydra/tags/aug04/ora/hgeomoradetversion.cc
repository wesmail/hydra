//*-- AUTHOR : Ilse Koenig
//*-- Created : 18/12/2003 by Ilse Koenig

/////////////////////////////////////////////////////////////
// HGeomOraDetVersion
//
// Utility class for geometry version management in Oracle
// (uses the Oracle C/C++ precompiler)
//
/////////////////////////////////////////////////////////////

#include "hgeomoradetversion.h"

ClassImp(HGeomOraDetVersion)

HGeomOraDetVersion::HGeomOraDetVersion(const char* name,Int_t id) {
  SetName(name);
  detectorId=id;
  geomVersion=-1;
  clearVersDate();
}

void HGeomOraDetVersion::clearVersDate() {
  versDate[0]=-1;
  versDate[1]=-1;
}

void HGeomOraDetVersion::fill(Int_t id,Int_t v,Double_t s,Double_t u) {
  detectorId=id;
  geomVersion=v;
  versDate[0]=s;
  versDate[1]=u;
}

void HGeomOraDetVersion::fill(Int_t v,Double_t s,Double_t u) {
  geomVersion=v;
  versDate[0]=s;
  versDate[1]=u;
}
