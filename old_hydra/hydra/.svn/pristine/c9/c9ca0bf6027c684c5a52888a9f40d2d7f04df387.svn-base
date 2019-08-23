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
class HMdcTdcThreshold;
class HMdcCal2ParSim;
class HMdcCellEff;
class HMdcTimeCut;
class HMdcLookupGeom;
class HMdcLayerGeomPar;

class HMdcHitFPar;
class HMdcLeverArmGeometry;
class HMdcModuleGeometry;
class HMdcSegmentFPar;
class HMdcTargetGeometry;
class HMdcClosePairCutsPar;



class HMdcParRootFileIo : public HDetParRootFileIo {
protected:
    TArrayI *initModuleTypes;
public:
  HMdcParRootFileIo(HParRootFile* f);
  ~HMdcParRootFileIo();
  Bool_t init(HParSet*,Int_t*);
  Bool_t read(HMdcRawStruct*,Int_t*);
  Bool_t read(HMdcGeomStruct*,Int_t*);
  Bool_t read(HMdcMboReadout*,Int_t*);
  Bool_t read(HMdcCalParRaw*,Int_t*);
  Bool_t read(HMdcTdcThreshold*,Int_t*);
  Bool_t read(HMdcCal2ParSim*,Int_t*);
  Bool_t read(HMdcCellEff*,Int_t*);
  Bool_t read(HMdcTimeCut*,Int_t*);
  Bool_t read(HMdcLookupGeom*,Int_t*);
  Bool_t read(HMdcLayerGeomPar*,Int_t*);
  Bool_t read(HMdcHitFPar*, Int_t*);
  Bool_t read(HMdcModuleGeometry*, Int_t*);
  Bool_t read(HMdcTargetGeometry*, Int_t*);
  Bool_t read(HMdcSegmentFPar*, Int_t*);
  Bool_t read(HMdcLeverArmGeometry*, Int_t*);
  Bool_t read(HMdcClosePairCutsPar*, Int_t*);

  ClassDef(HMdcParRootFileIo,0) // Class for MDC parameter I/O from ROOT file
};

#endif  /*!HMDCPARROOTFILEIO_H*/







