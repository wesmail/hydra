#ifndef HKICKCANDIDATE_H
#define HKICKCANDIDATE_H

#include "hdataobject.h"
#include "hgeomvector.h"

class HKickCandidate : public HDataObject{ //FIXME: Inherit from TObject??
protected:
  Int_t fSector;
  Float_t fP;      //momentum
  Float_t fPolarity;    //Polarity 
  Float_t fErrP; //Error in p
  Float_t fPTof;
  Float_t fPTofErr;
  Bool_t fAssigned; //Candidate was accepted?
  Float_t fMass; //Assigned mass in MeV/c^2 or -1.
  Float_t fX,fY,fZ; //Position before magnet
  Float_t fXPull; //DeltaX / SigmaX
  Char_t fPID; 
  Double_t fDenomin; //Deniminator.
  Float_t fDist,fKickDist;
  Int_t fFlag;
  Float_t fTof;
  Int_t fSystem;
  Float_t beta;
  TObject *fOuterHit;
 public:
  HKickCandidate(void);
  HKickCandidate(HKickCandidate &can);
  ~HKickCandidate(void) {}
  void setOuterHit(TObject *obj) { fOuterHit = obj; }
  TObject *getOuterHit(void) { return fOuterHit; }
  Float_t getBeta(void) { return beta; }
  void setBeta(Float_t b) { beta = b; }
  Float_t getTof(void) { return fTof; }
  void setTof(Float_t tof) {fTof = tof; }
  Int_t getSystem(void) { return fSystem; }
  void setSystem(Int_t s) { fSystem = s;}
  Float_t getMomentum(void)      const { return fP; }
  Float_t getPolarity(void)      const { return fPolarity; }
  Float_t getMomentumError(void) const { return fErrP; }
  Float_t getPTof(void)                { return fPTof; }
  Float_t getPTofErr(void)             { return fPTofErr; }
  Float_t getMass(void)                { return fMass; }
  Float_t getPos1X(void)               { return fX; }
  Float_t getPos1Y(void)               { return fY; }
  Float_t getPos1Z(void)               { return fZ; }
  Float_t getDist(void)                { return fDist; }
  Float_t getKickDist(void)            { return fKickDist; }
  Float_t getXPull(void)               { return fXPull; }
  Double_t getDenominator(void)        {return fDenomin; }
  Char_t getPID(void)                  { return fPID; }
  Bool_t isAssigned(void)              { return fAssigned; }
  Int_t getSector(void)                { return fSector; }
  Int_t getFlag(void)                  { return fFlag; }
  void setFlag(Int_t f)                { fFlag = f; }
  void setPID(Char_t pd)               { fPID= pd; }
  void setPTof(Float_t p)              { fPTof = p; }
  void setPTofErr(Float_t err)         { fPTofErr = err; }
  void setMomentum(Float_t ff)         { fP=ff; }
  void setPolarity(Float_t ff)         { fPolarity=ff; }
  void setMomentumError(Float_t ff)    { fErrP=ff; }
  void markAssigned(void)              { fAssigned=kTRUE; }
  void setMass(Float_t m)              { fMass = m; }
  void setSector(Int_t s)              { fSector=s; }
  void setPosition1(Float_t x,Float_t y,Float_t z) { 
    fX=x; fY=y; fZ=z;
  }
  void getPosition1(Float_t &x,Float_t &y,Float_t &z) {
    x=fX; y=fY; z=fZ;
  }
  void getPosition1(HGeomVector &v) {
    v.setX(fX); v.setY(fY); v.setZ(fZ);
  }
  void setXPull(Float_t x) { fXPull=x; }
  void setDenominator(Double_t d) { fDenomin = d; }
  void setDist(Float_t f) { fDist = f; }
  void setKickDist(Float_t f) { fKickDist = f; }
  ClassDef(HKickCandidate,0) //Evaluated combination of hits in MDC1,2 and TOF/SHOWER
};

#endif
