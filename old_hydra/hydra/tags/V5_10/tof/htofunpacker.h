#ifndef HTOFUNPACKER_H
#define HTOFUNPACKER_H

// File: tofunpack.h
//
// Author: D. Vassiliev
// Last update: 27/5/2000
//
#include "hldsubevt.h"
#include "hldunpack.h"

class HTofUnpacker: public HldUnpack {
public:
  HTofUnpacker(void);
  ~HTofUnpacker(void);
  int getSubEvtId() const { return 410; }
  int byte(int b, UInt4 i) { return i >> 32 - 8*b & 0xff; }
  int execute(); 
  Bool_t init(void);
protected:
  int fill();
  UInt_t *pTofData;

public:
  ClassDef(HTofUnpacker,0)         // unpack TOF May 2000 data
};          

#endif /* !HTOFUNPACKER_H */
