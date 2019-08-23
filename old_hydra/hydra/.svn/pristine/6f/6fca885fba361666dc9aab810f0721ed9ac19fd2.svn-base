#ifndef HTOFPARROOTFILEIO_H
#define HTOFPARROOTFILEIO_H

#include "hdetparrootfileio.h"
#include "TFile.h"
#include "TArrayI.h"

class HParRootFile;
class HParSet;
class HTofCalPar;

class HTofParRootFileIo : public HDetParRootFileIo {
public:
  HTofParRootFileIo(HParRootFile* f);
  ~HTofParRootFileIo();
  Bool_t init(HParSet*,Int_t*);
  Bool_t read(HTofCalPar*,Int_t*);
  ClassDef(HTofParRootFileIo,0) // Class for TOF parameter I/O from ROOT file
};

#endif  /*!HTOFPARROOTFILEIO_H*/
