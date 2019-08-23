#ifndef HMUMOMMAP_H
#define HMUMOMMAP_H

#pragma interface

#include "hparset.h"
#include "hlocation.h"
#include <TList.h>
 
  
class HMUMomMap: public TObject {
private:

 Float_t mu_mom[64][64][8];

public:
  HMUMomMap();
  ~HMUMomMap();

  Float_t getMom(Int_t th, Int_t Delta_th, Int_t ph){return mu_mom[th][Delta_th][ph];}

 
 ClassDef(HMUMomMap,1) 
};

#endif































