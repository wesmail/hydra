#ifndef HRICHPARROOTFILEIO_H
#define HRICHPARROOTFILEIO_H

#include "hdetparrootfileio.h"
#include "TFile.h"
#include "TArrayI.h"

class HParRootFile;
class HParSet;
class HRichParSet;
class HRichGeometry;
class HRichDigiDetPar;
class HRichHitFPar;
class HRichCalPar;

class HRichParRootFileIo : public HDetParRootFileIo {
public:
  HRichParRootFileIo(HParRootFile* f);
  ~HRichParRootFileIo();
  Bool_t init(HParSet*,Int_t*);
  Bool_t readObject(HRichParSet* pPar);

  Bool_t read(HRichGeometry* pPar,Int_t* set);
  Bool_t read(HRichDigiDetPar* pPar,Int_t* set);
  Bool_t read(HRichHitFPar* pPar,Int_t* set);
  Bool_t read(HRichCalPar* pPar,Int_t* set);

  ClassDef(HRichParRootFileIo,0) // Class for Rich parameter I/O from ROOT file
};

#endif  // HRICHPARROOTFILEIO_H

