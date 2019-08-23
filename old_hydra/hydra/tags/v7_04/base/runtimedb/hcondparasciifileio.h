#ifndef HCONDPARASCIIFILEIO_H
#define HCONDPARASCIIFILEIO_H

using namespace std;
#include <fstream>
#include "hdetparasciifileio.h"

class HParSet;

class HCondParAsciiFileIo : public HDetParAsciiFileIo {
public:
  HCondParAsciiFileIo(fstream* f=0);
  ~HCondParAsciiFileIo() {}
  Bool_t init(HParSet*,Int_t*);
  Int_t write(HParSet*);
  ClassDef(HCondParAsciiFileIo,0) // I/O from Ascii file for parameter containers derived from HParCond
};

#endif  /* !HCONDPARASCIIFILEIO_H */
