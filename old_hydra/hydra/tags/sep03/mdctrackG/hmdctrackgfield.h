#ifndef HMDCTRACKGFIELD_H
#define HMDCTRACKGFIELD_H
#include "TString.h"
#include "TObject.h"
#include "TNamed.h"
//Read the field
//Input: 3 dimensional Position vector 
//Output:3 dimensional Field vector
//created 16/01/2002/ 
//********************************************
//  nfz      number of steps in z-direction 
//  nfr      number of steps in rho-direction 
//  nfp      number of steps in phi-direction 
//  zflmin   minimum value of z-coordinate
//  zflmax   maximum value of z-coordinate
//  zfldel   step value of z-coordinate
//  rflmin   minimum value of rho-coordinate
//  rflmax   maximum value of rho-coordinate
//  rfldel   step value of rho-coordinate
//  pflmin   minimum value of phi-coordinate
//  pflmax   maximum value of phi-coordinate
//  pfldel   step value of phi-coordinate


class HMdcTrackGField:public TNamed
{
friend class HMdcTrackGSpline;
private:
  Int_t nfz,nfr,nfp;
  Double_t zflmin,zflmax,zfldel;
  Double_t rflmin,rflmax,rfldel;
  Double_t pflmin,pflmax,pfldel;
  Double_t tzfl[161][176][25];
  Double_t trfl[161][176][25];
  Double_t tpfl[161][176][25];
  Double_t Pvector[3]; //3 dimensional position vector
  Double_t Fvector[3]; //3 dimensional field vector
public:
  HMdcTrackGField(const char* name="",const char* title="");
  ~HMdcTrackGField();
  void init(TString);
  void calcField(Double_t *,Double_t *,Double_t);
  void clear();
ClassDef(HMdcTrackGField,1)

};


#endif
