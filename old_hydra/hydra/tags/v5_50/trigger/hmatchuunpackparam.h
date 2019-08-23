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

public:
  HMatchUUnpackParam();
  ~HMatchUUnpackParam();

  Float_t getTheta(Int_t row, Int_t col){return theta[row][col];}
  Float_t getPhi(Int_t row, Int_t col){return phi[row][col];}

  ClassDef(HMatchUUnpackParam,1) 
};

#endif































