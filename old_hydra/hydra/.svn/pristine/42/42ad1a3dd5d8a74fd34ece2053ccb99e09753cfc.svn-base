#ifndef HTRIGGERPARASCIIFILEIO_H
#define HTRIGGERPARASCIIFILEIO_H

#include <fstream.h>

#include "TObject.h"
#include "TArrayI.h"
#include "hdetparasciifileio.h"

class HParSet;

class HTriggerParAsciiFileIo : public HDetParAsciiFileIo {
public:
  HTriggerParAsciiFileIo(fstream*);
  ~HTriggerParAsciiFileIo() {}
  Bool_t init(HParSet*,Int_t*);
  Int_t write(HParSet*);
  template<class T> Bool_t read(T*, Int_t*, Bool_t needsClear=kFALSE);
  template<class T> Int_t writeFile3(T*);
  ClassDef(HTriggerParAsciiFileIo,0) // Class for Trigger parameter I/O from Ascii files
};

#endif  /* !HTRIGGERPARASCIIFILEIO_H */
