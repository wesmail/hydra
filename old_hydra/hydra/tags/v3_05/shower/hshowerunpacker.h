// File: showerunpack.h
//
// Author: Walter Karig <W.Karig@gsi.de>
// Last update: 98/01/20 21:25:08
//

#ifndef HShowerUnpacker_H
#define HShowerUnpacker_H

#include "haddef.h"
#include "hldsubevt.h"
#include "hldunpack.h"

//default unpacker 

class HShowerUnpacker: public HldUnpack {
public:
  HShowerUnpacker();
  ~HShowerUnpacker();

  virtual int getSubEvtId() const { return 300; }
  virtual Byte_t eventType(){return kRealEvent;}
  int execute();
  Bool_t init(void);

private:
  Int_t getChargeHigh(UInt_t dataWord);
  Int_t getChargeLow(UInt_t dataWord);
  Int_t getSector(UInt_t dataWord); //not defined 
  Int_t getModule(UInt_t dataWord);

  Int_t getRowHigh(UInt_t dataWord);
  Int_t getRowLow(UInt_t dataWord);
  Int_t getRowOffset(UInt_t dataWord);

  Int_t getColumn(UInt_t dataWord);
  Int_t frameIdentifier(UInt_t dataWord);
  Int_t dataStatus(UInt_t dataWord);

  Int_t fillData(class HLocation &, Int_t nCharge);
  int dump();

  int nEvNr;

  ClassDef(HShowerUnpacker,1)		// unpack Shower data
};

#endif /* !HShowerUnpacker_H */
