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
  TH2F *histdiagraw, *histdiagdif;// control histos

public:
  HHodoTrbUnpacker(UInt_t id=0);
  ~HHodoTrbUnpacker(void) { }

  Int_t execute(void);
  Bool_t init(void);
  Bool_t finalize(void);
  
  void correctRefTimeWKStyle(void);// trigger time ORed on serveryl channels; wolfgang style NOT USED UNTIL NOW
  void disableTimeRef(void){fNoTimeRefCorr=true;};// Switch of Reference Time substraction (debugging) 
  void enableControlHistograms() { fControlHistograms = kTRUE; }// Switch on control histos

public:
  ClassDef(HHodoTrbUnpacker,0) // Unpacker for HODO data
};          

#endif /* !HHODOTRBUNPACKER_H */







