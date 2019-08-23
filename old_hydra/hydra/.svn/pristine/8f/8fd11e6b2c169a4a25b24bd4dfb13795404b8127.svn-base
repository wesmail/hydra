#ifndef HTRIGGERPARSHOWERMAP_H
#define HTRIGGERPARSHOWERMAP_H

#include <TObject.h>
#include <TObjArray.h>
#include "hlocation.h"
#include "hparset.h"


class HTriggerParShowerMap : public HParSet {
private: 
  Int_t row;
  Int_t col;

  Float_t shower_theta[32][32];
  Float_t shower_phi[32][32];

  void printPause(void);
public:
  HTriggerParShowerMap(const char* name="TriggerParShowerMap",
	      const char* title="Parameters for MU Shower Mapping ",
	      const char* context="TriggerProduction");
  ~HTriggerParShowerMap(void);
  
  Float_t getShowerTheta(Int_t r, Int_t c){return shower_theta[r][c];}
  Float_t getShowerPhi(Int_t r, Int_t c){return shower_phi[r][c];}
  void setShowerTheta(Int_t r, Int_t c, Float_t sth){shower_theta[r][c]=sth;}
  void setShowerPhi(Int_t r, Int_t c, Float_t sph){shower_phi[r][c]=sph;}
  
  Bool_t init(HParIo*,Int_t*);
  Int_t write(HParIo*);
  void clear();
  void printParam();
  void readline(const char*, Int_t*);
  void putAsciiHeader(TString&);
  Bool_t writeline(char*, Int_t, Int_t, Int_t); 
  void setDefaults(void);
  ClassDef(HTriggerParShowerMap,1)  // Container for the MU Shower Mapping parameters
};

#endif /* !HTRIGGERPARSHOWERMAP_H */











