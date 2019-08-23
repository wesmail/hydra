#ifndef HSHOWERPARROOTFILEIO_H
#define HSHOWERPARROOTFILEIO_H

#include "hdetparrootfileio.h"
#include "TFile.h"
#include "TObject.h"
#include "TArrayI.h"
#include "TKey.h"

class HParRootFile;
class HParSet;
class HShowerParSet;
class HShowerGeometry;
class HShowerDigiDetPar;
class HShowerHitFPar;
class HShowerCalPar;
class HShowerCalAsic;
class HShowerHist;
class HShowerLookupAsic;
class HShowerUnpackParam;

class HShowerParRootFileIo : public HDetParRootFileIo {
public:
  HShowerParRootFileIo(HParRootFile* f);
  ~HShowerParRootFileIo();
  Bool_t init(HParSet*,Int_t*);
  Bool_t readObject(HShowerParSet* pPar);

  Bool_t read(HShowerGeometry* pPar,Int_t* set);
  Bool_t read(HShowerDigiDetPar* pPar,Int_t* set);
  Bool_t read(HShowerHitFPar* pPar,Int_t* set);
  Bool_t read(HShowerCalPar* pPar,Int_t* set);
  Bool_t read(HShowerHist* pPar,Int_t* set);
  Bool_t read(HShowerUnpackParam* pPar,Int_t* set);


  void readContainer(HShowerParSet *pPar, char* name, Int_t version);

  ClassDef(HShowerParRootFileIo,0) // Class for Shower parameter I/O from ROOT file
};

#endif  /*!HSHOWERPARROOTFILEIO_H*/

