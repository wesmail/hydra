#ifndef HMATCHUTOF_H
#define HMATCHUTOF_H

#include <TObject.h>

class HMatchUTof : public TObject {
 protected:
  Float_t fPhi;
  Int_t fErrPhi; 
  Float_t fTheta; 
  Int_t fErrTheta; 
  Int_t iPhi;
  Int_t iTheta; 

  Int_t sec;
  Float_t time;
  Int_t PID;


 public:
  HMatchUTof(void) {}
  ~HMatchUTof(void) {}
  void setTheta(Float_t val,Int_t error) { fTheta=val; 
                                          fErrTheta=error;}
  void setPhi(Float_t val,Int_t error) { fPhi = val; 
                                       fErrPhi=error;}
  void setiTheta(Int_t ith) { iTheta=ith;}
  void setiPhi(Int_t iph) { iPhi = iph;}

  void setTime(Float_t t){time=t;}
  void setSec(Int_t s){sec=s;}
  void setPID(Int_t p){PID=p;}
  Int_t getPID(void){return PID;}
  Int_t getSector(void) { return sec; }
  Float_t getTheta(){return fTheta;}
  Float_t getTime(){return time;}
  Int_t getErrTheta(){return fErrTheta;}
  Float_t getPhi(){return fPhi;}
  Int_t getErrPhi(){return fErrPhi;}
  Int_t getiTheta(){return iTheta;}
  Int_t getiPhi(){return iPhi;}

  ClassDef(HMatchUTof,1)
};

#endif
