// File: showerunpack.h
//
// Author: Walter Karig <W.Karig@gsi.de>
// Last update: 98/01/20 21:25:08
//

#ifndef HShowerUnpackerReal_H
#define HShowerUnpackerReal_H

#include "hldsubevt.h"
#include "hldunpack.h"
#include "hshowerunpacker.h"

class HShowerUnpackerReal: public HShowerUnpacker {
public:
  HShowerUnpackerReal();

  int getSubEvtId() const { return 300; } //it's the same as HShowerUnpacker
  Byte_t eventType(){return kRealEvent;}

private:

  ClassDef(HShowerUnpackerReal,1)		// unpack Shower data
};

#endif /* !HShowerUnpackerReal_H */
