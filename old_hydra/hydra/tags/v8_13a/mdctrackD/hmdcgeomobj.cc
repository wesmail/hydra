//*--- AUTHOR : Pechenov Vladimir
//*--- Modified: 16.08.05 V.Pechenov
//*--- Modified: 07.05.02 V.Pechenov
//*--- Modified: 25.02.99

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
//
// HMdcPointPlane - poin on the plane
// HMdcTrap - trapeze in vol.
// HMdcTrapPlane - trapize on the plane
// HMdcPlane - param. of project planes
//
// HMdcPointOnPlane 
//
//     This class keep point on the plane HMdcPlane
//
//     This class don't create and don't delete
//     HMdcPlane pl object.You need to set 
//     pointer to this plane by hand.
//
// HMdcLineParam
//
//     This class keep straight line parameters.
//     Parameters are two point on the two planes
//     (two HMdcPointOnPlane objects).
//
///////////////////////////////////////////////////////////////////////////////
using namespace std;
#include "hmdcgeomobj.h"
//#include <iostream> 
//#include <iomanip>

ClassImp(HMdcPointPlane)
ClassImp(HMdcTrap)
ClassImp(HMdcTrapPlane)
ClassImp(HMdcPlane)
ClassImp(HMdcPointOnPlane)
ClassImp(HMdcLineParam)

void HMdcPointPlane::print() const {
  printf("x=%7f y=%7f\n",x,y);
}

//--------------------------
HMdcTrap::HMdcTrap(const HMdcTrap& otrap) {
  // constructor creates the copy of obj. Trap
  for(Int_t i=0; i<8; i++) points[i]=otrap.points[i];
}

HGeomVector& HMdcTrap::operator[](Int_t i) {
  if(i<0 || i>7) {
    Error("operator[]","index=%i out of bounds! Set index=0", i);
    return points[0];
  }
  return points[i];
}

void HMdcTrap::copy(HMdcTrap& otrap) const {
  // copy obj "this" to otarp
  for(Int_t i=0; i<8; i++) otrap.points[i]=points[i];
}

void HMdcTrap::set(const HMdcTrap& otrap) {
  // copy obj "this" to otrap
  for(Int_t i=0; i<8; i++) points[i]=otrap.points[i];
}

void HMdcTrap::clear() {
  // copy obj "this" to otrap
  for(Int_t i=0; i<8; i++) points[i].clear();
}

void HMdcTrap::transFrom(const HGeomTransform &s) {
  for(Int_t i=0; i<8; i++) points[i]=s.transFrom(points[i]);
}

void HMdcTrap::transTo(const HGeomTransform &s) {
  for(Int_t i=0; i<8; i++) points[i]=s.transTo(points[i]);
}

void HMdcTrap::print() {
  // print Trap;
  printf("\nTrap. from 8 points:\n");
  for(Int_t i=0; i<8; i++) {
    printf("%2i) ",i);
    points[i].print();
  }
}

//------------------------------------------------------
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

//-----------------------------------------------------
void HMdcPlane::setPlanePar(const HGeomTransform& tr) { 
  const HGeomRotation& mt=tr.getRotMatrix();
  const HGeomVector& tv=tr.getTransVector();
  Double_t c=(Double_t)(mt(0)*mt(4)-mt(3)*mt(1));
  if( c==0. ) return;
  parA=(mt(3)*mt(7)-mt(6)*mt(4))/c;
  parB=(mt(6)*mt(1)-mt(0)*mt(7))/c;
  parD=parA*tv(0)+parB*tv(1)+tv(2);
}

void HMdcPlane::print() {
  printf("Plane equation: %g*x%+g*y+z=%g\n",parA,parB,parD);
}

//-----------------------------------------------------
HMdcPointOnPlane::HMdcPointOnPlane() {
  pl=0;
}

HMdcPointOnPlane::HMdcPointOnPlane(HMdcPlane* p) {
  pl=p;
}
  
HMdcPointOnPlane::HMdcPointOnPlane(HMdcPointOnPlane& p) : 
    HGeomVector(p.x,p.y,p.z) {
  pl=p.getPlane();
}

void HMdcPointOnPlane::print(void) const {
  printf("Point x=%7f y=%7f z=%7f on the plane %g*x%+g*y+z=%g\n",
      x,y,z,pl->A(),pl->B(),pl->D());
}

//-----------------------------------------------------
void HMdcLineParam::setSegmentLine(Double_t r, Double_t z,
    Double_t theta, Double_t phi) {
  Double_t cosPhi = cos(phi);
  Double_t sinPhi = sin(phi);
  Double_t x1=-r*sinPhi;   
  Double_t y1= r*cosPhi; 
  Double_t dZ=cos(theta);
  Double_t dxy=sqrt(1.-dZ*dZ)*100.;
  Double_t x2 = dxy*cosPhi+x1;
  Double_t y2 = dxy*sinPhi+y1;
  Double_t z2=dZ*100.+z;
  point1.calcPoint(x1,y1,z,x2,y2,z2);
  point2.calcPoint(x1,y1,z,x2,y2,z2);
}
