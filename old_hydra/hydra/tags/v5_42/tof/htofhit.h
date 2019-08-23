#ifndef HTOFHIT_H
#define HTOFHIT_H

#include <TObject.h>

class HTofHit : public TObject {
private:
  Int_t   index; //Index
  Float_t  tof; //time-of-flight
  Float_t  xpos; //X coordinate
  Float_t  xlab; //X in the lab system
  Float_t  ylab; //Y in the lab system
  Float_t  zlab; //Z in the lab system
  Float_t  dist; //distance from the target to the hit (in mm)
  Float_t  theta; //theta angle of the hit (in deg)
  Float_t  phi;   //phi angle of the hit (in deg)  
  Char_t  sector, module, cell; //Location

public:
  HTofHit(void) : index(-1),tof(0.0F), xpos(0.0F) {}
  ~HTofHit(void) {}
  Int_t getIndex(void) {return  index;}
  void setIndex(Int_t idx) { index=idx;}
  Float_t getTof(void) {return tof;}
  Float_t getXpos(void) {return  xpos;}
  void setTof(Float_t atof) {tof=atof;}
  void setXpos(Float_t axpos) {xpos=axpos;}
  void setXYZLab(Float_t x, Float_t y, Float_t z) { 
    xlab = x; ylab = y; zlab = z;
  }
  void getXYZLab(Float_t &x, Float_t &y, Float_t &z) {
    x = xlab; y = ylab; z = zlab;
  }
  void setDistance(Float_t d) { dist = d; }
  void getDistance(Float_t &d) { d = dist; }
  void setTheta(Float_t th) { theta = th; }
  void getTheta(Float_t &th) { th = theta; }
  void setPhi(Float_t ph) { phi = ph; }
  void getPhi(Float_t &ph) { ph = phi; }
  void setValues(Float_t atof,Float_t axpos) {tof=atof;xpos=axpos;}
  Char_t getSector(void) {return  sector;}
  Char_t getModule(void) {return  module;}
  Char_t getCell(void) {return  cell;}  
  void setSector(Char_t s) { sector=s;}
  void setModule(Char_t m) { module=m;}
  void setCell(Char_t c) { cell=c;}
  int operator< (const HTofHit &hit) { return tof<hit.tof && xpos<hit.xpos;}
  int operator>(const HTofHit &hit) {return tof>hit.tof && xpos>hit.xpos;}
  int operator!=(const HTofHit &hit) { return tof!=hit.tof || xpos!=hit.xpos;}
  HTofHit &operator=(const HTofHit &a) {
    tof=a.tof; xpos=a.xpos;
    return *this;
  }
  ClassDef(HTofHit,1) //Class with data for a TOF hit
};

#endif /* !HTOFHIT_H */

