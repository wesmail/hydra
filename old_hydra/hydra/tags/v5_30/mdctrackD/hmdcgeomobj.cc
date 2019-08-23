///*-- AUTHOR : Pechenov Vladimir
//*-- Modified : 25/02/99

////////////////////////////////////////////////////////////////
//
// HMdcPointPlane - poin on the plane
// HMdcTrap - trapeze in vol.
// HMdcTrap - trapize on the plane
// HMdcPrPlane - param. of project planes
//
////////////////////////////////////////////////////////////////

#include "hmdcgeomobj.h"
//#include <iostream.h>

ClassImp(HMdcPointPlane)
ClassImp(HMdcTrap)
ClassImp(HMdcTrapPlane)
ClassImp(HMdcPrPlane)

//--------------------------
HMdcTrap::HMdcTrap(HMdcTrap& otrap) {
  // constructor creates the copy of obj. Trap
  for(Int_t i=0; i<8; i++) {
    points[i]=otrap.points[i];
  }
}

HGeomVector& HMdcTrap:: operator[](Int_t i) {
  Int_t j=i;
  if(j<0 || j>7) {
   printf("Error:: index for HMdcTrap::operator[%i] out of bounds! Set index=0\n", i);
   j=0;
  }
  return points[j];
}

void HMdcTrap::copy(HMdcTrap& otrap) {
  // copy obj "this" to otarp
  for(Int_t i=0; i<8; i++) {
    otrap.points[i]=points[i];
  }
}

void HMdcTrap::set(HMdcTrap& otrap) {
  // copy obj "this" to otrap
  for(Int_t i=0; i<8; i++) {
    points[i]=otrap.points[i];
  }
}

void HMdcTrap::clear() {
  // copy obj "this" to otrap
  for(Int_t i=0; i<8; i++) {
    points[i].clear();
  }
}

void HMdcTrap::transFrom(HGeomTransform &s) {
  for(Int_t i=0; i<8; i++) {
    points[i]=s.transFrom(points[i]);
  }
}

void HMdcTrap::transTo(HGeomTransform &s) {
  for(Int_t i=0; i<8; i++) {
    points[i]=s.transTo(points[i]);
  }
}

void HMdcTrap::print() {
  // print Trap;
  printf("\nTrap. from 8 points:\n");
  for(Int_t i=0; i<8; i++) {
    printf("%2i) ",i);
    points[i].print();
  }
}

//--------------------------
HMdcTrapPlane::HMdcTrapPlane(HMdcTrapPlane& otrap) {
  // constructor creates the copy of obj. Trap
  for(Int_t i=0; i<4; i++) {
    points[i].set(otrap.points[i]);
  }
}

HMdcPointPlane& HMdcTrapPlane:: operator[](Int_t i) {
  Int_t j=i;
  if(j<0 || j>3) {
   printf("Error:: index for HMdcTrapPlane::operator[%i] out of bounds! Set index=0\n", i);
   j=0;
  }
  return points[j];
}

void HMdcTrapPlane::copy(HMdcTrapPlane& otrap) {
  // copy obj "this" to otarp
  for(Int_t i=0; i<4; i++) {
    points[i].copy(otrap.points[i]);
  }
}

void HMdcTrapPlane::set(HMdcTrapPlane& otrap) {
  // copy obj "this" to otrap
  for(Int_t i=0; i<4; i++) {
    points[i].set(otrap.points[i]);
  }
}

void HMdcTrapPlane::clear() {
  // copy obj "this" to otrap
  for(Int_t i=0; i<4; i++) {
    points[i].clear();
  }
}

void HMdcTrapPlane::print() {
  // print Trap;
  printf("\nTrap. on the plane from 4 points:\n");
  for(Int_t i=0; i<4; i++) {
    printf("%2i) ",i);
    points[i].print();
  }
}

//--------------------------
void HMdcPrPlane::setPrPlane(const HGeomTransform& tr) { 
  const HGeomRotation& mt=tr.getRotMatrix();
  const HGeomVector& tv=tr.getTransVector();
  Float_t c=(Float_t)(mt(0)*mt(4)-mt(3)*mt(1));
  if( c==0. ) return;
  a=(mt(3)*mt(7)-mt(6)*mt(4))/c;
  b=(mt(6)*mt(1)-mt(0)*mt(7))/c;
  d=a*tv(0)+b*tv(1)+tv(2);
}

void HMdcPrPlane::calcIntersection(const HGeomVector &r, const HGeomVector &dir,
                                   HGeomVector &out) {
  // Calculeting a cross of line r,d (point in vol. and direction)
  // with plane a*x+b*y+c*z=d
  Double_t cdZ=dir.getZ();
  Double_t bdY=b*dir.getY();
  Double_t crZ=r.getZ();
  Double_t bdYcdZ=bdY+cdZ;
  Double_t del=a*dir.getX()+bdYcdZ;
  out.setX((del!=0.) ? (r.getX()*bdYcdZ-(b*r.getY()+crZ-d)*dir.getX())/del :0.);
  out.setY((bdYcdZ!=0.) ? (r.getY()*cdZ-(a*out.X()+crZ-d)*dir.getY())/bdYcdZ :0.);
  out.setZ( -a*out.X()-b*out.Y()+d );
}
