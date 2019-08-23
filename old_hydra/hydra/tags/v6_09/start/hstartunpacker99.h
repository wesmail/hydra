#ifndef HSTARTUNPACKER99_H
#define HSTARTUNPACKER99_H

#include "hldunpack.h"
#include "hlocation.h"
#include "hstartdef.h"
#include "tofinodef.h"

class HCategory;
class HStartLookup;

class HStartUnpacker99 : public HldUnpack {
friend class  HStartMonitor;
protected:
  Int_t subEvtId;              //! subevent id
  HLocation loc;               //!
  HStartLookup* lookup;        //! lookup table for unpacker
  HCategory* tboxCat;       //!
  HCategory* tofinoRawCat;     //!
  Short_t adcData[8];          //!
  Short_t tdcData[8];          //!
  Int_t scalData[32];          //!
  void clearAdcData(void) {
    for(Int_t i=0;i<8;i++) adcData[i]=-1;
  }
  void clearTdcData(void) {
    for(Int_t i=0;i<8;i++) tdcData[i]=-1;
  }
  void clearScalData(void) {
    for(Int_t i=0;i<32;i++) scalData[i]=0;
  }
public:
  HStartUnpacker99(Int_t id = 501);
  HStartUnpacker99(const HStartUnpacker99& start);
  ~HStartUnpacker99(void) {;}
  Int_t getSubEvtId() const { return subEvtId; }
  Bool_t init(void);
  Int_t execute(void);
  
  //  decoding of CAEN ADCs and TDCs
  Int_t getData(const Short_t data) const { return data & 0xfff; }

  ClassDef(HStartUnpacker99, 1)	// unpack START November 1999 data
};

#endif /* ! HSTARTUNPACKER99_H */

















