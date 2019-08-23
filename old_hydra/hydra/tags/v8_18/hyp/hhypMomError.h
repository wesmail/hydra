//*-- AUTHOR : B. Spruck
//*-- Modified : 12 Jun 2006

#ifndef HHYP_MOMERROR_H
#define HHYP_MOMERROR_H

#include <TObject.h>

class HHypMomError : public TObject
 {
  private:
   Double_t getMomErrGeantJan04(Double_t theta, Double_t phi, Double_t mass, Double_t p,Int_t sec);
   Double_t getMomErrJan04(Double_t theta, Double_t phi, Double_t mass, Double_t p,Int_t sec);

   Double_t getMomErrGeantApr07(Double_t theta, Double_t phi, Double_t mass, Double_t p,Int_t sec);
   Double_t getMomErrApr07(Double_t theta, Double_t phi, Double_t mass, Double_t p,Int_t sec);

   Bool_t jan04, apr06, apr07;
  public:
   HHypMomError ();
  ~HHypMomError (void);

	void setJan04(){jan04=true; apr06=false; apr07=false;};
	void setApr06(){jan04=false; apr06=true; apr07=false;};
	void setApr07(){jan04=false; apr06=false; apr07=true;};

  Double_t getMomError(Double_t theta, Double_t phi, Double_t mass, Double_t p,Int_t sec,Bool_t simu);
	void getFaktors(Double_t &FakErrMomExp,Double_t &FakErrThetaExp,Double_t &FakErrPhiExp,
									Double_t &FakErrMomSim,Double_t &FakErrThetaSim,Double_t &FakErrPhiSim);

   Bool_t   init (void);

   ClassDef(HHypMomError,0)
 };

extern HHypMomError HypMomErr;

#endif
