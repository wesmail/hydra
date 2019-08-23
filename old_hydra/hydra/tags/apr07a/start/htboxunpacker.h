#ifndef HTBOXUNPACKER_H
#define HTBOXUNPACKER_H

#include "hldunpack.h"
#include "hlocation.h"

#define LATCH         0x4
#define SCALER        0x5
#define SCALER_JAN04  0x6  //Changed in scaler format jan04
#define SCALER_AUG04  0x9  //Changed in scaler format aug04
#define DEBUG         0x3f //in scaler format jan04

#define SCALER_MAX_WORDS 64

class HCategory;

class HTBoxUnpacker: public HldUnpack {
  protected:
    Int_t subEvtId;      //! subevent id
    HLocation loc;       //! location in the matrix category for tbox data
    HCategory* tboxCat;  //! pointer to the TBox category
    UInt_t scalData[SCALER_MAX_WORDS]; //! scaler data
  public:
    HTBoxUnpacker(Int_t id=416);
    ~HTBoxUnpacker(void) {}
    Int_t getSubEvtId() const { return subEvtId; }
    Int_t execute();
    Bool_t init(void);
  protected:
    void clearScalData(void) {
      for(Int_t i=0;i<SCALER_MAX_WORDS;i++) scalData[i]=0;
    }
    UInt_t *fillScaler(UInt_t*,UInt_t*,Bool_t bOld=kFALSE,Int_t nType=SCALER);
    void fillScalerData(Int_t,UInt_t*,Int_t);
    void fillScalers(Int_t);
    UInt_t* skipLatch(UInt_t*);
    ClassDef(HTBoxUnpacker,0) // unpack scaler data
};

#endif /* !HTBOXUNPACKER_H */











