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
    HMdcPointPlane(Double_t xi, Double_t yi);
    HMdcPointPlane(const HMdcPointPlane& point); 
    ~HMdcPointPlane(){;}
    Double_t getx(){return x;}
    Double_t gety(){return y;}
    void copy(HMdcPointPlane &point);
    void get(Double_t &xo, Double_t &yo);
    void getvector(Double_t* vc);
    void setx(Double_t xn){x=xn;}
    void sety(Double_t yn){y=yn;}
    void set(const HMdcPointPlane& point);
    void set(Double_t xo, Double_t yo);
    void setvector(Double_t* vc);
    void clear();
    void print();
protected:

    ClassDef(HMdcPointPlane,1)
};
 
class HMdcGeomObj : public TObject {
protected:
    TObjArray* points;
    Int_t numobj;
public:
    HMdcGeomObj(Int_t nobj=1);
    HMdcGeomObj(HMdcGeomObj& ogeomobj);  
    ~HMdcGeomObj();
    HGeomVector& operator[](Int_t i) {
      return *static_cast<HGeomVector*>((*points)[i]);
    }
    void print();
    Bool_t copy(HMdcGeomObj& ogeomobj);
    Bool_t set(HMdcGeomObj& ogeomobj);
    void transFrom(HGeomTransform &s);
    void transTo(HGeomTransform &s);
    void clear();
protected:

    ClassDef(HMdcGeomObj,1)
};

class HMdcLineP : public TObject {
protected:
    HGeomVector points[2];
public:
    HMdcLineP(){;}
    HMdcLineP(HMdcLineP& olinep);  
    ~HMdcLineP(){;}
    HGeomVector& operator[](Int_t i);
    void print();
    void copy(HMdcLineP& olinep);
    void set(HMdcLineP& olinep);
    void transFrom(HGeomTransform &s);
    void transTo(HGeomTransform &s);
//    HGeomVector& crossLineToPlane(HGeomTransform &s);
    void clear();
protected:

    ClassDef(HMdcLineP,1)
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

#endif  /*!HMDCGEOMOBJ_H*/
