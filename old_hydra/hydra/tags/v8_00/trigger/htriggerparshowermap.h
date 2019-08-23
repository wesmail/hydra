//*-- Author: Ilse Koenig
//*-- Modified: Joern Wuestenfeld 07/14/2005
//*-- Version: $Id: htriggerparshowermap.h,v 1.4 2005-07-15 09:00:25 wuestenf Exp $

#ifndef HTRIGGERPARSHOWERMAP_H
#define HTRIGGERPARSHOWERMAP_H

#include <TObject.h>
#include <TObjArray.h>
#include "hlocation.h"
#include "hparset.h"


class HTriggerParShowerMap : public HParSet {
private: 
  Int_t row;                    // row number
  Int_t col;                    // column number

  Float_t shower_theta[32][32]; // theta map
  Float_t shower_phi[32][32];   // phi map

  void printPause(void);
public:
  HTriggerParShowerMap(const char* name="TriggerParShowerMap",
                      const char* title="Parameters for MU Shower Mapping ",
                      const char* context="TriggerProduction");
  ~HTriggerParShowerMap(void);

  Int_t getRowSize() {return 32;}
  Int_t getColSize() {return 32;}       
  Float_t getShowerTheta(Int_t c, Int_t r){return shower_theta[c][r];}
  Float_t getShowerPhi(Int_t c, Int_t r){return shower_phi[c][r];}
  void setShowerTheta(Int_t c, Int_t r, Float_t sth){shower_theta[c][r]=sth;}
  void setShowerPhi(Int_t c, Int_t r, Float_t sph){shower_phi[c][r]=sph;}
  
  Bool_t init(HParIo*,Int_t*);
  Int_t write(HParIo*);
  void clear();
  void printParam();
  void readline(const char*, Int_t*);
  void putAsciiHeader(TString&);
  Bool_t writeline(char*, Int_t, Int_t); 
  void setDefaults(void);
  ClassDef(HTriggerParShowerMap,1)  // Container for the MU Shower Mapping parameters
};

#endif /* !HTRIGGERPARSHOWERMAP_H */











