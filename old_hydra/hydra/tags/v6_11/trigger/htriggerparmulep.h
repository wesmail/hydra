#ifndef HTRIGGERPARMULEP_H
#define HTRIGGERPARMULEP_H

#include <TObject.h>
#include <TObjArray.h>
#include "hlocation.h"
#include "hparset.h"

class HTriggerParMuLep : public HParSet {
private: 
  Float_t MUtheta;     
  Float_t MUphi;     
  void printPause(void);
public:
  HTriggerParMuLep(const char* name="TriggerParMuLep",
	      const char* title="Parameters for MU Lepton Emulation",
	      const char* context="TriggerProduction");
  ~HTriggerParMuLep(void);

  Float_t getMUtheta() { return MUtheta; }
  Float_t getMUphi() { return MUphi; }

  void setMUtheta(Float_t th) { MUtheta = th; }
  void setMUphi(Float_t ph) { MUphi = ph; }  

  void setDefaults(void);

  Bool_t init(HParIo*,Int_t*);
  Int_t write(HParIo*);
  void clear();
  void printParam();
  void readline(const char*, Int_t*);
  void putAsciiHeader(TString&);
  Bool_t writeline(char*, Int_t, Int_t, Int_t);
  ClassDef(HTriggerParMuLep,1)  // Container for the MU Lepton Emulation parameters
};

#endif /* !HTRIGGERPARMULEP_H */





