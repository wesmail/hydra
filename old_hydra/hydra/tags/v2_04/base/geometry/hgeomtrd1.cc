//*-- AUTHOR : Ilse Koenig
//*-- Modified : 16/05/99 by Ilse Koenig

/////////////////////////////////////////////////////////////
//
// HGeomTrd1
//
// class for the GEANT shape TRD1
// 
// The technical coordinate system of a TRD1, which sits in
// CAVE and is not rotated, is the laboratory system.
// The y-axis points from the smaller side to the larger one.
// That's the same definitition as for a TRAP and different from
// the Geant or ROOT definition for a TRD1.
// Therefore a transformation is needed:
//              x-technical = - (x-Geant)
//              y-technical = z-Geant
//              z-technical = y-Geant
// This is stored in the data element intrinsicRot which is
// created in the function calcVoluPosition(...)
// 
/////////////////////////////////////////////////////////////

#include "hgeomtrd1.h"
#include "hgeomvolume.h"
#include "hgeomvector.h"

ClassImp(HGeomTrd1)

HGeomTrd1::HGeomTrd1() {
  fName="TRD1";
  nPoints=8;
  nParam=4;
}


HGeomTrd1::~HGeomTrd1() {
  if (param) { delete param; param=0; }
  if (intrinsicRot) { delete intrinsicRot; param=0; }
  if (center) { delete center; param=0; }
  if (position) { delete position; param=0; }
}


HGeomBasicShape* HGeomTrd1::calcVoluParam(HGeomVolume* volu) {
  Double_t fac=20.;
  if (!param) param=new TArrayD(nParam);
  HGeomVector& v0=*(volu->getPoint(0));
  HGeomVector& v1=*(volu->getPoint(2));
  HGeomVector& v2=*(volu->getPoint(1));
  HGeomVector& v3=*(volu->getPoint(3));
  HGeomVector& v4=*(volu->getPoint(4));
  param->AddAt(TMath::Abs((v0(0)-v3(0))/fac),0);
  param->AddAt((v1(0)-v2(0))/fac,1);
  param->AddAt((v4(2)-v0(2))/fac,2);
  param->AddAt((v1(1)-v0(1))/fac,3);
  return this;
} 


HGeomBasicShape* HGeomTrd1::calcVoluPosition(HGeomVolume* volu,
            const HGeomTransform& dTC,const HGeomTransform& mTR) {
  if (!intrinsicRot) intrinsicRot=new HGeomRotation();
  if (!center) center=new HGeomTransform();
  if(!position) position=new HGeomTransform();
  Double_t t[3]={0.,0.,0.};
  for(Int_t i=0;i<4;i++) t[0]+=(*(volu->getPoint(i)))(0);
  t[0]/=4.;
  HGeomVector& v0=*(volu->getPoint(0));
  HGeomVector& v1=*(volu->getPoint(1));
  HGeomVector& v4=*(volu->getPoint(4));
  t[1]=(v1(1)-v0(1))/2.;
  t[2]=(v4(2)-v0(2))/2.;
  center->setTransVector(t);
  intrinsicRot->setZero();
  intrinsicRot->setElement(-1.,0);
  intrinsicRot->setElement(1.,5);
  intrinsicRot->setElement(1.,7);
  center->setRotMatrix(*intrinsicRot);
  posInMother(dTC,mTR);
  return this;
}

