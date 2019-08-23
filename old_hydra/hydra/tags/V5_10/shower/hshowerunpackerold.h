// File: showerunpackold.h
//
// Author: Walter Karig <W.Karig@gsi.de>
// Last update: 98/01/20 21:25:08
//

#ifndef HShowerUnpackerOld_H
#define HShowerUnpackerOld_H

#include "hldsubevt.h"
#include "hldunpack.h"

class HShowerUnpackerOld: public HldUnpack {
public:
  HShowerUnpackerOld();
  ~HShowerUnpackerOld();

  int getSubEvtId() const { return 7; }
  int execute();
  Bool_t init(void);

private:
  int getSector(int ) { return 0; }
  int getModule(int adcNr);
  int getRow(int adcNr);
  int getColumn(int adcNr);
  int dump();

  int nEvNr;

  ClassDef(HShowerUnpackerOld,1)		// unpack Shower data
};

#endif /* !HShowerUnpackerOld_H */
