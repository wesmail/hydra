#ifndef HGO4PARASCIIFILEIO_H
#define HGO4PARASCIIFILEIO_H

#include <fstream.h>
#include "hdetparasciifileio.h"

class HParSet;

class HGo4ParAsciiFileIo : public HDetParAsciiFileIo {
public:
  HGo4ParAsciiFileIo(fstream*);
  ~HGo4ParAsciiFileIo() {}
  Bool_t init(HParSet*,Int_t*);
  Int_t write(HParSet*);
  ClassDef(HGo4ParAsciiFileIo,0) // Class for MDC parameter I/O from Ascii files
};

#endif  /* !HGO4PARASCIIFILEIO_H */



