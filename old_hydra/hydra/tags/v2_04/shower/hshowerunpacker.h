// File: showerunpack.h
//
// Author: Walter Karig <W.Karig@gsi.de>
// Last update: 98/01/20 21:25:08
//

#ifndef HShowerUnpacker_H
#define HShowerUnpacker_H

#include "hldsubevt.h"
#include "hldunpack.h"

class HShowerUnpacker: public HldUnpack {
public:
  HShowerUnpacker();
  ~HShowerUnpacker();

  int getSubEvtId() const { return 300; }
  int execute();
  Bool_t init(void);

private:
  Int_t getCharge(UInt_t dataWord);
  Int_t getSector(UInt_t dataWord);
  Int_t getModule(UInt_t dataWord);
  Int_t getRow(UInt_t dataWord);
  Int_t getColumn(UInt_t dataWord);
  int dump();


  ClassDef(HShowerUnpacker,1)		// unpack Shower data
};

#endif /* !HShowerUnpacker_H */
