//*-- Author: Ilse Koenig
//*-- Modified: Joern Wuestenfeld 07/14/2005
//*-- Version: $Id: htriggerparasciifileio.h,v 1.6 2005-07-15 09:00:25 wuestenf Exp $

#ifndef HTRIGGERPARASCIIFILEIO_H
#define HTRIGGERPARASCIIFILEIO_H

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
