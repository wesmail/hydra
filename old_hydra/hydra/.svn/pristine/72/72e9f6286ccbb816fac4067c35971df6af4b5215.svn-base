#ifndef HHODOUNPACKER_H
#define HHODOUNPACKER_H

#include "hldunpack.h"
#include "hlocation.h"

class HHodoLookup;

class HHodoUnpacker: public HldUnpack {
protected:
  UInt_t subEvtId;     // subevent id
  HLocation loc;       // location in the raw data category
  HHodoLookup* lookup; // pointer to lookup table

private:
	Bool_t UnpackTDCv1190(UInt_t *data,UInt_t uBlockSize);// Unpack TDC1190 data

public:
  HHodoUnpacker(UInt_t id=416);
  ~HHodoUnpacker(void) {}
  Int_t getSubEvtId(void) const { return subEvtId; }
  Int_t execute(void); 
  Bool_t init(void);
public:
  ClassDef(HHodoUnpacker,0) // Unpacker for data of the Hodoscopes
};          

#endif /* !HHODOUNPACKER_H */







