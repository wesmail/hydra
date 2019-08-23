//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/11/2005
//*-- Version: $id$

#ifndef HTRIGGERPARMUDILEP_H
#define HTRIGGERPARMUDILEP_H

#include "hparcond.h"
#include "TString.h"

class HParamList;

class HTriggerParMuDilep : public HParCond
{
private:
  Int_t MUcutoff;  // Matching unit cut off parameter
public:
  HTriggerParMuDilep(const Char_t* name="TriggerParMuDilep",
                     const Char_t* title="Parameters for MU Dilepton Emulation",
                     const Char_t* context="TriggerProduction");
  ~HTriggerParMuDilep(void);

  Int_t getMUcutoff() { return MUcutoff; }
  void setMUcutoff(Int_t ct) { MUcutoff = ct; }

  void setDefaults(void);

  Bool_t init(HParIo*,Int_t*);
  Int_t write(HParIo*);
  void putParams(HParamList*);
  Bool_t getParams(HParamList*);
  void clear();
  void printParam();
  void readline(const Char_t*, Int_t*);
  void putAsciiHeader(TString&);
/*   Bool_t writeline(Char_t*, Int_t, Int_t, Int_t); */
  ClassDef(HTriggerParMuDilep,1)  // Container for the MU Dilepton Emulation parameters
};

#endif /* !HTRIGGERPARMUDILEP_H */


