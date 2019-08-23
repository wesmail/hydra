#ifndef HTOFHIT_H
#define HTOFHIT_H

#include "hdataobject.h"

class HTofHit : public HDataObject {
private:
  Int_t   index; //Index
  Float_t  x; //Average time of flight (t1+t2)/2
  Float_t  y; //X coordinate
public:
  HTofHit(void) : index(-1),x(0.0F), y(0.0F) {}
  ~HTofHit(void) {}
  Int_t getIndex(void) {return  index;}
  void setIndex(Int_t idx) { index=idx;}
  Float_t getX(void) {return  x;}
  Float_t getY(void) {return  y;}
  void setX(Float_t ax) {x=ax;}
  void setY(Float_t ay) {y=ay;}
  void setValues(Float_t ax,Float_t ay) {x=ax;y=ay;}
  int operator< (const HTofHit &hit) { return x<hit.x && y<hit.y;}
  int operator>(const HTofHit &hit) {return x>hit.x && y>hit.y;}
  int operator!=(const HTofHit &hit) { return x!=hit.x || y!=hit.y;}
  HTofHit &operator=(const HTofHit &a) {
    x=a.x; y=a.y;
    return *this;
  }
  ClassDef(HTofHit,1) //Class with data for a TOF hit
};

#endif /* !HTOFHIT_H */

