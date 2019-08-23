#ifndef HKICKTRACK_H
#define HKICKTRACK_H

#include <TObject.h>
#include "hsymmat.h"

class HKickTrack : public TObject
{
private:
  Float_t pTof;  //Track Momentum using determined by Time Of Flight using inverse kinematics
  Float_t errPTof; // Error in pTof
  Char_t pid;      // Tentative particle Id of the track
  Char_t quality;  // Track Quality. q=0 all reasonable combs, q=1 good ones.
  Float_t z;       // z component of the 'vertex' coordinates
  Float_t r;       // rho component of the 'vertex' coordinates
  Float_t p;       // Track momentum determined from the kickplane
  Float_t theta;   // Polar angle of the track's direction vector
  Float_t phi;     // Azimuthal angle of the track's direction vector
  Float_t mass;    // Track's mass squared. 
  Int_t charge;    // Sign of charge
  HSymMat6 cov;    // Covariance matrix of track params: z,r,p,theta,phi,mass
  Char_t sector;   // Sector where the track was found
  Int_t system;    // Detector which was matched with MDC to form a track
  Float_t tof;     // Track's time of flight
  Float_t metaeloss; // energy loss in Meta
  Float_t beta;    // Track's speed
  Float_t showerSum10; // Shower: sum of 9 charges in post / sum of 9 charges in pre
  Float_t showerSum20; // Shower: sum of 9 charges in post2 / sum of 9 charges in pre
  Short_t segmentId; // Id of the MDC segment used to build the track
  Short_t ringId;    // Id of a RICH ring matching the track 
  Short_t outerHitId; // Id of the hit in META used to build the track
  Float_t pull;       // The variable used for matching
  Float_t flag;       //Undefined
public:
  static const Char_t antiProtonId;
  static const Char_t protonId;
  static const Char_t kPlusId;
  static const Char_t kMinusId;
  static const Char_t piPlusId;
  static const Char_t piMinusId;
  static const Char_t unknownId;
  HKickTrack(void);
  HKickTrack(HKickTrack &aTrack, Int_t vers=0);
  ~HKickTrack(void);
  void setPull(Float_t p) { pull = p; }
  void setSegment(Int_t i) { segmentId = i; }
  void setRingId(Int_t i) { ringId = i; }
  void setOuterHitId(Int_t i) { outerHitId = i; }
  void setTof(Float_t t) { tof = t; }
  void setMetaEloss(Float_t t) { metaeloss = t; }
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
  void setFlag(Float_t f) { flag = f; }
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
  Float_t getMetaEloss(void) {return metaeloss; }
  Int_t getSystem(void) { return system; }
  Float_t getBeta(void) { return beta; }
  Int_t getSegment(void) { return segmentId; }
  Int_t getRingId(void) { return ringId; }
  Int_t getOuterHitId(void) { return outerHitId; }
  Float_t getPull(void) { return pull; }
  Float_t getFlag(void) { return flag; }
  HSymMat6& getCov(void) { return cov; }
  ClassDef(HKickTrack,3) //A final tracki (preliminar)
};

#endif /* !HKickTRACK_H */



