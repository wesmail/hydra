#ifndef HMDCPARROOTFILEIO_H
#define HMDCPARROOTFILEIO_H

#include "hdetparrootfileio.h"
#include "TFile.h"
#include "TArrayI.h"

class HParRootFile;
class HParSet;
class HMdcRawStruct;
class HMdcGeomStruct;
class HMdcMboReadout;
class HMdcCalPar;
class HMdcLookupGeom;
class HMdcDigitPar;
class HMdcLayerGeomPar;

class HMdcParRootFileIo : public HDetParRootFileIo {
public:
  HMdcParRootFileIo(HParRootFile* f);
  ~HMdcParRootFileIo();
  Bool_t init(HParSet*,Int_t*);
  Bool_t read(HMdcRawStruct*,Int_t*);
  Bool_t read(HMdcGeomStruct*,Int_t*);
  Bool_t read(HMdcMboReadout*,Int_t*);
  Bool_t read(HMdcCalPar*,Int_t*);
  Bool_t read(HMdcLookupGeom*,Int_t*);
  Bool_t read(HMdcDigitPar*,Int_t*);
  Bool_t read(HMdcLayerGeomPar*,Int_t*);
  ClassDef(HMdcParRootFileIo,0) // Class for MDC parameter I/O from ROOT file
};

#endif  /*!HMDCPARROOTFILEIO_H*/

