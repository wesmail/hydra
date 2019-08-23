// File: showerunpackexp.h
//
// Author: Walter Karig <W.Karig@gsi.de>
// Last update: 98/01/20 21:25:08
//

#ifndef HShowerUnpackerExp_H
#define HShowerUnpackerExp_H

#include "hldsubevt.h"
#include "hldunpack.h"

class HShowerUnpackerExp: public HldUnpack {
public:
  HShowerUnpackerExp();
  ~HShowerUnpackerExp();

  int getSubEvtId() const { return 300; }
  int execute();
  Bool_t init(void);

private:
  int getSector(int ) { return 0; }
  int getModule(int adcNr);
  int getRow(int adcNr);
  int getColumn(int adcNr);
  int dump();


  ClassDef(HShowerUnpackerExp,1)		// unpack Shower data
};

#endif /* !HShowerUnpackerExp_H */
