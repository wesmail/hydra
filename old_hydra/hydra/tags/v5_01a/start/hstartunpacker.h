#ifndef HSTARTUNPACKER_H
#define HSTARTUNPACKER_H

#include "hldunpack.h"
#include "hlocation.h"

class HCategory;
class HStartLookup;

class HStartUnpacker: public HldUnpack {
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
  HStartUnpacker(Int_t id = 501);
  HStartUnpacker(const HStartUnpacker& start);
  ~HStartUnpacker(void) {;}
  Int_t getSubEvtId() const { return subEvtId; }
  Bool_t init(void);
  Int_t execute(void);
  
  //  decoding of CAEN ADCs and TDCs
  Int_t getData(const Short_t data) const { return data & 0xfff; }

  ClassDef(HStartUnpacker, 1)	// unpack START data 12/99
};

#endif /* ! HSTARTUNPACKER_H */

















