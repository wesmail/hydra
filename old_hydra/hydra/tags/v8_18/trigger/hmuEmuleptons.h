//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/10/2005
//*-- Version: $Id: hmuEmuleptons.h,v 1.4 2005-07-15 09:00:24 wuestenf Exp $

#ifndef HMUEMULEPTONS_H
#define HMUEMULEPTONS_H

#include <TObject.h>


class HMUEMULeptons : public TObject
{
 protected: 
  Int_t iDetBit;        // Flag for META detector  (=0 for TOF, =1 for SHOWER)
  Int_t iSector;        // Sectror in standard  notation (0 - 5)
  Float_t fThetaRich;   // theta coordinate of RICH hit in lab coordinate system
  Float_t fPhiRich;     // phi coordinate of RICH hit in lab coordinate system
  Float_t fThetaMeta;   // theta coordinate of META hit in lab coordinate system
  Float_t fPhiMeta;     // phi coordinate of META hit in lab coordinate system
  Int_t iFlag;          // lepton flag (=0 for positrons, =1 for electrons)
  Float_t fMom;         // Momentum (0 - 255)

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
 
  
 ClassDef(HMUEMULeptons,1) // Leptons from the MatchingUnit emulation

};

class HMUEMUDiLeptons : public TObject
{
 protected: 
  Int_t iN1;        // index of first lepton used for dilepton
  Int_t iN2;        // index of second lepton used for dilepton
  Float_t fMass;    // invariant mass of dilepton
  Float_t opangle;  // opening angle of the two leptons
 
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

 ClassDef(HMUEMUDiLeptons,1) // Dileptons from the MatchingUnit emulation

};

#endif /* !HMUEMULEPTONS_H */
