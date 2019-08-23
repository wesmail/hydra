//*-- Author : Alberica Toia
//*-- Modified: Joern Wuestenfeld 03/31/2005
//*-- Version: $Id: hmuleptons.h,v 1.6 2008-09-18 13:02:58 halo Exp $

#ifndef HMULEPTONS_H
#define HMULEPTONS_H

#include "TObject.h"


class HMULeptons : public TObject
{
 private: 
  Int_t iRichNr;        // index of RICH hit used for lepton
  Int_t iMetaNr;        // index of META hit used for lepton
  Int_t iDetBit;        // flag for META detector (=0 for TOF, =1 for SHOWER)
  Int_t iSector;        // sector (0..5)
  Float_t fThetaRich;   // theta coordinate of RICH hit in lab coordinate system
  Float_t fPhiRich;     // phi coordinate of RICH hit in lab coordinate system
  Float_t fThetaMeta;   // theta coordinate of META hit in lab coordinate system
  Float_t fPhiMeta;     // phi coordinate of META hit in lab coordinate system
  Int_t iFlag;          // lepton flag (=0 for positrons, =1 for electrons)
  Float_t fMom;         // momentum (0..255)

 public:
  HMULeptons(void);
  ~HMULeptons(void){;}
  void setRichNr( Int_t rnr ) { iRichNr = rnr; }
  void setMetaNr( Int_t rmr ) { iMetaNr = rmr; }
  void setDetBit( Int_t db ) { iDetBit = db; }
  void setSector( Int_t sec ) { iSector = sec; }
  void setThetaRich( Float_t theta ) { fThetaRich = theta; }
  void setPhiRich( Float_t phi ) { fPhiRich = phi; }
  void setThetaMeta( Float_t theta ) { fThetaMeta = theta; }
  void setPhiMeta( Float_t phi ) { fPhiMeta = phi; }
  void setEFlag( Int_t flag) {iFlag = flag;}
  void setMomentum (Float_t mom) {fMom = mom;}

  Int_t getRichNr(void) { return iRichNr; }
  Int_t getMetaNr(void) { return iMetaNr; }
  Int_t getDetBit(void) { return iDetBit; }
  Int_t getSector(void) { return iSector; } 
  Float_t getThetaRich(void) { return fThetaRich; }
  Float_t getPhiRich(void) { return fPhiRich; }
  Float_t getThetaMeta(void) { return fThetaMeta; }
  Float_t getPhiMeta(void) { return fPhiMeta; }
  Int_t getFlag(void) { return iFlag; }
  Float_t getMom(void) { return fMom; }
 
  
 ClassDef(HMULeptons,1) // Matching Unit lepton datacontainer

};

class HMUDiLeptons : public TObject
{
 private: 
  Int_t iN1;      // index of first lepton used for dilepton
  Int_t iN2;      // index of second lepton used for dilepton
  Float_t fMass;  // invariant mass
 public:
  HMUDiLeptons(void);
  ~HMUDiLeptons(void){;}
  void setN1( Int_t n ) { iN1=n; }  
  void setN2( Int_t n ) { iN2=n; }
  void setMass( Float_t x ) { fMass = x; }  

  Int_t getN1(void) { return iN1; }
  Int_t getN2(void) { return iN2; }
  Float_t getMass(void) { return fMass; }

 ClassDef(HMUDiLeptons,1) // Matching Unit dilepton datacontainer

};

#endif /* !HMULEPTONS_H */
