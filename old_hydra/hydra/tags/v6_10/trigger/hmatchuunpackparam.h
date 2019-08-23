#ifndef HMATCHUUNPACKPARAM_H
#define HMATCHUUNPACKPARAM_H

#pragma interface

#include "hparset.h"
#include "hlocation.h"
#include <TList.h>
 
  
class HMatchUUnpackParam: public TObject {
private:

 Float_t theta[96][96];
 Float_t phi[96][96];

 Float_t theta_sh[32][32];
 Float_t phi_sh[32][32];      

public:
  HMatchUUnpackParam();
  ~HMatchUUnpackParam();

  Float_t getTheta(Int_t row, Int_t col){return theta[row][col];}
  Float_t getPhi(Int_t row, Int_t col){return phi[row][col];}

  Float_t getThetaSh(Int_t row, Int_t col){return theta_sh[row][col];}
  Float_t getPhiSh(Int_t row, Int_t col){return phi_sh[row][col];}

  ClassDef(HMatchUUnpackParam,1) 
};

#endif































