#ifndef HMDCPARFILEIO_H
#define HMDCPARFILEIO_H

#include "hdetparfileio.h"
#include "TFile.h"
#include "TArrayI.h"

class HParRootFile;
class HParSet;
class HMdcRawStruct;
class HMdcGeomStruct;
class HMdcCalPar;
class HMdcLookupGeom;

class HMdcParFileIo : public HDetParFileIo {
public:
  HMdcParFileIo(HParRootFile* f);
  ~HMdcParFileIo();
  Bool_t init(HParSet*,Int_t*);
  Bool_t read(HMdcRawStruct*,Int_t*);
  Bool_t read(HMdcGeomStruct*,Int_t*);
  Bool_t read(HMdcCalPar*,Int_t*);
  Bool_t read(HMdcLookupGeom*,Int_t*);
  ClassDef(HMdcParFileIo,0) // Class for MDC parameter I/O from ROOT file
};

#endif  /*!HMDCPARFILEIO_H*/

