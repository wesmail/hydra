#ifndef HTRIGGERPARASCIIFILEIO_H
#define HTRIGGERPARASCIIFILEIO_H

#include <fstream.h>

#include "TObject.h"
#include "TArrayI.h"
#include "hdetparasciifileio.h"

class HParSet;
class HTriggerParMomMap;
class HTriggerParShowerMap;
class HTriggerParRichMap;
class HRichIPUParPattern;
class HRichIPUParThresholds;

class HTriggerParAsciiFileIo : public HDetParAsciiFileIo {
public:
  HTriggerParAsciiFileIo(fstream*);
  ~HTriggerParAsciiFileIo() {}
  Bool_t init(HParSet*,Int_t*);
  Int_t write(HParSet*);
  template<class T> Bool_t read(T*, Int_t*, Bool_t needsClear=kFALSE);
  Int_t writeFile(HTriggerParMomMap*);
  Int_t writeFile(HTriggerParShowerMap*);
  Int_t writeFile(HTriggerParRichMap*);
  Int_t writeFile(HRichIPUParPattern*);
  Int_t writeFile(HRichIPUParThresholds*);
  ClassDef(HTriggerParAsciiFileIo,0) // Class for Trigger parameter I/O from Ascii files
};

#endif  /* !HTRIGGERPARASCIIFILEIO_H */
