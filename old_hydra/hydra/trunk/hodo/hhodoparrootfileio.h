#ifndef HHODOPARROOTFILEIO_H
#define HHODOPARROOTFILEIO_H

#include "hdetparrootfileio.h"
#include "TFile.h"
#include "TArrayI.h"

class HParRootFile;
class HParSet;
class HHodoLookup;
class HHodoCalPar;
class HHodoRefHitPar;

class HHodoParRootFileIo : public HDetParRootFileIo {
public:
  HHodoParRootFileIo(HParRootFile* f);
  ~HHodoParRootFileIo();
  Bool_t init(HParSet*,Int_t*);

  Bool_t read(HHodoLookup*,Int_t*);
  Bool_t read(HHodoCalPar*,Int_t*);
  Bool_t read(HHodoRefHitPar*,Int_t*);
//  template<class T> Bool_t read(T*, Int_t*);

  ClassDef(HHodoParRootFileIo,0) // Parameter I/O from ROOT file for the Pion Hodoscopes
};

#endif  /* !HHODOPARROOTFILEIO_H */










