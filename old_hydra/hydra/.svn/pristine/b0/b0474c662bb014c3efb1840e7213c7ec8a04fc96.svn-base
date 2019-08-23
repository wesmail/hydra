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

  public:
   HHypMomError ();
  ~HHypMomError (void);

   Double_t getMomError(Double_t theta, Double_t phi, Double_t mass, Double_t p,Int_t sec,Bool_t simu);

   Bool_t   init (void);

   ClassDef(HHypMomError,0)
 };

#endif
