#ifndef HTOFINOPARORAIO_H
#define HTOFINOPARORAIO_H

#include "hdetparoraio.h"
#include "TArrayI.h"
#include "TString.h"

class HOraConn;
class HTofinoDetector;
class HTofinoLookup;
class HTofinoGeomPar;

class HTofinoParOraIo : public HDetParOraIo {
private:
  TArrayI* tofinoIds;    // id-numbers of all Tofino's (access by sector and module)
  Int_t pVersId;         // version for the positioning of the modules
  TArrayI* initModules;  // array of modules initialized from Oracle
  Double_t* lookupDate;  // actual date range of TofinoLookup
  Int_t runStart;        // start time of actual run
public:
  HTofinoParOraIo(HOraConn* p=0);
  ~HTofinoParOraIo();
  Bool_t init(HParSet*,Int_t*);
  Bool_t read(HTofinoLookup*,Int_t*);
  Bool_t read(HTofinoGeomPar*,Int_t*);
  Int_t write(HParSet*);
  Bool_t write(HTofinoDetector* p) {return kTRUE;}
  Int_t startIo(void);
  void printIds();
  void printInfo(Text_t*);
private:
  Int_t readIds();
  Int_t getLookupVersion(Int_t runId);
  Bool_t readModGeomNames(HTofinoGeomPar*,Int_t*);
  Bool_t readCompGeomNames(HTofinoGeomPar*,Int_t*);
  void clearVersDate(Double_t*);
  ClassDef(HTofinoParOraIo,0) // TOFINO parameter I/O from Oracle
};

#endif  /* !HTOFINOPARORAIO_H */
