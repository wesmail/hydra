#ifndef HWALLPARORAIO_H
#define HWALLPARORAIO_H

#include "hdetparoraio.h"
#include "TString.h"

class HOraConn;
class HWallLookup;
class HWallCalPar;
class HWallGeomPar;

class HWallParOraIo : public HDetParOraIo {
private:
  Int_t runStart;               // start time of actual run
  HGeomOraDetVersion* geomVers; // geometry version
public:
  HWallParOraIo(HOraConn* p=0);
  ~HWallParOraIo();
  Bool_t init(HParSet*,Int_t*);
  Int_t write(HParSet*);
private:
  Int_t startIo(HParSet*);
  Int_t getVersion(HParSet*);
  Bool_t read(HWallLookup*);
  Bool_t read(HWallCalPar*);
  Bool_t read(HWallGeomPar*,Int_t*);
  Bool_t readModGeomNames(HWallGeomPar* pPar);
  Bool_t readCompGeomNames(HWallGeomPar* pPar);
  Int_t createVers(HParSet*);
  Int_t writePar(HWallLookup*);
  Int_t writePar(HWallCalPar*);
  ClassDef(HWallParOraIo,0) // Forward Wall parameter I/O from Oracle
};

#endif  /* !HWALLPARORAIO_H */
