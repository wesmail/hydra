#ifndef HENERGYLOSSCORRPAR_H
#define HENERGYLOSSCORRPAR_H

#include "hparcond.h"

class HEnergyLossCorrPar : public HParCond {
protected:
// for protons
  Int_t   nThetaReg;            // number of theta regions 
  Float_t thetaStep;            // size of one theta region, degrees
  Float_t thetaMiddle0;         // middle of first theta region, degrees
  Float_t momInt1;              // end of first momentum interval, MeV
  Float_t momInt2;              // end of second momentum interval, MeV
  Int_t   nParamsProt;          // number of parameters for one theta region
  Float_t parMomCorrProt[70];   // parameters. Size=nThetaReg*nParamsProt

//for electrons  
  Int_t   nParamsElect;         // number of parameters
  Float_t parMomCorrElect[5];   // parameters
  
public:
  HEnergyLossCorrPar(const char* name    = "EnergyLossCorrPar",
                     const char* title   = "Energy losses correction parameters",
                     const char* context = "EnergyLossCorrProduction");
  ~HEnergyLossCorrPar(void) {}
  
  Bool_t  init(HParIo*,Int_t* set=0);
  Int_t   write(HParIo*);
  void    clear(void);
  void    putParams(HParamList*);
  Bool_t  getParams(HParamList*);

  Bool_t  setParamProt(Int_t nTh,Int_t nParPr,Float_t thM0,Float_t thSt,Float_t* parPr,Float_t mInt1,Float_t mInt2);
  Bool_t  setParamElect(Int_t nParEl,Float_t* parEl);
  Bool_t  setDefaultPar(const char *rn="jan04");
  Float_t getDeltaMom(Int_t pId, Float_t mom, Float_t theta); // units: MeV for mom, degrees for theta
  Float_t getCorrMom(Int_t pId, Float_t mom, Float_t theta) {
    return mom+getDeltaMom(pId,mom,theta);}

  ClassDef(HEnergyLossCorrPar,1) // Parameter container for energy loss correction
};

#endif  /*!HENERGYLOSSCORRPAR_H */









