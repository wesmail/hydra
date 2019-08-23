#ifndef HTOFUNPACKER_H
#define HTOFUNPACKER_H

// File: tofunpack.h
//
// Author: Walter Karig <W.Karig@gsi.de>
// Last update: 97/11/27 19:48:46
//
#include "hldsubevt.h"
#include "hldunpack.h"

class HTofUnpacker: public HldUnpack {
public:
  HTofUnpacker(void);
  ~HTofUnpacker(void);
  int getSubEvtId() const { return 400; }
  int byte(int b, UInt4 i) { return i >> 32 - 8*b & 0xff; }
  int getSector(int) {return 0;}
  int getModule(int chan);
  int getCell(int chan) {return chan;}
  int getHit(int) {return 0;}
  int execute(); 
  Bool_t init(void);
protected:
  int fill();
  UInt_t *pTofData;
public:
  ClassDef(HTofUnpacker,0)         // unpack TOF data
};          

#endif /* !HTOFUNPACKER_H */
