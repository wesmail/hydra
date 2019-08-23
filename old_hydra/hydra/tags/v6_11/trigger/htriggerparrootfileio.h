#ifndef HTRIGGERPARROOTFILEIO_H
#define HTRIGGERPARROOTFILEIO_H

#include "hdetparrootfileio.h"
#include "TFile.h"
#include "TArrayI.h"

class HParRootFile;
class HParSet;
class HTriggerParMuLep;
class HTriggerParMuDilep;
class HTriggerParMomMap;
class HTriggerParShowerMap;

class HTriggerParRootFileIo : public HDetParRootFileIo {
public:
  HTriggerParRootFileIo(HParRootFile* f);
  ~HTriggerParRootFileIo();
  Bool_t init(HParSet*,Int_t*);
  Bool_t read(HTriggerParMuLep*,Int_t*);
  Bool_t read(HTriggerParMuDilep*,Int_t*);
  Bool_t read(HTriggerParMomMap*,Int_t*);
  Bool_t read(HTriggerParShowerMap*,Int_t*);

  ClassDef(HTriggerParRootFileIo,0) // Class for TRIGGER parameter I/O from ROOT file
};

#endif  /*!HTRIGGERPARROOTFILEIO_H*/
