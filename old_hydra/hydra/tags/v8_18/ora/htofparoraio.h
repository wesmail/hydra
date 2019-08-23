#ifndef HTOFPARORAIO_H
#define HTOFPARORAIO_H

#include "hdetparoraio.h"
#include "TArrayI.h"
#include "TString.h"

class HOraConn;
class HTofDetector;
class HTofLookup;
class HTofCalPar;
class HTofDigiPar;
class HTofGeomPar;

class HTofParOraIo : public HDetParOraIo {
private:
  Int_t maxModules;             // maximum number of modules in a sector
  Int_t numModules;             // number of modules found in Oracle
  Int_t runStart;               // start time of current run
  TArrayI* tofIds;              // id-numbers of all Tof's (access by sector and module)
  TArrayI* secIds;              // id-numbers of all sectors with at least one Tof module
  TArrayI* initModules;         // array of modules initialized from Oracle
  HGeomOraDetVersion* geomVers; // geometry version
public:
  HTofParOraIo(HOraConn* p=0);
  ~HTofParOraIo();
  Bool_t init(HParSet*,Int_t*);
  Int_t write(HParSet*);
  void printIds();
private:
  Int_t startIo(HParSet*);
  Int_t readIds();
  Int_t getVersion(HParSet*,Int_t*,Int_t*);
  Int_t getLookupVersion(HTofLookup*);
  Bool_t read(HTofLookup*,Int_t*);
  Bool_t read(HTofCalPar*,Int_t*);
  Bool_t read(HTofDigiPar*,Int_t*);
  Bool_t read(HTofGeomPar*,Int_t*);
  Bool_t readModGeomNames(HTofGeomPar*,Int_t*);
  Bool_t readCompGeomNames(HTofGeomPar*,Int_t*);
  Int_t createVers(HParSet*);
  Int_t writePar(HTofCalPar*);
  Int_t writePar(HTofDigiPar*);
  Int_t countRods(Int_t);
  void putTofSectors(Int_t*,Int_t*);
  void printInfo(const Char_t*);
  ClassDef(HTofParOraIo,0) // TOF parameter I/O from Oracle
};

#endif  /* !HTOFPARORAIO_H */
