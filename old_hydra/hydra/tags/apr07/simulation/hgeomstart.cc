//*-- AUTHOR : Ilse Koenig
//*-- Created : 12/12/2003

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////
// HGeomStart
//
// Class for geometry of START
//
/////////////////////////////////////////////////////////////

#include "hgeomstart.h"

ClassImp(HGeomStart)

HGeomStart::HGeomStart() {
  // Constructor
  fName="start";
  maxSectors=0;
  maxModules=6;
}

const char* HGeomStart::getModuleName(Int_t m) {
  // Return the module name in module m
  sprintf(modName,"IST%i",m);
  return modName;
}

const char* HGeomStart::getEleName(Int_t m) {
  // Return the element name in module m
  sprintf(eleName,"I%i",m);
  return eleName;
}
