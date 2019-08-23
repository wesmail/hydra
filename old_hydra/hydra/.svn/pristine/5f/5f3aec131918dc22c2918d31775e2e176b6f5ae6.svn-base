#ifndef HRPCPARORAIO_H
#define HRPCPARORAIO_H

#include "hdetparoraio.h"
#include "TArrayI.h"
#include "TString.h"

class HOraConn;
class HRpcDetector;
class HRpcCalPar;
class HRpcGeomPar;

class HRpcParOraIo : public HDetParOraIo {
private:
  Int_t numSectors;             // number of modules(=sectors) found in Oracle
  Int_t runStart;               // start time of current run
  TArrayI* secIds;              // id-numbers of all sectors with RPC module
  TArrayI* initModules;         // array of modules initialized from Oracle
  HGeomOraDetVersion* geomVers; // geometry version
public:
  HRpcParOraIo(HOraConn* p=0);
  ~HRpcParOraIo(void);
  Bool_t init(HParSet*,Int_t*);
  Int_t write(HParSet*);
  void printIds(void);
private:
  Int_t startIo(HParSet*);
  Int_t readIds(void);
  Int_t getVersion(HParSet*,Int_t*,Int_t*);
  Bool_t read(HRpcCalPar*,Int_t*);
  Bool_t read(HRpcGeomPar*,Int_t*);
  Bool_t readModGeomNames(HRpcGeomPar*,Int_t*);
  Bool_t readCompGeomNames(HRpcGeomPar*,Int_t*);
  Int_t createVers(HParSet*);
  Int_t writePar(HRpcCalPar*);
  Int_t countCells(void);
  void printInfo(const char*);
  ClassDef(HRpcParOraIo,0) // RPC parameter I/O from Oracle
};

#endif  /* !HRPCPARORAIO_H */
