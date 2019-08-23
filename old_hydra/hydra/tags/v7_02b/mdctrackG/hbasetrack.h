#ifndef HBASETRACK_H
#define HBASETRACK_H

#include "TObject.h"
#include "hsymmat.h"
class HBaseTrack:public TObject
{
 protected:
  Float_t z;            // From HMdcSeg               (in Lab.?)
  Float_t r;            // From HMdcSeg
  Float_t theta;        // From HMdcSeg
  Float_t phi;          // From HMdcSeg
  Short_t tofHitInd;    // Index of HTofHit or HTofCluster
  Short_t showerHitInd; // Index of HShowerHitTof
  Float_t p;            // Momentum of particle
  Float_t beta;         // speed of paricle
  Float_t mass2;        // mass squared
  Float_t tof;          // time of flight
  Char_t polarity;      // polarity (+1 or -1)
  Char_t sector;        // Sector number
  HSymMat6 cov;         //                            6 ????
  Float_t metaEloss;    // corrected energy loss in TOF or TOFINO
 public:
  HBaseTrack();
  HBaseTrack(HBaseTrack &);
  virtual ~HBaseTrack(){;};
  void setZ(const Float_t Z,Float_t err) {z=Z; cov.setErr(0,err);}
  void setR(const Float_t R,Float_t err) {r=R; cov.setErr(1,err);}
  void setP (const Float_t P,Float_t err) {p=P; cov.setErr(2,err);}
  void setTheta(const Float_t THETA, Float_t err) {
    theta=THETA; cov.setErr(3,err);}
  void setPhi (const Float_t PHI,Float_t err) {
    phi=PHI; cov.setErr(4,err);}
  void setTofHitInd(const Short_t tof) {tofHitInd=tof;}
  void setShowerHitInd(const Short_t shower) {showerHitInd=shower;}
  void setMetaHitInd(Short_t shower, Short_t tof){
     showerHitInd=shower; tofHitInd=tof;}
  void setMass2(const Float_t MASS2,Float_t err) {mass2=MASS2; cov.setErr(5,err);}
  void setTof(const Float_t TOF) {tof=TOF;}
  void setPolarity(const Char_t POLARYTY) {polarity=POLARYTY;}
  void setSector(const Char_t SECTOR) {sector=SECTOR;}
  void setBeta (const Float_t BETA) {beta=BETA;}
  void setMetaEloss(const Float_t e) {metaEloss=e;}
  void setTofFlag(const Short_t FLAG) {tofHitInd=(tofHitInd+1)%10000-1+10000*FLAG;}
  Float_t getZ(void) const {return z;}
  Float_t getR(void) const {return r;}
  Float_t getTheta(void) const {return theta;}
  Float_t getPhi(void) const {return phi;}
  Bool_t isOverlap(void) const {
    return (showerHitInd>-1 && getTofHitInd()>-1) ? kTRUE:kFALSE; }
  Short_t getMetaHitInd(void) const {
    return (getSystem()==1) ? tofHitInd:showerHitInd;} //???
  Int_t getSystem(void) const {
    return (getTofHitInd()>-1) ? 1 : ((showerHitInd>-1) ? 0:-1); } //???
  Short_t getShowerHitInd(void) const {return showerHitInd;}
  Short_t getTofHitInd(void) const {return (((tofHitInd+1)%10000)-1);}
  Float_t getP(void) const {return p;}
  Float_t getMass2(void) const {return mass2;}
  Float_t getTof(void) const {return tof;}
  Char_t getPolarity(void) const {return polarity;}
  Char_t getSector(void) const {return sector;}
  Float_t getBeta(void) const {return beta;}
  Float_t getMetaEloss(void) const {return metaEloss;}
  Short_t getTofFlag(void) const {return ((tofHitInd+1)/10000);}
  HSymMat &getCovariance(void) { return cov; } 
   
  ClassDef(HBaseTrack,1)
};
#endif
