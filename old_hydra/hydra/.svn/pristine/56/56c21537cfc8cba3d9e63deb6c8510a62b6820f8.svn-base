#ifndef HKICKTRACK_H
#define HKICKTRACK_H

#include "hdataobject.h"
#include "hsymmat.h"

class HKickTrack : public HDataObject
{
private:
  Float_t pTof; //Momentum using Time Of Flight
  Float_t errPTof;
  Char_t pid;
  Char_t quality;
  Float_t z;
  Float_t r;
  Float_t p;
  Float_t theta;
  Float_t phi;
  Float_t mass;
  Int_t charge;   
  HSymMat6 cov; 
  Char_t sector;
  Int_t system;
  Float_t tof;
  Float_t beta;
  Float_t showerSum10,showerSum20;
  Short_t segmentId;
  Short_t ringId;
  Short_t outerHitId;
public:
  static const Char_t antiProtonId;
  static const Char_t protonId;
  static const Char_t kPlusId;
  static const Char_t kMinusId;
  static const Char_t piPlusId;
  static const Char_t piMinusId;
  static const Char_t unknownId;
  Float_t pull;
  HKickTrack(void);
  HKickTrack(HKickTrack &aTrack);
  ~HKickTrack(void);
  void setSegment(Int_t i) { segmentId = i; }
  void setRingId(Int_t i) { ringId = i; }
  void setOuterHitId(Int_t i) { outerHitId = i; }
  void setTof(Float_t t) { tof = t; }
  void setSystem(Int_t s) { system = s;} 
  void setZ(Float_t az,Float_t err) { z=az; cov.setErr(0,err); }
  void setR(Float_t ar,Float_t err) { r=ar; cov.setErr(1,err); }
  void setP(Float_t ap,Float_t err) { p=ap; cov.setErr(2,err); }
  void setTheta(Float_t atheta,Float_t err) {theta=atheta; cov.setErr(3,err); }
  void setPhi(Float_t aphi,Float_t err)     { phi=aphi; cov.setErr(4,err); }
  void setCharge(Int_t c)       { charge=c; }                               
  void setMass(Float_t m,Float_t err) {mass=m; cov.setErr(5,err); }
  void setSector(Int_t s) { sector = s; }
  void setQuality(Char_t q) { quality = q; }
  void setPID(Char_t p) { pid = p;}
  void setPTof(Float_t p) { pTof = p; }
  void setErrPTof(Float_t e) { errPTof = e; }
  void setBeta(Float_t b) { beta = b; }
  void setShowerSum10(Float_t f) { showerSum10 = f; }
  void setShowerSum20(Float_t f) { showerSum20 = f; }
  Char_t getPID(void) { return pid; }
  Float_t getPTof(void) { return pTof; }
  Float_t getErrPTof(void) { return errPTof; }
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
  Float_t getShowerSum10(Float_t f) { return showerSum10; }
  Float_t getShowerSum20(Float_t f) { return showerSum20; }
  Int_t getSector(void)     { return sector; }            
  Char_t getQuality(void) { return quality; }
  Float_t getTof(void) { return tof; }
  Int_t getSystem(void) { return system; }
  Float_t getBeta(void) { return beta; }
  Int_t getSegment(void) { return segmentId; }
  Int_t getRingId(void) { return ringId; }
  Int_t getOuterHitId(void) { return outerHitId; }
  ClassDef(HKickTrack,1) //A final tracki (preliminar)
};

#endif /* !HKickTRACK_H */



