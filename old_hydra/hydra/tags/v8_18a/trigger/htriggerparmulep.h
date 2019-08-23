//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/11/2005
//*-- Version: $Id: htriggerparmulep.h,v 1.6 2008-05-09 16:18:42 halo Exp $

#ifndef HTRIGGERPARMULEP_H
#define HTRIGGERPARMULEP_H

#include "hparcond.h"
#include "TString.h"

// Forward declarations
class HParamList; 

class HTriggerParMuLep : public HParCond {
private: 
  Int_t MUtheta;      // MU theta cut
  Int_t MUphi_offset; // MU phi offset
  Int_t MUphi_slope;  // MU phi slope
  Int_t MUfield;      // MU field value
  Float_t MUtof;      // MU tof cut
     
public:
  HTriggerParMuLep(const Char_t* name="TriggerParMuLep",
                  const Char_t* title="Parameters for MU Lepton Emulation",
                  const Char_t* context="TriggerProduction");
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
  void setMUtof(Float_t tof) { MUtof = tof; }  

  void setDefaults(void);

  Bool_t init(HParIo*,Int_t*);
  Int_t write(HParIo*);
  void putParams(HParamList*);
  Bool_t getParams(HParamList*); 
  void clear();
  void printParam();
  void readline(const Char_t*, Int_t*);
  void putAsciiHeader(TString&);

  ClassDef(HTriggerParMuLep,1)  // Container for the MU Lepton Emulation parameters
};

#endif /* !HTRIGGERPARMULEP_H */





