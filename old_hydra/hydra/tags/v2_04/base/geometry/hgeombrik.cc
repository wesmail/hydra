//*-- AUTHOR : Ilse Koenig
//*-- Modified : 14/05/99 by Ilse Koenig

/////////////////////////////////////////////////////////////
//
// HGeomBrik
//
// class for the ROOT shape BRIK which is identical with the
// GEANT shape BOX
// 
// The intrisic coordinate system of a BRIK in ROOT has the
// same orientation as the volume described by its 8 points
// in the technical coordinate system 
// 
/////////////////////////////////////////////////////////////

#include "hgeombrik.h"
#include "hgeomvolume.h"
#include "hgeomvector.h"

ClassImp(HGeomBrik)

HGeomBrik::HGeomBrik() {
  fName="BOX ";
  nPoints=8;
  nParam=3;
}


HGeomBrik::~HGeomBrik() {
  if (param) { delete param; param=0; }
  if (intrinsicRot) { delete intrinsicRot; param=0; }
  if (center) { delete center; param=0; }
  if (position) { delete position; param=0; }
}


HGeomBasicShape* HGeomBrik::calcVoluParam(HGeomVolume* volu) {
  HGeomVector v=*(volu->getPoint(5)) - *(volu->getPoint(3));
  v.abs();
  v*=(1/20.);
  if (!param) param=new TArrayD(nParam);
  for(Int_t i=0;i<nParam;i++) param->AddAt(v(i),i);
  return this;
} 


HGeomBasicShape* HGeomBrik::calcVoluPosition(HGeomVolume* volu,
          const HGeomTransform& dTC,const HGeomTransform& mTR) {
  if (!intrinsicRot) intrinsicRot=new HGeomRotation();
  if (!center) center=new HGeomTransform();
  if (!position) position=new HGeomTransform();
  HGeomVector trans=*(volu->getPoint(5)) + *(volu->getPoint(3));
  trans*=0.5;
  center->setTransVector(trans);
  center->setRotMatrix(*intrinsicRot);
  posInMother(dTC,mTR);
  return this;
}





