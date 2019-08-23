#ifndef HTRIGGERPARMOMMAP_H
#define HTRIGGERPARMOMMAP_H

#include <TObject.h>
#include <TObjArray.h>
#include "hlocation.h"
#include "hparset.h"


class HTriggerParMomMap : public HParSet {
private: 
  Int_t Theta_bins;
  Int_t Deltatheta_bins;
  Int_t Phi_bins;

  Float_t mu_mom[64][64][8];  
public:
  HTriggerParMomMap(const char* name="TriggerParMomMap",
	      const char* title="Parameters for MU Momentum Mapping ",
	      const char* context="TriggerProduction");
  ~HTriggerParMomMap(void);
  
  Int_t getTheta_bins(void){return Theta_bins;}
  Int_t getDeltatheta_bins(void){return Deltatheta_bins;}
  Int_t getPhi_bins(void){return Phi_bins;}

  void setTheta_bins(Int_t th_bins) {Theta_bins=th_bins;}
  void setDeltatheta_bins(Int_t Dth_bins) {Deltatheta_bins=Dth_bins;}
  void setPhi_bins(Int_t ph_bins) {Phi_bins=ph_bins;}

  Float_t getMom(Int_t th, Int_t Delta_th, Int_t ph){return mu_mom[th][Delta_th][ph];}
  void setMom(Int_t th, Int_t Delta_th, Int_t ph, Float_t mom){mu_mom[th][Delta_th][ph]=mom;}
  
  Bool_t init(HParIo*,Int_t*);
  Int_t write(HParIo*);
  void clear();
  void printParam();
  void readline(const char*, Int_t*);
  void putAsciiHeader(TString&);
  Bool_t writeline(char*, Int_t, Int_t, Int_t); 
  Bool_t writesize(char *buf);
  void setDefaults(void);
  void printPause(void);

  ClassDef(HTriggerParMomMap,1)  // Container for the MU Momentum Mapping parameters
};

#endif /* !HTRIGGERPARMOMMAP_H */











