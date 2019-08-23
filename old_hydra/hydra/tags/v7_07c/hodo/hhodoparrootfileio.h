#ifndef HHODOPARROOTFILEIO_H
#define HHODOPARROOTFILEIO_H

#include "hdetparrootfileio.h"
#include "TFile.h"
#include "TArrayI.h"

class HParRootFile;
class HParSet;
class HHodoLookup;

class HHodoParRootFileIo : public HDetParRootFileIo {
public:
  HHodoParRootFileIo(HParRootFile* f);
  ~HHodoParRootFileIo();
  Bool_t init(HParSet*,Int_t*);
  Bool_t read(HHodoLookup*,Int_t*);
  ClassDef(HHodoParRootFileIo,0) // Parameter I/O from ROOT file for the Pion Hodoscopes
};

#endif  /* !HHODOPARROOTFILEIO_H */










