#ifndef HKICKPARORAIO_H
#define HKICKPARORAIO_H

#include "hdetparoraio.h"

class HKickPlane;
class HKickPlane2;
class HKickMatchPar;
class HSurface;
class HKickGrid;

class HKickParOraIo : public HDetParOraIo {
private:
  Int_t runStart;    // start time of current run
  Int_t idMdc3;      // Id for Mdc3 related parameters
  Int_t idMeta;      // Id for Meta related parameters
public:
  HKickParOraIo(HOraConn* p=0);
  ~HKickParOraIo() {}
  Bool_t init(HParSet*,Int_t*);
  Int_t write(HParSet*);
private:
  Int_t startIo(HParSet*);
  void readIds();
  Int_t getVersion(HParSet*,Int_t,Int_t&);
  Bool_t read(HKickPlane2*,Int_t);
  Bool_t read(HKickMatchPar*,Int_t);
  HSurface* readSurface(Int_t);
  Bool_t readGrids(Int_t,HKickGrid**,Int_t*);
  Int_t createVersion(HParSet*);
  Int_t writePar(HKickPlane2*,Int_t);
  Int_t writePar(HKickMatchPar*,Int_t);
  Int_t writeSurface(HSurface*);
  Int_t writeGrid(HKickGrid*);
  ClassDef(HKickParOraIo,0) // Parameter I/O from Oracle for the kickplane
};

#endif  /* !HKICKPARORAIO_H */
