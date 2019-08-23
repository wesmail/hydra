#ifndef HMUEMULEPTONS_H
#define HMUEMULEPTONS_H

#include <TObject.h>


class HMUEMULeptons : public TObject
{
 public: 
  Int_t iDetBit; 
  Int_t iSector;
  Float_t fThetaRich; 
  Float_t fPhiRich; 
  Float_t fThetaMeta; 
  Float_t fPhiMeta; 
  Int_t iFlag; 
  Float_t fMom; 

 public:
  HMUEMULeptons(void);
  ~HMUEMULeptons(void); 
  void setDetBit( Int_t db ) { iDetBit = db; }
  void setSector( Int_t sec ) { iSector = sec; }
  void setThetaRich( Float_t thetaRich ) { fThetaRich = thetaRich; }
  void setPhiRich( Float_t phiRich ) { fPhiRich = phiRich; }
  void setThetaMeta( Float_t thetaMeta ) { fThetaMeta = thetaMeta; }
  void setPhiMeta( Float_t phiMeta ) { fPhiMeta = phiMeta; }
  void setEFlag( Int_t flag) {iFlag = flag;}
  void setMomentum (Float_t mom) {fMom = mom;}

  Int_t getDetBit(void) { return iDetBit; }
  Int_t getSector(void) { return iSector; } 
  Float_t getThetaRich(void) { return fThetaRich; }
  Float_t getPhiRich(void) { return fPhiRich; }
  Float_t getThetaMeta(void) { return fThetaMeta; }
  Float_t getPhiMeta(void) { return fPhiMeta; }
  Int_t getFlag(void) { return iFlag; }
  Float_t getMom(void) { return fMom; }
 
  
 ClassDef(HMUEMULeptons,1) // MUEMULeptons

};

class HMUEMUDiLeptons : public TObject
{
 public: 
  Int_t iN1; 
  Int_t iN2; 
  Float_t fMass;
  Float_t opangle;
 
 public:
  HMUEMUDiLeptons(void);
  ~HMUEMUDiLeptons(void);
  void setN1( Int_t n ) { iN1=n; }  
  void setN2( Int_t n ) { iN2=n; }
  void setMass( Float_t m ) { fMass = m; }  
  void setOpangle( Float_t op ) { opangle = op; }  

  Int_t getN1(void) { return iN1; }
  Int_t getN2(void) { return iN2; }
  Float_t getMass(void) { return fMass; }
  Float_t getOpangle(void) { return opangle; }

 ClassDef(HMUEMUDiLeptons,1) // MUEMUDiLeptons

};





#endif /* !HMUEMULEPTONS_H */













