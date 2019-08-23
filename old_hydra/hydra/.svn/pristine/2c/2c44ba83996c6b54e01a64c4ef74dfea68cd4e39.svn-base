#ifndef HLATCHUNPACKER_H
#define HLATCHUNPACKER_H

#include "hldunpack.h"
#include "hlocation.h"

#define LATCH         0x4
#define SCALER        0x5
#define SCALER_JAN04  0x6  //Changed in scaler format jan04
#define LATCH_AUG04   0x8  //Changed in latch format aug04
#define SCALER_AUG04  0x9  //Changed in scaler format aug04
#define DEBUG         0x3f //in scaler format jan04

class HLatchUnpacker: public HldUnpack {
protected:
  Int_t subEvtId;           //! subevent id
public:
  HLatchUnpacker(Int_t id=416);
  ~HLatchUnpacker(void) {}
  Int_t getSubEvtId() const { return subEvtId; }
  Int_t execute();
  Bool_t init(void) { return kTRUE; }
protected:
  UInt_t* HLatchUnpacker::fillLatch(UInt_t* pData);
  UInt_t* skipScaler(UInt_t*,Bool_t bOld=kFALSE,Int_t nType=SCALER);

  ClassDef(HLatchUnpacker,0) // unpack LATCH data
};

#endif /* !HLATCHUNPACKER_H */











