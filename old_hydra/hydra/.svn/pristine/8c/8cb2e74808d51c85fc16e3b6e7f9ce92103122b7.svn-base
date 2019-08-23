#ifndef HKICKCANDIDATE_H
#define HKICKCANDIDATE_H

#include "hdataobject.h"

class HKickCandidate : public HDataObject{ //FIXME: Inherit from TObject??
protected:
  Int_t fSector;
  Float_t fP;      //momentum
  Float_t fDPhi;   //Deflection in azimuthal
  Float_t fDTheta; //Deflection in theta
  Float_t fPolarity;    //Polarity 
  Float_t fErrP; //Error in p
  Bool_t fAssigned; //Candidate was accepted?
  Float_t fMass; //Assigned mass in MeV/c^2 or -1.
  Float_t fPhi1; //Phi before magnet
  Float_t fGhi1; //Ghi before magnet
  Float_t fX,fY,fZ; //Position before magnet
  Float_t fTheta1; //Theta before magnet
  Float_t fXPull; //DeltaX / SigmaX
  Int_t fFlag;
public:
  HKickCandidate(void) {fAssigned=kFALSE; fMass=-10;}
  HKickCandidate(HKickCandidate &can);
  ~HKickCandidate(void) {}
  Float_t getMomentum(void)      const { return fP; }
  Float_t getDeltaPhi(void)      const { return fDPhi; }
  Float_t getDeltaTheta(void)    const { return fDTheta; }
  Float_t getPolarity(void)      const { return fPolarity; }
  Float_t getMomentumError(void) const { return fErrP; }
  void setMomentum(Float_t ff)         { fP=ff; }
  void setDeltaPhi(Float_t ff)         { fDPhi=ff; }
  void setDeltaTheta(Float_t ff)       { fDTheta=ff; }
  void setPolarity(Float_t ff)         { fPolarity=ff; }
  void setMomentumError(Float_t ff)    { fErrP=ff; }
  void markAssigned(void) { fAssigned=kTRUE; }
  Bool_t isAssigned(void) { return fAssigned; }
  void setMass(Float_t m) { fMass = m; }
  Float_t getMass(void) { return fMass; }
  Float_t getPhi1(void) { return fPhi1; }
  void setPhi1(Float_t p) { fPhi1=p;}
  Float_t getGhi1(void) { return fGhi1; }
  void setGhi1(Float_t g) { fGhi1=g; }
  void setSector(Int_t s) { fSector=s; }
  Int_t getSector(void) { return fSector; }
  void setPosition1(Float_t x,Float_t y,Float_t z) { 
    fX=x; fY=y; fZ=z;
  }
  void getPosition1(Float_t &x,Float_t &y,Float_t &z) {
    x=fX; y=fY; z=fZ;
  }
  void setTheta1(Float_t t1) { fTheta1 = t1;  }
  Float_t getTheta1(void) { return fTheta1; }
  void setFlag(Int_t f) { fFlag=f; }
  Int_t getFlag(void) { return fFlag; }
  void setXPull(Float_t x) { fXPull=x; }
  Float_t getXPull(void) { return fXPull; }
  ClassDef(HKickCandidate,0) //Evaluated combination of hits in MDC1,2 and TOF/SHOWER
};

#endif
