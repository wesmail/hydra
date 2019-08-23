#ifndef HRPCTRBUNPACKER_H
#define HRPCTRBUNPACKER_H

#include "htrbbaseunpacker.h"
#include "hlocation.h"

class HTrbLookup;

class HRpcTrbUnpacker: public HTrbBaseUnpacker {

protected:
  HLocation loc;
  Int_t debugRpcFlag; 

public:
  HRpcTrbUnpacker(UInt_t id=802);
  ~HRpcTrbUnpacker(void) { }

  Int_t execute(void); 
  Bool_t init(void);
  void setDebugRpcFlag(Int_t db){ debugRpcFlag = db;}

public:
  ClassDef(HRpcTrbUnpacker,0) // Unpacker for Rpc data
};          

#endif /* !HRPCTRBUNPACKER_H */
