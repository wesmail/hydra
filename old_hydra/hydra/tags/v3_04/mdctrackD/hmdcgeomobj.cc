///*-- AUTHOR : Pechenov Vladimir
//*-- Modified : 25/02/99

////////////////////////////////////////////////////////////////
// HMdcCalcLookUp
//
// Container class for Mdc transformations and sizes parameters
//
////////////////////////////////////////////////////////////////

#include "hmdcgeomobj.h"
//#include <iostream.h>

ClassImp(HMdcPointPlane)
ClassImp(HMdcGeomObj)
ClassImp(HMdcLineP)
ClassImp(HMdcTrap)
ClassImp(HMdcTrapPlane)


HMdcPointPlane::HMdcPointPlane(Double_t xi, Double_t yi) {
  // constructor creates the copy of obj. point
  x=xi;
  y=yi;
}

HMdcPointPlane::HMdcPointPlane(const HMdcPointPlane& point) {
  // constructor creates the copy of obj. point
  x=point.x;
  y=point.y;
}

void HMdcPointPlane::copy(HMdcPointPlane& point) {
  // constructor creates the copy of obj. point
  point.x=x;
  point.y=y;
}

void HMdcPointPlane::get(Double_t &xo, Double_t &yo) {
  // constructor creates the copy of obj. point
  xo=x;
  yo=y;
}

void HMdcPointPlane::getvector(Double_t *vc) {
  vc[0]=x;
  vc[1]=y;
}

void HMdcPointPlane::set(const HMdcPointPlane& point) {
  // constructor creates the copy of obj. point
  x=point.x;
  y=point.y;
}

void HMdcPointPlane::set(Double_t xo, Double_t yo) {
  x=xo;
  y=yo;
}

void HMdcPointPlane::setvector(Double_t *vc) {
  x=vc[0];
  y=vc[1];
}

void HMdcPointPlane::clear() {
  x=0.0;
  y=0.0;
}

void HMdcPointPlane::print() {
  printf("x=%7f y=%7f\n",x,y);
}

//--------------------------------------------------------------

HMdcGeomObj::HMdcGeomObj(Int_t npoint=1) {
  // constructor creates the copy of obj. LineP
  numobj=npoint;
  points = new TObjArray(numobj);
  for(Int_t i=0; i<numobj; i++) {
   (*points)[i]=new HGeomVector();
  }
}

HMdcGeomObj::HMdcGeomObj(HMdcGeomObj& ogeomobj) {
  // constructor creates the copy of obj. GeomObj
  numobj=ogeomobj.numobj;
  points = new TObjArray(numobj);
  for(Int_t i=0; i<numobj; i++) {
    (*points)[i]=new HGeomVector(ogeomobj[i]);
  }
}

HMdcGeomObj::~HMdcGeomObj() {
 points->Delete ();
}

Bool_t HMdcGeomObj::copy(HMdcGeomObj& ogeomobj) {
  // copy obj "this" to ogeomobj
  if(ogeomobj.numobj != numobj) return kFALSE;
  for(Int_t i=0; i<numobj; i++) {
    ogeomobj[i]=(*this)[i];
  }
  return kTRUE;
}

Bool_t HMdcGeomObj::set(HMdcGeomObj& ogeomobj) {
  // copy obj "this" to ogeomobj
  if(ogeomobj.numobj != numobj) return kFALSE;
  for(Int_t i=0; i<numobj; i++) {
    ogeomobj[i]=(*this)[i];
  }
  return kTRUE;
}

void HMdcGeomObj::clear() {
  // clear obj "this"
  for(Int_t i=0; i<numobj; i++) {
    (*this)[i].clear();
  }
}

void HMdcGeomObj::transFrom(HGeomTransform &s) {
  for(Int_t i=0; i<numobj; i++) {
    (*this)[i]=s.transFrom((*this)[i]);
  }
}

void HMdcGeomObj::transTo(HGeomTransform &s) {
  for(Int_t i=0; i<numobj; i++) {
    (*this)[i]=s.transTo((*this)[i]);
  }
}

void HMdcGeomObj::print() {
  // print GeomObj;
  printf("\nGeometric object consisting from %i points:\n",numobj);
  for(Int_t i=0; i<numobj; i++) {
    printf("%2i) ",i);
    (*this)[i].print();
  }
}


//--------------------------
HMdcLineP::HMdcLineP(HMdcLineP& olinep) {
  // constructor creates the copy of obj. LineP
  points[0]=olinep.points[0];
  points[1]=olinep.points[1];
}

HGeomVector& HMdcLineP:: operator[](Int_t i) {
  Int_t j=i;
  if(j<0 || j>1) {
   printf("Error:: index for HMdcLineP::operator[%i] out of bounds! Set index=0\n", i);
   j=0;
  }  
  return points[j];
}

void HMdcLineP::copy(HMdcLineP& olinep) {
  // copy obj "this" to olinep
  olinep.points[0]=points[0];
  olinep.points[1]=points[1];
}

void HMdcLineP::set(HMdcLineP& olinep) {
  // copy obj "this" to oline
  points[0]=olinep.points[0];
  points[1]=olinep.points[1];
}

void HMdcLineP::clear() {
  // clear obj "this" to oline
  points[0].clear();
  points[1].clear();
}

void HMdcLineP::transFrom(HGeomTransform &s) {
  points[0]=s.transFrom(points[0]);
  points[1]=s.transFrom(points[1]);
}

void HMdcLineP::transTo(HGeomTransform &s) {
  points[0]=s.transTo(points[0]);
  points[1]=s.transTo(points[1]);
}

void HMdcLineP::print() {
  // print GeomObj;
  printf("\nLine from 2 points:\n");
  for(Int_t i=0; i<2; i++) {
    printf("%2i) ",i);
    points[i].print();
  }
}

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

