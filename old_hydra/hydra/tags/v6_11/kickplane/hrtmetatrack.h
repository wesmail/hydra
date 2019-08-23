#ifndef HRTMETATRACK_H
#define HRTMETATRACK_H

#include "hgeomvector.h"
#include <TObject.h>

class HRtMetaTrack : public TObject {
protected:
  Float_t mass2;
  Float_t mass2Meta;
  Float_t mass2Mdc3;
  Float_t p;
  Float_t pMdc3;
  Float_t pMeta;
  Float_t tof;
  Float_t d;
  Float_t pull;
  Int_t system;
  Float_t resX,resY;
  Float_t theta,phi;
  Float_t dKick,dTrack;
  Float_t r,z;
  Float_t scaling;
public:
  HRtMetaTrack(void) : tof(-1) {}
  ~HRtMetaTrack(void) {}
  void set(Float_t ap, Float_t m2, Float_t atof, Float_t dis) {
    p = ap;
    tof = atof;
    d = dis;
    mass2 = m2;
  }

  void setTof(Float_t t) { tof = t; }
  Float_t getTof(void) { return tof; }
  void setDist(Float_t t) { d = t; }
  Float_t getDist(void) { return d; }
  void setPull(Float_t t) { pull = t; }
  Float_t getPull(void) { return pull; }
  void setSystem(Int_t t) { system = t; }
  Float_t getSystem(void) { return system; }
  void setResXY(Float_t rx,Float_t ry) { resX=rx; resY=ry; }
  void setPMdc3(Float_t p) { pMdc3 = p; }
  void setPMeta(Float_t p) { pMeta = p; }
  Float_t getMeta(void) { return pMeta; }
  Float_t getMdc3(void) { return pMdc3; }
  void setMass(Float_t meta,Float_t mdc3) 
    { mass2Meta = meta; mass2Mdc3 = mdc3; }
  void setDirection(Float_t t,Float_t p) { theta = t; phi = p; } 
  void setDistances(Float_t dk,Float_t dt) { dKick=dk; dTrack=dt; }
  void setTargetPos(Float_t ar,Float_t az) { r=ar; z=az; }
  void setScaling(Float_t ss) { scaling = ss; }
  ClassDef(HRtMetaTrack,1)
};

#endif
