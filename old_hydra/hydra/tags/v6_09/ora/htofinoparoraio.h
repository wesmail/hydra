#ifndef HTOFINOPARORAIO_H
#define HTOFINOPARORAIO_H

#include "hdetparoraio.h"
#include "TArrayI.h"
#include "TString.h"

class HOraConn;
class HTofinoDetector;
class HTofinoLookup;
class HTofinoGeomPar;
class HTofinoCalPar;
class HTofinoShowerMap;
class HTofinoParOraIo : public HDetParOraIo {
private:
  Int_t numModules;        // number of modules found in Oracle
  Int_t runStart;        // start time of actual run
  TArrayI* tofinoIds;    // id-numbers of all Tofino's (access by sector and module)
  TArrayI* initModules;  // array of modules initialized from Oracle
public:
  HTofinoParOraIo(HOraConn* p=0);
  ~HTofinoParOraIo();
  Bool_t init(HParSet*,Int_t*);
  Int_t write(HParSet*);
  void printIds();
private:
  Int_t startIo(HParSet*);
  Int_t readIds();
  Int_t getLookupVersion(HTofinoLookup*);
  Int_t getParVersion(HParSet *pPar,Int_t *version);
  Int_t createVers(HParSet *pPar);
  Int_t writePar(HTofinoCalPar*);
  Int_t writePar(HTofinoShowerMap*);
  Bool_t read(HTofinoLookup*,Int_t*);
  Bool_t read(HTofinoGeomPar*,Int_t*);
  Bool_t read(HTofinoCalPar*,Int_t*);
  Bool_t read(HTofinoShowerMap*,Int_t*);
  Bool_t readModGeomNames(HTofinoGeomPar*,Int_t*);
  Bool_t readCompGeomNames(HTofinoGeomPar*,Int_t*);
  void printInfo(const char*);
  ClassDef(HTofinoParOraIo,0) // TOFINO parameter I/O from Oracle
};

#endif  /* !HTOFINOPARORAIO_H */
