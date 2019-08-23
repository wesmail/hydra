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
class HMdcCalParRaw;
class HMdcLookupGeom;
class HMdcDigitPar;
class HMdcLayerGeomPar;

class HMdcHitFPar;
class HMdcLeverArmGeometry;
class HMdcModuleGeometry;
class HMdcSegmentFPar;
class HMdcTargetGeometry;

class HMdcParRootFileIo : public HDetParRootFileIo {
public:
  HMdcParRootFileIo(HParRootFile* f);
  ~HMdcParRootFileIo();
  Bool_t init(HParSet*,Int_t*);
  Bool_t read(HMdcRawStruct*,Int_t*);
  Bool_t read(HMdcGeomStruct*,Int_t*);
  Bool_t read(HMdcMboReadout*,Int_t*);
  Bool_t read(HMdcCalParRaw*,Int_t*);
  Bool_t read(HMdcLookupGeom*,Int_t*);
  Bool_t read(HMdcDigitPar*,Int_t*);
  Bool_t read(HMdcLayerGeomPar*,Int_t*);
  Bool_t read(HMdcHitFPar*, Int_t*);
  Bool_t read(HMdcModuleGeometry*, Int_t*);
  Bool_t read(HMdcTargetGeometry*, Int_t*);
  Bool_t read(HMdcSegmentFPar*, Int_t*);
  Bool_t read(HMdcLeverArmGeometry*, Int_t*);

  ClassDef(HMdcParRootFileIo,0) // Class for MDC parameter I/O from ROOT file
};

#endif  /*!HMDCPARROOTFILEIO_H*/







