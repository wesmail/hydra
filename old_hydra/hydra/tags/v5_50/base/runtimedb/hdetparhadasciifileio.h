#ifndef HDETPARHADASCIIFILEIO_H
#define HDETPARHADASCIIFILEIO_H

#include "hparhadasciifileio.h"
#include "hdetpario.h"
#include "TArrayI.h"
#include "hgeomtransform.h"

class HDetParHadAsciiFileIo : public HDetParIo {
protected:
  HParHadAsciiFileIo* pFile;  // pointer to ascii file

public:
  HDetParHadAsciiFileIo(HParHadAsciiFileIo* f);
  ~HDetParHadAsciiFileIo() {}

  ClassDef(HDetParHadAsciiFileIo,0) // Class for detector parameter I/O from ascii file
};

#endif  /* !HDETPARHADASCIIFILEIO_H */
