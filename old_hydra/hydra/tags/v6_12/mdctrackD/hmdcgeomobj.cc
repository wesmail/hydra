///*-- AUTHOR : Pechenov Vladimir
//*-- Modified : 07/05/2002 by V.Pechenov
//*-- Modified : 25/02/99

////////////////////////////////////////////////////////////////
//
// HMdcPointPlane - poin on the plane
// HMdcTrap - trapeze in vol.
// HMdcTrap - trapize on the plane
// HMdcPrPlane - param. of project planes
//
////////////////////////////////////////////////////////////////
using namespace std;
#include "hmdcgeomobj.h"
//#include <iostream> 
//#include <iomanip>

ClassImp(HMdcPointPlane)
ClassImp(HMdcTrap)
ClassImp(HMdcTrapPlane)
ClassImp(HMdcPrPlane)

//--------------------------
HMdcTrap::HMdcTrap(const HMdcTrap& otrap) {
  // constructor creates the copy of obj. Trap
  for(Int_t i=0; i<8; i++) {
    points[i]=otrap.points[i];
  }
}

HGeomVector& HMdcTrap:: operator[](Int_t i) {
  Int_t j=i;
  if(j<0 || j>7) {
   Error("operator[]","index=%i out of bounds! Set index=0", i);
   j=0;
  }
  return points[j];
}

void HMdcTrap::copy(HMdcTrap& otrap) const {
  // copy obj "this" to otarp
  for(Int_t i=0; i<8; i++) {
    otrap.points[i]=points[i];
  }
}

void HMdcTrap::set(const HMdcTrap& otrap) {
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

void HMdcTrap::transFrom(const HGeomTransform &s) {
  for(Int_t i=0; i<8; i++) {
    points[i]=s.transFrom(points[i]);
  }
}

void HMdcTrap::transTo(const HGeomTransform &s) {
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
HMdcTrapPlane::HMdcTrapPlane(const HMdcTrapPlane& otrap) {
  // constructor creates the copy of obj. Trap
  for(Int_t i=0; i<4; i++) points[i].set(otrap.points[i]);
}

HMdcPointPlane& HMdcTrapPlane:: operator[](Int_t i) {
  Int_t j=i;
  if(j<0 || j>3) {
    Error("operator[]","index=%i out of bounds! Set index=0", i);
    j=0;
  }
  return points[j];
}

void HMdcTrapPlane::copy(HMdcTrapPlane& otrap) {
  // copy obj "this" to otarp
  for(Int_t i=0; i<4; i++) points[i].copy(otrap.points[i]);
}

void HMdcTrapPlane::set(HMdcTrapPlane& otrap) {
  // copy obj "this" to otrap
  for(Int_t i=0; i<4; i++) points[i].set(otrap.points[i]);
}

void HMdcTrapPlane::clear() {
  // copy obj "this" to otrap
  for(Int_t i=0; i<4; i++) points[i].clear();
}

void HMdcTrapPlane::print() const {
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
