#ifndef HSPECPARROOTFILEIO_H
#define HSPECPARROOTFILEIO_H

#include "hdetparrootfileio.h"

class HParRootFile;
class HParSet;
class HSpecGeomPar;

class HSpecParRootFileIo : public HDetParRootFileIo {
public:
  HSpecParRootFileIo(HParRootFile* f);
  ~HSpecParRootFileIo() {}
  Bool_t init(HParSet*,Int_t*);
  Bool_t read(HSpecGeomPar*,Int_t*);
  ClassDef(HDetParRootFileIo,0) // Class for spectrometer parameter I/O from ROOT file 
};

#endif  /* !HSPECPARROOTFILEIO_H */
 
