#ifndef HRPCUNPACKER_H
#define HRPCUNPACKER_H

#include "hldunpack.h"
#include "hlocation.h"

class HRpcLookup;

class HRpcUnpacker: public HldUnpack {
protected:
  UInt_t subEvtId;
  Bool_t isUnpaired;
  Int_t debug;
  HLocation loc;
  HRpcLookup* lookup;
public:
  HRpcUnpacker(UInt_t id=0x222, Bool_t unpaired=kFALSE, Int_t dbg=0);  
  ~HRpcUnpacker(void) {}
  Int_t getSubEvtId(void) const { return subEvtId; }
  Int_t execute(void); 
  Bool_t init(void);
public:
  ClassDef(HRpcUnpacker,0) // Unpacker for the RPC Detector data
};          

#endif /* !HRPCUNPACKER_H */


