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

class HMdcPlane : public TObject {
  protected:
    Double_t parA;  // Plane: parA*x+parB*y+c*z=parD  (c=1 !!!)
    Double_t parB;  // parA*parA+parB*parB+c*c must be > 0 and c != 0 !!!
    Double_t parD;
  public:
    HMdcPlane() {parA=parB=parD=0.;}
    HMdcPlane(const HMdcPlane& p) {parA=p.parA; parB=p.parB; parD=p.parD;}
    virtual ~HMdcPlane(){;}
    void print() {printf("Plane equation: %g*x%+g*y+z=%g\n",parA,parB,parD);}
    void setPlanePar(const HMdcPlane& p) {parA=p.parA; parB=p.parB; parD=p.parD;}
    void setPlanePar(Double_t al, Double_t bl, Double_t cl, Double_t dl) {
        if(cl!=0.) {parA=al/cl; parB=bl/cl; parD=dl/cl;} }
    void setPlanePar(const HGeomTransform& tr);
    Double_t A() const {return parA;}
    Double_t B() const {return parB;}
    Double_t C() const {return 1.;}
    Double_t D() const {return parD;}
    HMdcPlane& operator = (const HMdcPlane& p) {
        parA=p.parA; parB=p.parB; parD=p.parD; return *this;}
    void getPlanePar(HMdcPlane& p) const {
      p.parA=parA; p.parB=parB; p.parD=parD;}
    void getPlanePar(Double_t& a,Double_t& b,Double_t& d) const {
      a=parA; b=parB; d=parD;}
    Double_t getZOnPlane(Double_t x,Double_t y) const {
      return parD-parA*x-parB*y;}
    Double_t getYOnPlane(Double_t x,Double_t z) const {
      return (parB!=0.)?(parD-parA*x-z)/parB:0.;}
    Double_t getXOnPlane(Double_t y,Double_t z) const {
      return (parA!=0.)?(parD-parB*y-z)/parA:0.;}
    inline void calcIntersection(const HGeomVector &r,const HGeomVector &dir,
                                  HGeomVector &out) const;
    inline void calcIntersection(Double_t x1,Double_t y1,Double_t z1,
      Double_t x2, Double_t y2, Double_t z2,
      Double_t& x, Double_t& y, Double_t& z) const;

    ClassDef(HMdcPlane,1)
};
    
// ---- inlines--------------------

void HMdcPlane::calcIntersection(const HGeomVector &r, const HGeomVector &dir,
                                   HGeomVector &out) const {
  // Calculating a cross of line r,dir (point in vol. and direction)
  // with plane parA*x+parB*y+z=parD
  Double_t x1=r(0);
  Double_t y1=r(1);
  Double_t z1=r(2);
  Double_t dX=dir(0);
  Double_t dY=dir(1);
  Double_t dZ=dir(2);
  Double_t del=1/(parA*dX+parB*dY+dZ);
  Double_t x=(dX*(parD-z1-parB*y1)+x1*(parB*dY+dZ))*del;
  Double_t y=(dY*(parD-z1-parA*x1)+y1*(parA*dX+dZ))*del;
  out.setXYZ(x,y,-parA*x-parB*y+parD);
}

void HMdcPlane::calcIntersection(Double_t x1,Double_t y1,Double_t z1,
      Double_t x2, Double_t y2, Double_t z2,
      Double_t& x, Double_t& y, Double_t& z) const {
  // Calcul. a cross of the line with plane parA*x+parB*y+c*z=parD
  Double_t dX=x2-x1;
  Double_t dY=y2-y1;
  Double_t dZ=z2-z1;
  Double_t del=1/(parA*dX+parB*dY+dZ);
  x=(dX*(parD-z1-parB*y1)+x1*(parB*dY+dZ))*del;
  y=(dY*(parD-z1-parA*x1)+y1*(parA*dX+dZ))*del;
  z= parD-parA*x-parB*y;
}

#endif  /*!HMDCGEOMOBJ_H*/
