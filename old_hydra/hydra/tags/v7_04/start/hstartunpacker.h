#ifndef HSTARTUNPACKER_H
#define HSTARTUNPACKER_H

#include "hldunpack.h"
#include "hlocation.h"
#define LATCH   0x4
#define SCALER  0x5
#define SCALER_JAN04  0x6 //Changed in scaler format jan04
#define LATCH_AUG04  0x8 //Changed in latch format aug04
#define SCALER_AUG04  0x9 //Changed in scaler format aug04
#define DEBUG  0x3f //in scaler format jan04

#define SCALER_MAX_WORDS 64

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
  UInt_t scalData[SCALER_MAX_WORDS];       //!

  void clearScalData(void) {
    for(Int_t i=0;i<SCALER_MAX_WORDS;i++) scalData[i]=0;
  }
public:
    HStartUnpacker(Int_t id=416);
    ~HStartUnpacker(void) {}
    Int_t getSubEvtId() const { return subEvtId; }
    Int_t execute();
    Bool_t init(void);
    UInt_t *fillScaler(UInt_t *pData,UInt_t *uTrigbits,Bool_t bOld=kFALSE,Int_t nType=SCALER);
    Int_t fillScalerData(Int_t nScalerNumber,UInt_t *pScalerData,Int_t nSize);
    UInt_t *HStartUnpacker::fillLatch(UInt_t *pData);
    Int_t fillScalers(Int_t nScalerCounts);
    ClassDef(HStartUnpacker,0)         // unpack START data
};

#endif /* !HSTARTUNPACKER_H */











