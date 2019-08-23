#ifndef HBASETRACK_H
#define HBASETRACK_H

#include "TObject.h"
#include "hsymmat.h"
class HBaseTrack:public TObject
{
 private:
  Float_t z;//From HMdcSeg
  Float_t r; //From HMdcSeg
  Float_t theta;//From HMdcSeg
  Float_t phi;//From HMdcSeg
  Short_t OuterInd;//Index from META
  Float_t p;//Momentum of particle
  Float_t beta;//speed of paricle
  Float_t mass2;//mass squared
  Float_t tof;//time of flight
  Char_t polarity;//polaryty (+1 or -1)
  Char_t sector;//Sector number
  Int_t system;//Tof or Shower
  HSymMat6 cov;//
 public:
  HBaseTrack();
  HBaseTrack(HBaseTrack &);
  ~HBaseTrack(){;};
  void setZ(Float_t Z,Float_t err) {z=Z; cov.setErr(0,err);}
  void setR(Float_t R,Float_t err) {r=R; cov.setErr(1,err);}
  void setP (Float_t P,Float_t err) {p=P; cov.setErr(2,err);}
  void setTheta(Float_t THETA, Float_t err) {theta=THETA; cov.setErr(3,err);}
  void setPhi (Float_t PHI,Float_t err) {phi=PHI; cov.setErr(4,err);}
  void setOuterInd(Short_t OUTERIND) {OuterInd=OUTERIND;}
  void setMass2(Float_t MASS2,Float_t err) {mass2=MASS2; cov.setErr(5,err);}
  void setTof(Float_t TOF) {tof=TOF;}
  void setPolarity (Char_t POLARYTY) {polarity=POLARYTY;}
  void setSector(Char_t SECTOR) {sector=SECTOR;}
  void setSystem(Int_t SYSTEM) {system=SYSTEM;}
  void setBeta (Float_t BETA) {beta=BETA;}
    
  Float_t getZ() {return z;}
  Float_t getR() {return r;}
  Float_t getTheta() {return theta;}
  Float_t getPhi() {return phi;}
  Short_t getOuterInd () {return OuterInd;}
  Float_t getP() {return p;}
  Float_t getMass2() {return mass2;}
  Float_t getTof() {return tof;}
  Char_t getPolarity() {return polarity;}
  Char_t getSector() {return sector;}
  Float_t getBeta() {return beta;}
  Int_t getSystem() {return system;}
  ClassDef(HBaseTrack,1)
     };
#endif
