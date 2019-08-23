#ifndef HKICKPARROOTFILEIO_H
#define HKICKPARROOTFILEIO_H

#include "hdetparrootfileio.h"
#include "TFile.h"
#include "TArrayI.h"

class HParRootFile;
class HParSet;
class HKickPlane;
class HKickPlane2;
class HKickTrackFPar;
class HKickMatchPar;
class HRtMatchingPar;

class HKickParRootFileIo : public HDetParRootFileIo {
public:
  HKickParRootFileIo(HParRootFile* f);
  ~HKickParRootFileIo();
  Bool_t init(HParSet*,Int_t*);
  Bool_t read(HKickPlane*);
  Bool_t read(HKickPlane2* pPar);
  Bool_t read(HKickTrackFPar*);
  Bool_t read(HKickMatchPar *);
  Bool_t read(HRtMatchingPar *);

  ClassDef(HKickParRootFileIo,1) // Class for Kick parameter I/O from ROOT file
};

#endif  /*!HKICKPARROOTFILEIO_H*/







