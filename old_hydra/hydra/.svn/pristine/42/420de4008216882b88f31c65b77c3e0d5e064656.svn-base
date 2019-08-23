#ifndef HTofinoParRootFileIo_h
#define HTofinoParRootFileIo_h

#include "hdetparrootfileio.h"
#include "TFile.h"
#include "TArrayI.h"

class HParRootFile;
class HParSet;
class HTofinoParSet;
class HTofinoCalPar;
class HTofinoShowerMap;

class HTofinoParRootFileIo : public HDetParRootFileIo {
public:
  HTofinoParRootFileIo(HParRootFile* f);
  ~HTofinoParRootFileIo();
  Bool_t init(HParSet*,Int_t*);
  Bool_t readObject(HTofinoParSet* pPar);

  Bool_t read(HTofinoCalPar* pPar,Int_t* set);
  Bool_t read(HTofinoShowerMap* pPar,Int_t* set);

  ClassDef(HTofinoParRootFileIo,0) // Tofino parameter I/O from ROOT file
};

#endif  /*!HTofinoParRootFileIo_h*/

