#ifndef HHODOTRBUNPACKER_H
#define HHODOTRBUNPACKER_H

#include "htrbbaseunpacker.h"
#include "hlocation.h"
#include "TH2F.h"

class HTrbLookup;
class HHodoRefHitPar;

class HHodoTrbUnpacker: public HTrbBaseUnpacker {

protected:
  HLocation loc;
  HHodoRefHitPar *refhit;

  Bool_t fNoTimeRefCorr;// Switch of Reference Time substraction (debugging)
  Bool_t fControlHistograms;  // flag for control histograms
  TH2F *histdiagraw, *histdiagdif, *multdiag;// control histos

	Int_t forced_TimeRefType;// if !=-1  use this type of time ref correction

public:
  HHodoTrbUnpacker(UInt_t id=0);
  ~HHodoTrbUnpacker(void) { }

  Int_t execute(void);
  Bool_t init(void);
  Bool_t finalize(void);

  Int_t correctRefTimeChVar(void);// test, will be transfered to base unpacker lateron
  
  void correctRefTimeWKStyle(void);// trigger time ORed on serveryl channels; wolfgang style NOT USED UNTIL NOW
  void disableTimeRef(void){fNoTimeRefCorr=true;};// Switch off Reference Time substraction (debugging)
  void setTimeRefType(Int_t f){forced_TimeRefType=f;};// Change Reference Time substraction type (for debugging or wrongly initialized TRB)
  void enableControlHistograms() { fControlHistograms = kTRUE; }// Switch on control histos

public:
  ClassDef(HHodoTrbUnpacker,0) // Unpacker for HODO data
};

#endif /* !HHODOTRBUNPACKER_H */







