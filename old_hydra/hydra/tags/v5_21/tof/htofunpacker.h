#ifndef HTOFUNPACKER_H
#define HTOFUNPACKER_H

//
// Author: Walter Karig <W.Karig@gsi.de>
// Last update: 15/11/2000  R. Holzmann
//
#include "hldsubevt.h"
#include "hldunpack.h"
#include "tofdef.h"

class HTofUnpacker: public HldUnpack {
public:
  HTofUnpacker(void);
  HTofUnpacker(UInt_t id);
  ~HTofUnpacker(void);
  int getSubEvtId() const { return subEvtId; }
  int byte(int b, UInt4 i) { return i >> 32 - 8*b & 0xff; }
  int execute(); 
  Bool_t init(void);
protected:
  int fill();
  UInt_t *pTofData;
  UInt_t subEvtId;
  static FILE *fp;
  static Int_t lookup[256][22];

public:
  ClassDef(HTofUnpacker,0)         // unpack TOF data
};          

#endif /* !HTOFUNPACKER_H */







