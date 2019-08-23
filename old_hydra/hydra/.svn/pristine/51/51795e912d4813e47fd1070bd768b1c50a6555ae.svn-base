#ifndef HMATCHUTOF_H
#define HMATCHUTOF_H

#include "hdataobject.h"

class HMatchUTof : public HDataObject {
 protected:
  Float_t fPhi;
  Int_t fErrPhi; 
  Float_t fTheta; 
  Int_t fErrTheta; 

 public:
  HMatchUTof(void) {}
  ~HMatchUTof(void) {}
  void setTheta(Float_t val,Int_t error) { fTheta=val; 
                                          fErrTheta=error;}
  void setPhi(Float_t val,Int_t error) { fPhi = val; 
                                       fErrPhi=error;}

  Float_t getTheta(){return fTheta;}
  Int_t getErrTheta(){return fErrTheta;}
  Float_t getPhi(){return fPhi;}
  Int_t getErrPhi(){return fErrPhi;}
 
  ClassDef(HMatchUTof,0)
};

#endif
