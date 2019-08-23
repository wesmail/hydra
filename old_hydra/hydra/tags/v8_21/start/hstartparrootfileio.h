#ifndef HSTARTPARROOTFILEIO_H
#define HSTARTPARROOTFILEIO_H

#include "hdetparrootfileio.h"
#include "TFile.h"
#include "TArrayI.h"

class HParRootFile;
class HParSet;
class HStartLookup;
class HStartCalPar;
class HStartSelfCoPar;

class HStartParRootFileIo : public HDetParRootFileIo {
public:
  HStartParRootFileIo(HParRootFile* f);
  ~HStartParRootFileIo();
  Bool_t init(HParSet*,Int_t*);
  Bool_t read(HStartLookup*,Int_t*);
  Bool_t read(HStartCalPar*,Int_t*);
  Bool_t readStartModule(HStartSelfCoPar*,Int_t*);
  ClassDef(HStartParRootFileIo,0) // Class for START parameter I/O from ROOT file
};

#endif  /* !HSTARTPARROOTFILEIO_H */










