#ifndef HMULEPTONS_H
#define HMULEPTONS_H

#include <TObject.h>


class HMULeptons : public TObject
{
 private: 
  Float_t fTheta; 
  Float_t fPhi; 
  Float_t fFlag; 
  Float_t fMom; 

 public:
  HMULeptons(void);
  ~HMULeptons(void){;}
  void setTheta( Float_t theta ) { fTheta = theta; }
  void setPhi( Float_t phi ) { fPhi = phi; }
  void setEFlag( Float_t flag) {fFlag = flag;}
  void setMomentum (Float_t mom) {fMom = mom;}
 
  
 ClassDef(HMULeptons,1) // MULeptons

};

class HMUDiLeptons : public TObject
{
 private: 
  Int_t n1; 
  Int_t n2; 
  Float_t mass; 
 public:
  HMUDiLeptons(void);
  ~HMUDiLeptons(void){;}
  void setN1( Int_t n ) { n1=n; }  
  void setN2( Int_t n ) { n2=n; }
  void setMass( Float_t x ) { mass=x;}  

 ClassDef(HMUDiLeptons,1) // MUDiLeptons

};





#endif /* !HMULEPTONS_H */













