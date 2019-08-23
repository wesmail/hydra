#ifndef HKICKPARASCIIFILEIO_H
#define HKICKPARASCIIFILEIO_H

#include <fstream.h>

#include "TObject.h"
#include "TArrayI.h"
#include "hdetparasciifileio.h"

class HParSet;

class HKickParAsciiFileIo : public HDetParAsciiFileIo {
public:
  HKickParAsciiFileIo(fstream*);
  ~HKickParAsciiFileIo() {}
  Bool_t init(HParSet*,Int_t*);
  Int_t write(HParSet*);
  ClassDef(HKickParAsciiFileIo,0) // Class for Kick parameter I/O from Ascii files
};

#endif  /* !HKICKPARASCIIFILEIO_H */



