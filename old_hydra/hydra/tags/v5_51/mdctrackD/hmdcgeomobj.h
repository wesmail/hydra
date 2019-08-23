#ifndef HMDCGEOMOBJ_H
#define HMDCGEOMOBJ_H

#include "TObject.h"
#include "TObjArray.h"
#include "TArrayI.h"
#include "hgeomvector.h"
#include "hgeomtransform.h"

class HMdcPointPlane : public TObject {
  protected:
    Double_t x;
    Double_t y;
  public:
    HMdcPointPlane() : x(0.F), y(0.F){;}
    HMdcPointPlane(Double_t xi, Double_t yi) {x=xi; y=yi;}
    HMdcPointPlane(const HMdcPointPlane& point) {x=point.x; y=point.y;}
    ~HMdcPointPlane(){;}
    Double_t getx() const {return x;}
    Double_t gety() const {return y;}
    void copy(HMdcPointPlane &point) const {point.x=x; point.y=y;}
    void get(Double_t &xo, Double_t &yo) const {xo=x; yo=y;}
    void getvector(Double_t* vc) const {vc[0]=x; vc[1]=y;}
    void setx(Double_t xn){x=xn;}
    void sety(Double_t yn){y=yn;}
    void set(const HMdcPointPlane& point){x=point.x; y=point.y;}
    void set(Double_t xo, Double_t yo){x=xo; y=yo;}
    void setvector(Double_t* vc){x=vc[0]; y=vc[1];}
    void clear() {x=y=0.0;}
    void print() const {printf("x=%7f y=%7f\n",x,y);}
  protected:

    ClassDef(HMdcPointPlane,1)
};

class HMdcTrap : public TObject {
  protected:
    HGeomVector points[8];
  public:
    HMdcTrap(){;}
    HMdcTrap(const HMdcTrap& otrap);
    ~HMdcTrap(){;}
    HGeomVector& operator[](Int_t i);
    void print();
    void copy(HMdcTrap& otrap) const;
    void set(const HMdcTrap& otrap);
     void clear();
    void transFrom(const HGeomTransform &s);
    void transTo(const HGeomTransform &s);
  protected:

    ClassDef(HMdcTrap,1)
};

class HMdcTrapPlane : public TObject {
  protected:
    HMdcPointPlane points[4];
  public:
    HMdcTrapPlane(){;}
    HMdcTrapPlane(const HMdcTrapPlane& otrap);
    ~HMdcTrapPlane(){;}
    HMdcPointPlane& operator[](Int_t i);
    void print() const;
    void copy(HMdcTrapPlane& otrap);
    void set(HMdcTrapPlane& otrap);
    void clear();
  protected:

    ClassDef(HMdcTrapPlane,1)
};

class HMdcPrPlane {
  protected:
    Float_t a;  // Plane: a*x+b*y+c*z=d  (c=1 !!!)
    Float_t b;  // a*a+b*b+c*c must be > 0 and c != 0 !!!
    Float_t d;
  public:
    HMdcPrPlane() {a=b=d=0.;}
    HMdcPrPlane(const HMdcPrPlane& pl) {a=pl.a; b=pl.b; d=pl.d;}
    virtual ~HMdcPrPlane(){;}
    void print() {printf("Plane equation: %g*x%+g*y+z=%g\n",a,b,d);}
    void setPrPlane(const HMdcPrPlane& pl) {a=pl.a; b=pl.b; d=pl.d;}
    void setPrPlane(Float_t al, Float_t bl, Float_t cl, Float_t dl) {
        if(cl!=0.) {a=al/cl; b=bl/cl; d=dl/cl;} }
    void setPrPlane(const HGeomTransform& tr);
    Float_t A() const {return a;}
    Float_t B() const {return b;}
    Float_t C() const {return 1.;}
    Float_t D() const {return d;}
    HMdcPrPlane& operator = (const HMdcPrPlane& pl) {
        a=pl.a; b=pl.b; d=pl.d; return *this;}
    void getPrPlane(HMdcPrPlane& pl) const {pl.a=a; pl.b=b; pl.d=d;}
    Float_t getZOnPlane(Float_t x,Float_t y) const {return d-a*x-b*y;}
    Float_t getYOnPlane(Float_t x,Float_t z) const {return (b!=0.)?(d-a*x-z)/b:0.;}
    Float_t getXOnPlane(Float_t y,Float_t z) const {return (a!=0.)?(d-b*y-z)/a:0.;}
    void calcIntersection(const HGeomVector &r,const HGeomVector &dir,
                                  HGeomVector &out) const;

    ClassDef(HMdcPrPlane,1)
};

#endif  /*!HMDCGEOMOBJ_H*/
