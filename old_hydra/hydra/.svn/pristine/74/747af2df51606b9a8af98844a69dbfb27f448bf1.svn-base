#ifndef HSTARTUNPACKER_H
#define HSTARTUNPACKER_H

#include "hldunpack.h"
#include "hlocation.h"
#define LATCH   0x4
#define SCALER  0x5
#define SCALER_JAN04  0x6 //Changed in scaler format jan04
#define DEBUG  0x3f //in scaler format jan04

class HIterator;
class HCategory;
class HStartLookup;

class HStartUnpacker: public HldUnpack {
friend class  HStartMonitor;
protected:
  Int_t subEvtId;           //! subevent id
  HLocation loc;
  HIterator* iter;
  HCategory* tboxCat;       //!
  HStartLookup* lookup;
  Int_t scalData[32];       //!

  void clearScalData(void) {
    for(Int_t i=0;i<32;i++) scalData[i]=0;
  }
public:
  HStartUnpacker(Int_t id=412);
  ~HStartUnpacker(void) {}
  Int_t getSubEvtId() const { return subEvtId; }
  Int_t execute();
  Bool_t init(void);
	UInt_t *fillScaler(UInt_t *pData,UInt_t *uTrigbits,Bool_t bOld=kFALSE,Int_t nType=SCALER);
    UInt_t *HStartUnpacker::fillLatch(UInt_t *pData);
  ClassDef(HStartUnpacker,0)         // unpack START data
};

#endif /* !HSTARTUNPACKER_H */











