#ifndef HMULEPTONS_H
#define HMULEPTONS_H

#include <TObject.h>


class HMULeptons : public TObject
{
 private: 
  Int_t iRichNr;
  Int_t iMetaNr;
  Int_t iDetBit; 
  Int_t iSector;
  Float_t fTheta; 
  Float_t fPhi; 
  Int_t iFlag; 
  Float_t fMom; 

 public:
  HMULeptons(void);
  ~HMULeptons(void){;}
  void setRichNr( Int_t rnr ) { iRichNr = rnr; }
  void setMetaNr( Int_t rmr ) { iMetaNr = rmr; }
  void setDetBit( Int_t db ) { iDetBit = db; }
  void setSector( Int_t sec ) { iSector = sec; }
  void setTheta( Float_t theta ) { fTheta = theta; }
  void setPhi( Float_t phi ) { fPhi = phi; }
  void setEFlag( Int_t flag) {iFlag = flag;}
  void setMomentum (Float_t mom) {fMom = mom;}

  Int_t getRichNr(void) { return iRichNr; }
  Int_t getMetaNr(void) { return iMetaNr; }
  Int_t getDetBit(void) { return iDetBit; }
  Int_t getSector(void) { return iSector; } 
  Float_t getTheta(void) { return fTheta; }
  Float_t getPhi(void) { return fPhi; }
  Int_t getFlag(void) { return iFlag; }
  Float_t getMom(void) { return fMom; }
 
  
 ClassDef(HMULeptons,1) // MULeptons

};

class HMUDiLeptons : public TObject
{
 private: 
  Int_t iN1; 
  Int_t iN2; 
  Float_t fMass; 
 public:
  HMUDiLeptons(void);
  ~HMUDiLeptons(void){;}
  void setN1( Int_t n ) { iN1=n; }  
  void setN2( Int_t n ) { iN2=n; }
  void setMass( Float_t x ) { fMass = x; }  

  Int_t getN1(void) { return iN1; }
  Int_t getN2(void) { return iN2; }
  Float_t getMass(void) { return fMass; }

 ClassDef(HMUDiLeptons,1) // MUDiLeptons

};





#endif /* !HMULEPTONS_H */













