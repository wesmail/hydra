#ifndef HKICKPARROOTFILEIO_H
#define HKICKPARROOTFILEIO_H

#include "hdetparrootfileio.h"
#include "TFile.h"
#include "TArrayI.h"

class HParRootFile;
class HParSet;
class HKickPlane;
class HKickTrackFPar;

class HKickParRootFileIo : public HDetParRootFileIo {
public:
  HKickParRootFileIo(HParRootFile* f);
  ~HKickParRootFileIo();
  Bool_t init(HParSet*,Int_t*);
  Bool_t read(HKickPlane*);
  Bool_t read(HKickTrackFPar*);

  ClassDef(HKickParRootFileIo,1) // Class for Kick parameter I/O from ROOT file
};

#endif  /*!HKICKPARROOTFILEIO_H*/







