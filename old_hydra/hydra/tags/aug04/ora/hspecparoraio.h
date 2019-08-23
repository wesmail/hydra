#ifndef HSPECPARORAIO_H
#define HSPECPARORAIO_H

#include "hdetparoraio.h"
#include "TList.h"

class HSpecGeomPar;
class HGeomOraDetVersion;

class HSpecParOraIo : public HDetParOraIo {
private:
  HGeomOraDetVersion* geomCaveVers;
  HGeomOraDetVersion* geomSectorVers;
  HGeomOraDetVersion* geomTargetVers;
  HParOraSet* alignTargetVers;
public:
  HSpecParOraIo(HOraConn* p=0);
  ~HSpecParOraIo();
  Bool_t init(HParSet*,Int_t*);
  Int_t write(HParSet*);
private:
  Bool_t read(HSpecGeomPar*,Int_t*);
  Bool_t readTargetVolumes(HSpecGeomPar*,TList&);
  Int_t writeAlignment(HSpecGeomPar*);
  ClassDef(HSpecParOraIo,0) // Parameter I/O from Oracle for spectrometer
};

#endif  /* !HSPECPARORAIO_H */
