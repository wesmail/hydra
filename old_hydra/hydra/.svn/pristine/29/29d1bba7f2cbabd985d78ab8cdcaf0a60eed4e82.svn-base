#ifndef HTRIGGERPARMUDILEP_H
#define HTRIGGERPARMUDILEP_H

#include <TObject.h>
#include <TObjArray.h>
#include "hlocation.h"
#include "hparset.h"

class HTriggerParMuDilep : public HParSet {
private: 
  Int_t MUcutoff;
  void printPause(void);
public:
  HTriggerParMuDilep(const char* name="TriggerParMuDilep",
	      const char* title="Parameters for MU Dilepton Emulation",
	      const char* context="TriggerProduction");
  ~HTriggerParMuDilep(void);

  Int_t getMUcutoff() { return MUcutoff; }
  void setMUcutoff(Int_t ct) { MUcutoff = ct; }

  void setDefaults(void);

  Bool_t init(HParIo*,Int_t*);
  Int_t write(HParIo*);
  void clear();
  void printParam();
  void readline(const char*, Int_t*);
  void putAsciiHeader(TString&);
  Bool_t writeline(char*, Int_t, Int_t, Int_t);
  ClassDef(HTriggerParMuDilep,1)  // Container for the MU Dilepton Emulation parameters
};

#endif /* !HTRIGGERPARMUDILEP_H */


