#ifndef HSTARTPARORAIO_H
#define HSTARTPARORAIO_H

#include "hdetparoraio.h"
#include "TArrayI.h"
#include "TString.h"

class HOraConn;
class HStartDetector;
class HStartCalPar;
class HStartSelfCoPar;
class HStartLookup;
class HStartGeomPar;

class HStartParOraIo : public HDetParOraIo {
private:
  Int_t maxModules;        // maximum number of modules
  Int_t maxStrips;         // maximum number of strips
  Int_t numModules;        // number of modules found in Oracle
  Int_t runStart;          // start time of actual run
  TArrayI* startIds;       // number of strips of all Start modules in the actual setup
  TArrayI* initModules;    // array of modules initialized from Oracle
  Int_t geomVersion;        // geometry version for START
public:
  HStartParOraIo(HOraConn* p=0);
  ~HStartParOraIo();
  Bool_t init(HParSet*,Int_t*);
  Int_t write(HParSet*);
  void printNStrips();
private:
  Int_t startIo(HParSet*);
  Int_t readIds();
  Int_t getVersion(HParSet*,Int_t*,Int_t*);
  Bool_t read(HStartLookup*,Int_t*);
  Bool_t read(HStartCalPar*,Int_t*);
  Bool_t read(HStartSelfCoPar*,Int_t*);
  Bool_t read(HStartGeomPar*,Int_t*);
  Int_t readGeomVersion();
  Bool_t readModGeomNames(HStartGeomPar*,Int_t*,Int_t&);
  Bool_t readCompGeomNames(HStartGeomPar*,Int_t*);
  Int_t createVers(HParSet*);
  Int_t writePar(HStartCalPar*);
  Int_t writePar(HStartSelfCoPar*);
  Int_t writePar(HStartLookup*);
  void printInfo(const char*);
  ClassDef(HStartParOraIo,0) // START detector parameter I/O from Oracle
};

#endif  /* !HSTARTPARORAIO_H */
