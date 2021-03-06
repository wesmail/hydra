#ifndef HTRIGGERPARMULEP_H
#define HTRIGGERPARMULEP_H

#include <TObject.h>
#include <TObjArray.h>
#include "hlocation.h"
#include "hparcond.h"

class HParamList; 
class HTriggerParMuLep : public HParCond {
private: 
  Int_t MUtheta;     
  Int_t MUphi_offset;
  Int_t MUphi_slope; 
  Int_t MUfield; 
  Float_t MUtof; 
     
  void printPause(void);
public:
  HTriggerParMuLep(const char* name="TriggerParMuLep",
	      const char* title="Parameters for MU Lepton Emulation",
	      const char* context="TriggerProduction");
  ~HTriggerParMuLep(void);

  Int_t getMUtheta() { return MUtheta; }
  Int_t getMUphiOffset() { return MUphi_offset; }
  Int_t getMUphiSlope() { return MUphi_slope; }
  Int_t getMUfield() { return MUfield; }
  Float_t getMUtof() { return MUtof; }

  void setMUtheta(Int_t th) { MUtheta = th; }
  void setMUphiOffset(Int_t phO)  { MUphi_offset = phO; }  
  void setMUphiSlope(Int_t phS) { MUphi_slope = phS; }  
  void setMUfield(Int_t f) { MUfield = f; }  
  void setMUtof(Int_t tof) { MUtof = tof; }  

  void setDefaults(void);

  Bool_t init(HParIo*,Int_t*);
  Int_t write(HParIo*);
  void putParams(HParamList*);
  Bool_t getParams(HParamList*); 
  void clear();
  void printParam();
  void readline(const char*, Int_t*);
  void putAsciiHeader(TString&);

  ClassDef(HTriggerParMuLep,1)  // Container for the MU Lepton Emulation parameters
};

#endif /* !HTRIGGERPARMULEP_H */





