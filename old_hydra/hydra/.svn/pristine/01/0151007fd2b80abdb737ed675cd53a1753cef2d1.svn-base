//*-- AUTHOR : Ilse Koenig
//*-- Modified : 13/04/99

/////////////////////////////////////////////////////////////
// HGeomRotation
//
// This class defines a 3x3 rotation matrix.
// The data are stored in a linear array with 9 Double_t
// components.
//
/////////////////////////////////////////////////////////////

#include "hgeomrotation.h"

ClassImp(HGeomRotation)

/*
HGeomRotation::HGeomRotation() {
  rot[0]=rot[4]=rot[8]=1.;
  rot[1]=rot[2]=rot[3]=rot[5]=rot[6]=rot[7]=0.;
}

HGeomRotation::HGeomRotation(HGeomRotation& r) {
  for(Int_t i=0;i<9;i++) rot[i]=r(i);
}

HGeomRotation::HGeomRotation(Double_t* matrix) {
  for(Int_t i=0;i<9;i++) rot[i]=matrix[i];
}
  
Double_t HGeomRotation::operator () (Int_t i) const {
  if (i>=0 && i<9) return rot[i];
  Error("operator()","bad index");
  return 0;
}

HGeomRotation& HGeomRotation::operator = (const HGeomRotation& r) {
  for(Int_t i=0;i<9;i++) rot[i]=r(i);
  return *this;
}

Bool_t HGeomRotation::operator == (const HGeomRotation& r) {
  Int_t i=0;
  while (i<9) {
    if (rot[i]!=r(i)) return kFALSE;
    i++;
  }
  return kTRUE;
}

Bool_t HGeomRotation::operator != (const HGeomRotation& r) {
  Int_t i=0;
  while (i<9) {
    if (rot[i]!=r(i)) return kTRUE;
    i++;
  }
  return kFALSE;
}
 
HGeomVector HGeomRotation::operator * (const HGeomVector& v) const {
  return HGeomVector(rot[0]*v(0)+rot[1]*v(1)+rot[2]*v(2),
                     rot[3]*v(0)+rot[4]*v(1)+rot[5]*v(2),
                     rot[6]*v(0)+rot[7]*v(1)+rot[8]*v(2));
}

HGeomRotation HGeomRotation::operator * (const HGeomRotation& r) const {
  Double_t a[9];
  for(Int_t i=0;i<9;i++) a[i]=0;
  for(Int_t i=0;i<3;i++) {
    for(Int_t j=0;j<3;j++) {
      Int_t n=3*i+j;
      for(Int_t k=0;k<3;k++) a[n]+=rot[3*i+k]*r(3*k+j);
    }
  }
  return HGeomRotation(&a[0]);
}

HGeomRotation& HGeomRotation::operator *= (const HGeomRotation& r) {
  return *this=operator * (r);  
}

HGeomRotation& HGeomRotation::transform(const HGeomRotation& r) {
  return *this=r*(*this);
}

Bool_t HGeomRotation::isUnitMatrix() {
  return (rot[0]==1. && rot[1]==0. && rot[2]==0. &&
          rot[3]==0. && rot[4]==1. && rot[5]==0. &&
          rot[6]==0. && rot[7]==0. && rot[8]==1.)  ? kTRUE : kFALSE;
}

HGeomRotation HGeomRotation::inverse() const {
  Double_t a[9];
  for(Int_t i=0;i<3;i++) {
    for(Int_t j=0;j<3;j++) a[j+3*i]=rot[i+3*j];
  }
  return HGeomRotation(a); 
}

HGeomRotation& HGeomRotation::invert() {
  return *this=inverse();
}

Double_t HGeomRotation::determinant() const {
  return rot[0]*(rot[4]*rot[8]-rot[7]*rot[5])
        -rot[3]*(rot[1]*rot[8]-rot[7]*rot[2])
        +rot[6]*(rot[1]*rot[5]-rot[4]*rot[2]);
}

Double_t HGeomRotation::diff2(HGeomRotation& r) {
  // calculates the square of the difference between 2 matrices
  Double_t s=0;
  for(Int_t i=0;i<9;i++) {
    Double_t d=rot[i]-r(i);
    s+=d*d;
  }
  return s;
}

void HGeomRotation::setUnitMatrix(){
  rot[0]=rot[4]=rot[8]=1.;
  rot[1]=rot[2]=rot[3]=rot[5]=rot[6]=rot[7]=0.;
}

void HGeomRotation::setZero(){
  for(Int_t i=0;i<9;i++) rot[i]=0.;
}

void HGeomRotation::print() {
  for(Int_t i=0;i<9;i++) cout<<rot[i]<<"  ";
  cout<<'\n';
}
*/
