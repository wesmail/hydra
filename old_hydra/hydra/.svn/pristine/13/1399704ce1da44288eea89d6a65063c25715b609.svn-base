#ifndef HKICKTRACK_H
#define HKICKTRACK_H

#include "hdataobject.h"
#include "hsymmat.h"

class HKickTrack : public HDataObject
{
private:
  Float_t z;
  Float_t r;
  Float_t p;
  Float_t theta;
  Float_t phi;
  Float_t mass;
  Int_t charge;   
  HSymMat6 cov; 
  Char_t sector;

public:
  HKickTrack(void);
  HKickTrack(HKickTrack &aTrack);
  ~HKickTrack(void);
  void setZ(Float_t az,Float_t err) { z=az; cov.setErr(0,err); }
  void setR(Float_t ar,Float_t err) { r=ar; cov.setErr(1,err); }
  void setP(Float_t ap,Float_t err) { p=ap; cov.setErr(2,err); }
  void setTheta(Float_t atheta,Float_t err) {theta=atheta; cov.setErr(3,err); }
  void setPhi(Float_t aphi,Float_t err)     { phi=aphi; cov.setErr(4,err); }
  void setCharge(Int_t c)       { charge=c; }                               
  void setMass(Float_t m,Float_t err) {mass=m; cov.setErr(5,err); }
  void setSector(Int_t s) { sector = s; }
  Float_t getZ(void)        { return z; }
  Float_t getErrZ(void)     { return cov.getErr(0); }
  Float_t getR(void)        { return r; }
  Float_t getErrR(void)     { return cov.getErr(1); }
  Float_t getP(void)        { return p; }
  Float_t getErrP(void)     { return cov.getErr(2); }
  Float_t getTheta(void)    { return theta; }
  Float_t getErrTheta(void) { return cov.getErr(3); }
  Float_t getPhi(void)      { return phi; }
  Float_t getErrPhi(void)   { return cov.getErr(4); }
  Int_t getCharge(void)     { return charge; }
  Float_t getMass(void) { return mass; }
  Float_t getErrMass(void) { return cov.getErr(5); }
  Int_t getSector(void)     { return sector; }            

  ClassDef(HKickTrack,1) //A final tracki (preliminar)
};

#endif /* !HKickTRACK_H */



