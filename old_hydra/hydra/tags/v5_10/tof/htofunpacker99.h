#ifndef HTOFUNPACKER99_H
#define HTOFUNPACKER99_H

// File: tofunpacker99.h
//
// Author: Walter Karig <W.Karig@gsi.de>
// Last update: 28/5/2000 by R. Holzmann 
//
#include "hldsubevt.h"
#include "hldunpack.h"

class HTofUnpacker99: public HldUnpack {
public:
  HTofUnpacker99(void);
  ~HTofUnpacker99(void);
  int getSubEvtId() const { return 400; }
  int byte(int b, UInt4 i) { return i >> 32 - 8*b & 0xff; }
  int execute(); 
  Bool_t init(void);
protected:
  int fill();
  UInt_t *pTofData;

public:
  ClassDef(HTofUnpacker99,0)         // unpack TOF November 1999 data
};          

#endif /* !HTOFUNPACKER99_H */
