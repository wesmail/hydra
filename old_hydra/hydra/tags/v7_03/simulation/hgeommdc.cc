//*-- AUTHOR : Ilse Koenig
//*-- Created : 10/11/2003

/////////////////////////////////////////////////////////////
// HGeomMdc
//
// Class for geometry of MDC
//
/////////////////////////////////////////////////////////////

#include "hgeommdc.h"
#include "hgeommdchit.h"

ClassImp(HGeomMdc)

HGeomMdc::HGeomMdc() {
  // Constructor
  fName="mdc";
  maxSectors=6;
  maxModules=4;
  pHit=new HGeomMdcHit(this);
}

const char* HGeomMdc::getModuleName(Int_t m) {
  // Return the module name in plane m
  sprintf(modName,"DR%iM",m+1);
  return modName;
}

const char* HGeomMdc::getEleName(Int_t m) {
  // Return the element name in plane m
  sprintf(eleName,"D%i",m+1);
  return eleName;
}
  
