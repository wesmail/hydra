#ifndef HKICKMASSFIT_H
#define HKICKMASSFIT_H

#include <TObject.h>

class HKickMassFit : public TObject {
protected:
  Double_t fMomentum;
  Double_t fMomentumErr;
  Double_t fTolerance;
  Double_t fMass;
  Int_t fPolarity;
  Char_t fPid;
  static Double_t fMassTable[];
  static Char_t fPidTable[];
  Double_t snap(Double_t mass, Double_t errMass);
public:
  HKickMassFit(void);
  ~HKickMassFit(void);
  void fitMomentum(Double_t p0, Double_t errP0, 
		   Double_t beta, Double_t errBeta,
		   Int_t polarity);
  void setTolerance(Double_t t) { fTolerance = t; }
  Double_t getMass(void) { return fMass; }
  Double_t getMomentum(void) { return fMomentum; }
  Double_t getMomentumErr(void) { return fMomentumErr; }
  Double_t getTolerance(void) { return fTolerance; }
  Char_t getPID(void) { return fPid; }
};


#endif
