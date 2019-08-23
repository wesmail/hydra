//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 04/03/2005
//*-- Version: $Id: hmatchutof.h,v 1.9 2008-09-18 13:02:58 halo Exp $

#ifndef HMATCHUTOF_H
#define HMATCHUTOF_H

#include "TObject.h"

class HMatchUTof : public TObject {
 protected:
  Float_t fPhi;     // phi coordinate in lab system (deg)
  Int_t fErrPhi;    // error in phi
  Float_t fTheta;   // theta coordinate in lab system (deg)
  Int_t fErrTheta;  // error in theta
  Int_t iPhi;       // phi coordinate in lab coordinatesystem (0 - 255)
  Int_t iTheta;     // theta coordinate in lab coordinate system (0 - 255)

  Int_t sec;        // sector in standad notation (0 - 5)
  Float_t time;     // time of flight (ns)
  Int_t PID;        // identifier for the particle (=1 for leptons, =2 for other particles)


 public:
  HMatchUTof(void) {}
  ~HMatchUTof(void) {}
  void setTheta(Float_t val,Int_t error) { fTheta=val; fErrTheta=error;};
  void setPhi(Float_t val,Int_t error) { fPhi = val; fErrPhi=error;};
  void setiTheta(Int_t ith) { iTheta=ith;};
  void setiPhi(Int_t iph) { iPhi = iph;};

  void setTime(Float_t t){time=t;};
  void setSec(Int_t s){sec=s;};
  void setPID(Int_t p){PID=p;};
  Int_t getPID(void){return PID;};
  Int_t getSector(void) { return sec; };
  Float_t getTheta(){return fTheta;};
  Float_t getTime(){return time;};
  Int_t getErrTheta(){return fErrTheta;};
  Float_t getPhi(){return fPhi;};
  Int_t getErrPhi(){return fErrPhi;};
  Int_t getiTheta(){return iTheta;};
  Int_t getiPhi(){return iPhi;};

  ClassDef(HMatchUTof,1)    // Datacontainer storing all Matching unit information from TOF
};

#endif
