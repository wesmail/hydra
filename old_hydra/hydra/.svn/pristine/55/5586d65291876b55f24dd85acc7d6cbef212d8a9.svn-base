#ifndef HHODOTRBUNPACKER_H
#define HHODOTRBUNPACKER_H

#include "htrbbaseunpacker.h"
#include "hlocation.h"

class HTrbLookup;
class HHodoRefHitPar;

class HHodoTrbUnpacker: public HTrbBaseUnpacker {

protected:
  HLocation loc;
  Int_t doRefTimeCorr;
  HHodoRefHitPar *refhit;

public:
  HHodoTrbUnpacker(UInt_t id=0);
  ~HHodoTrbUnpacker(void) { }

  Int_t execute(void);
  Bool_t init(void);
  void setRefTimeCorrFlag(Int_t fl){ doRefTimeCorr = fl; }

  void correctRefTimeWKStyle(void);// trigger time ORed on serveryl channels; wolfgang style

public:
  ClassDef(HHodoTrbUnpacker,0) // Unpacker for HODO data
};          

#endif /* !HHODOTRBUNPACKER_H */







