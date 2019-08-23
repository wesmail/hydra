#ifndef HSTARTUNPACKER_H
#define HSTARTUNPACKER_H

//
// Author: Walter Karig <W.Karig@gsi.de>
// Last update: 15/11/2000 R. Holzmann
//
#include "hldsubevt.h"
#include "hldunpack.h"
#include "hstartdef.h"
#include "tofinodef.h"
#include "hlocation.h"

class HStartUnpacker: public HldUnpack {
friend class  HStartMonitor;

protected:
  Int_t subEvtId;              //! subevent id
  HLocation loc;               
  HLocation locStart;               
  HCategory* tboxCat;          
  HCategory* pRawCat;          
  HCategory* tofinoRawCat;     
  Int_t fill();
  UInt_t *pStartData;
  static FILE *fp;
  static Int_t lookup[256][22];

public:
  HStartUnpacker(Int_t id=410);
  ~HStartUnpacker(void);
  Int_t getSubEvtId() const { return subEvtId; }
  Int_t byte(int b, UInt4 i) { return i >> 32 - 8*b & 0xff; }
  Int_t execute(); 
  Bool_t init(void);

  ClassDef(HStartUnpacker,0)         // unpack START+SCALLERS+TOFINO data
};          

#endif /* !HSTARTUNPACKER_H */













