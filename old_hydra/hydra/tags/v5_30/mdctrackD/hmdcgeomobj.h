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
    Double_t getx(){return x;}
    Double_t gety(){return y;}
    void copy(HMdcPointPlane &point) {point.x=x; point.y=y;}
    void get(Double_t &xo, Double_t &yo){xo=x; yo=y;}
    void getvector(Double_t* vc) {vc[0]=x; vc[1]=y;}
    void setx(Double_t xn){x=xn;}
    void sety(Double_t yn){y=yn;}
    void set(const HMdcPointPlane& point){x=point.x; y=point.y;}
    void set(Double_t xo, Double_t yo){x=xo; y=yo;}
    void setvector(Double_t* vc){x=vc[0]; y=vc[1];}
    void clear() {x=y=0.0;}
    void print() {printf("x=%7f y=%7f\n",x,y);}
protected:

    ClassDef(HMdcPointPlane,1)
};

class HMdcTrap : public TObject {
protected:
    HGeomVector points[8];
public:
    HMdcTrap(){;}
    HMdcTrap(HMdcTrap& otrap);
    ~HMdcTrap(){;}
    HGeomVector& operator[](Int_t i);
    void print();
    void copy(HMdcTrap& otrap);
    void set(HMdcTrap& otrap);
     void clear();
    void transFrom(HGeomTransform &s);
    void transTo(HGeomTransform &s);
protected:

    ClassDef(HMdcTrap,1)
};

class HMdcTrapPlane : public TObject {
protected:
    HMdcPointPlane points[4];
public:
    HMdcTrapPlane(){;}
    HMdcTrapPlane(HMdcTrapPlane& otrap);
    ~HMdcTrapPlane(){;}
    HMdcPointPlane& operator[](Int_t i);
    void print();
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
    Float_t A() {return a;}
    Float_t B() {return b;}
    Float_t C() {return 1.;}
    Float_t D() {return d;}
    HMdcPrPlane& operator = (const HMdcPrPlane& pl) {
        a=pl.a; b=pl.b; d=pl.d; return *this;}
    void getPrPlane(HMdcPrPlane& pl) {pl.a=a; pl.b=b; pl.d=d;}
    Float_t getZOnPlane(Float_t x, Float_t y) {return d-a*x-b*y;}
    Float_t getYOnPlane(Float_t x, Float_t z) {return (b!=0.)?(d-a*x-z)/b:0.;}
    Float_t getXOnPlane(Float_t y, Float_t z) {return (a!=0.)?(d-b*y-z)/a:0.;}
    void calcIntersection(const HGeomVector &r,const HGeomVector &dir,
                                  HGeomVector &out);

    ClassDef(HMdcPrPlane,1)
};

#endif  /*!HMDCGEOMOBJ_H*/
