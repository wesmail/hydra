//*-- AUTHOR : Ilse Koenig
//*-- Modified : 12/04/99

/////////////////////////////////////////////////////////////
// HGeomVector
//
// This class defines a vector with 3 Double_t components.
// Instantiations of this class are e.g geometry points and
// translation vectors in the geometry transformations.
//
/////////////////////////////////////////////////////////////

#include "hgeomvector.h"

ClassImp(HGeomVector)

/*
Double_t HGeomVector::operator() (const Int_t i) const {
  switch (i) {
    case 0: return x;
    case 1: return y;
    case 2: return z;
    default: Error("operator()","bad index");
  }
  return 0;
}

HGeomVector HGeomVector::operator - () const {
  HGeomVector p(-x,-y,-z);
  return p;
}

HGeomVector& HGeomVector::operator = (const HGeomVector& v) {
  x=v.getX();
  y=v.getY();
  z=v.getZ();
  return *this;
}

Bool_t HGeomVector::operator == (const HGeomVector& v) const {
  return ((v.getX()!=x || v.getY()!=y || v.getZ()!=z) ? kFALSE : kTRUE); 
}

Bool_t HGeomVector::operator != (const HGeomVector& v) const {
  return (v.getX()!=x || v.getY()!=y || v.getZ()!=z)  ? kTRUE : kFALSE; 
}

Bool_t HGeomVector::operator < (const Double_t a) {
  return (x>a || y>a || z>a) ? kFALSE : kTRUE;
}

Bool_t HGeomVector::operator <= (const Double_t a) {
  return (x>a || y>a || z>a) ? kFALSE : kTRUE;
}

Bool_t HGeomVector::operator > (const Double_t a) {
  return (x<=a || y<=a || z<=a) ? kFALSE : kTRUE;
}

Bool_t HGeomVector::operator >= (const Double_t a) {
  return (x<a || y<a || z<a) ? kFALSE : kTRUE;
}

HGeomVector& HGeomVector::operator += (const Double_t a) {
  x+=a;
  y+=a;
  z+=a;
  return *this;
}

HGeomVector& HGeomVector::operator -= (const Double_t a) {
  x-=a;
  y-=a;
  z-=a;
  return *this;
}

HGeomVector& HGeomVector::operator *= (const Double_t a) {
  x*=a;
  y*=a;
  z*=a;
  return *this;
}

HGeomVector& HGeomVector::operator /= (const Double_t a) {
  x/=a;
  y/=a;
  z/=a;
  return *this;
}

HGeomVector& HGeomVector::operator += (const HGeomVector& v) {
  x+=v.getX();
  y+=v.getY();
  z+=v.getZ();
  return *this;
}

HGeomVector& HGeomVector::operator -= (const HGeomVector& v) {
  x-=v.getX();
  y-=v.getY();
  z-=v.getZ();
  return *this;
}

HGeomVector HGeomVector::operator + (const HGeomVector& v) const {
  HGeomVector p(*this);
  return p+=v;
}

HGeomVector HGeomVector::operator - (const HGeomVector& v) const {
  HGeomVector p(*this);
  return p-=v;
}

HGeomVector& HGeomVector::abs() {
  x=TMath::Abs(x);
  y=TMath::Abs(y);
  z=TMath::Abs(z);
  return *this;
}

Double_t HGeomVector::scalarProduct(const HGeomVector& v) const {
  return (x*v.getX()+x*v.getY()+x*v.getZ());
}

HGeomVector HGeomVector::vectorProduct(const HGeomVector& v) const {
  HGeomVector p(y*v.getZ()-z*v.getY(),z*v.getX()-x*v.getZ(),
                x*v.getY()-y*v.getX());
  return p;
}

void HGeomVector::round(Double_t d,Int_t n) {
  // rounds d to a precision with n digits
  if(d>0) d=floor(d*pow(10,n)+0.5)/pow(10,n);
  else d=-floor((-d)*pow(10,n)+0.5)/pow(10,n);
}

void HGeomVector::round(Int_t n) {
  // rounds every component to a precision with n digits
  round(x,n);
  round(y,n);
  round(z,n);
}
*/
