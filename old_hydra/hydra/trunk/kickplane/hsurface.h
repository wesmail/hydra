#ifndef HSURFACE_H
#define HSURFACE_H
using namespace std;
#include "TObject.h"
#include "TArrayD.h"
#include <iostream> 
#include <iomanip>

class HGeomVector;

class HSurface : public TObject {
public:
  HSurface(void) {}
  ~HSurface(void) {}
  virtual HSurface *clone(void)=0;
  virtual void printTo(ostream &out)=0;
  virtual void readFrom(istream &in)=0;
  virtual Int_t getParams(TArrayD&)=0;
  virtual Bool_t setParams(TArrayD&)=0;
  virtual Double_t distanceTo(const HGeomVector &v)=0;
  virtual void calcIntersection(const HGeomVector &r,const HGeomVector &alpha,
				HGeomVector &out) const =0;
  ClassDef(HSurface,1) //Defines an abstract surface for the kick plane
};

class HSurfPlane : public HSurface {
protected:
  Double_t fDx,fDz,fC; //Plane equation: y=fDx*x + fDz*z + fC
public:
  HSurfPlane(void) { setParams(0,0,0); }
  HSurfPlane(const HSurfPlane &pl);
  ~HSurfPlane(void) {}
  virtual HSurface *clone(void);
  Double_t distanceTo(const HGeomVector &v);
  void calcIntersection(const HGeomVector &r,const HGeomVector &alpha,
			HGeomVector &out) const;
  inline void setParams(const Double_t dx, const Double_t dz, const Double_t c);
  void printTo(ostream &out) { out << fDx << " " << fDz << " " << fC << endl; }
  void readFrom(istream &in) { in >> fDx >> fDz >> fC; }
  inline Int_t getParams(TArrayD&);
  inline Bool_t setParams(TArrayD&);
  ClassDef(HSurfPlane,1) //Defines a plane surface
};

class HSurfPlaneV : public HSurface {
protected:
  Double_t fDx,fDz,fC; //Plane equation: y=fDx*x + fDz*z + fC
public:
  HSurfPlaneV(void) { setParams(0,0,0); }
  HSurfPlaneV(const HSurfPlaneV &pl);
  ~HSurfPlaneV(void) {}
  virtual HSurface *clone(void);
  Double_t distanceTo(const HGeomVector &v);
  void calcIntersection(const HGeomVector &r,const HGeomVector &alpha,
			HGeomVector &out) const;
  inline void setParams(const Double_t dx, const Double_t dz, const Double_t c);
  void printTo(ostream &out) { out << fDx << " " << fDz << " " << fC << endl; }
  void readFrom(istream &in) { in >> fDx >> fDz >> fC; }
  inline Int_t getParams(TArrayD&);
  inline Bool_t setParams(TArrayD&);
  ClassDef(HSurfPlaneV,1) //Defines a plane surface
};

class HSurfPlaneTriV : public HSurface {
protected:
  Double_t fDzLimit[3];
  Double_t fDx,fDz[3],fC[3]; //Plane equation: y=fDx*x + fDz*z + fC
  void calcC(void);
public:
  HSurfPlaneTriV(void);
  HSurfPlaneTriV(const HSurfPlaneTriV &pl);
  ~HSurfPlaneTriV(void) {}
  virtual HSurface *clone(void);
  Double_t distanceTo(const HGeomVector &v);
  void calcIntersection(const HGeomVector &r,const HGeomVector &alpha,
			HGeomVector &out) const;
  void printTo(ostream &out);
  void readFrom(istream &in);
  inline Int_t getParams(TArrayD&);
  inline Bool_t setParams(TArrayD&);
  ClassDef(HSurfPlaneTriV,1) //Defines a plane surface
};

class HSurfXCilinder : public HSurface {
protected:
  Double_t fDx,fDz,fC; // y=fDx*x*x + fDz*z +fC;
public:
  HSurfXCilinder(void) { setParams(0,0,0); }
  ~HSurfXCilinder(void) {}
  HSurfXCilinder(const HSurfXCilinder &ci);
  virtual HSurface *clone(void);
  Double_t distanceTo(const HGeomVector &v);
  void calcIntersection(const HGeomVector &r,const HGeomVector &alpha,
			HGeomVector &out) const;
  inline void setParams(const Double_t dx,const Double_t dz, const Double_t c);
  void printTo(ostream &out) { out << fDx << " " << fDz << " " << fC << endl; }
  void readFrom(istream &in) { in >> fDx >> fDz >> fC; }
  inline Int_t getParams(TArrayD&);
  inline Bool_t setParams(TArrayD&);
  ClassDef(HSurfXCilinder,1)
};

//******* Inlines

inline void HSurfPlane::setParams(const Double_t dx,const Double_t dz,
			   const Double_t c) {
  fDx=dx; fC=c; fDz=dz;
}

inline void HSurfPlaneV::setParams(const Double_t dx,const Double_t dz,
			    const Double_t c) {
  fDx=dx; fC=c; fDz=dz;
}

inline void HSurfXCilinder::setParams(const Double_t dx,const Double_t dz,
			       const Double_t c) {
  fDx=dx; fC=c; fDz=dz;
}

inline Int_t HSurfPlane::getParams(TArrayD& array) {
  array.AddAt(fDx,0); array.AddAt(fDz,1); array.AddAt(fC,2);
  return 3;
} 

inline Int_t HSurfPlaneV::getParams(TArrayD& array) {
  array.AddAt(fDx,0); array.AddAt(fDz,1); array.AddAt(fC,2);
  return 3;
} 

inline Int_t HSurfPlaneTriV::getParams(TArrayD& array) {
  array.AddAt(fDx,0);
  for (Int_t i=0;i<3;i++) array.AddAt(fDz[i],i+1);
  array.AddAt(fC[0],4);
  return 5;
}

inline Int_t HSurfXCilinder::getParams(TArrayD& array) {
  array.AddAt(fDx,0); array.AddAt(fDz,1); array.AddAt(fC,2);
  return 3;
} 

inline Bool_t HSurfPlane::setParams(TArrayD& array) {
  if (array.GetSize()==3) {
    fDx=array.At(0); fDz=array.At(1); fC=array.At(2);
    return kTRUE;
  }
  return kFALSE;
} 

inline Bool_t HSurfPlaneV::setParams(TArrayD& array) {
  if (array.GetSize()==3) {
    fDx=array.At(0); fDz=array.At(1); fC=array.At(2);
    return kTRUE;
  }
  return kFALSE;
} 

inline Bool_t HSurfXCilinder::setParams(TArrayD& array) {
  if (array.GetSize()==3) {
    fDx=array.At(0); fDz=array.At(1); fC=array.At(2);
    return kTRUE;
  }
  return kFALSE;
} 

inline Bool_t HSurfPlaneTriV::setParams(TArrayD& array) {
  if (array.GetSize() == 5) {
    fDx = array.At(0);
    for (Int_t i=0;i<3;i++) fDz[i] = array.At(i+1);
    fC[0] = array.At(4);
    calcC();
    return kTRUE;
  }
  return kFALSE;
}

#endif
