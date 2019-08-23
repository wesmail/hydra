#ifndef HTOFPARORAIO_H
#define HTOFPARORAIO_H

#include "hdetparoraio.h"
#include "TArrayI.h"
#include "TString.h"

class HOraConn;
class HTofDetector;
class HTofLookup;
class HTofCalPar;
class HTofGeomPar;

class HTofParOraIo : public HDetParOraIo {
private:
  Int_t maxModules;      // maximum number of modules in a sector
  TArrayI* tofIds;       // id-numbers of all Tof's (access by sector and module)
  TArrayI* secIds;       // id-numbers of all sectors with at least one Tof module
  Int_t pVersId;         // version for the positioning of the modules
  TArrayI* initModules;  // array of modules initialized from Oracle
  Double_t* calparDate;  // actual date range of TofCalPar
  Double_t* lookupDate;  // actual date range of TofLookup
  Int_t runStart;        // start time of actual run
  TString author;        // creator of parameters
  TString description;   // description of parameters
  TString runsUsed;      // runs used to define parameters
public:
  HTofParOraIo(HOraConn* p=0);
  ~HTofParOraIo();
  Bool_t init(HParSet*,Int_t*);
  Bool_t read(HTofLookup*,Int_t*);
  Bool_t read(HTofCalPar*,Int_t*);
  Bool_t read(HTofGeomPar*,Int_t*);
  Int_t write(HParSet*);
  Bool_t write(HTofDetector* p) {return kTRUE;}
  Int_t writePar(HTofCalPar*);
  Int_t startIo(void);
  Int_t writeTimeCalPar(HTofCalPar*);
  Int_t writeAmpCalPar(HTofCalPar*);
  const char* getAuthor() {return author.Data();}
  const char* getDescription() {return description.Data();}
  const char* getRunsUsed() {return runsUsed.Data();}
  void setAuthor(const char* s) {author=s;}
  void setDescription(const char* s) {description=s;}
  void setRunsUsed(const char* s) {runsUsed=s;}
  void printIds();
  void printInfo(Text_t*);
private:
  Int_t readIds();
  Int_t getLookupVersion(Int_t runId);
  Int_t getCalparVersion(Int_t runId);
  Bool_t readModGeomNames(HTofGeomPar*,Int_t*);
  Bool_t readCompGeomNames(HTofGeomPar*,Int_t*);
  void clearVersDate(Double_t*);
  Int_t createCalVers(const Text_t*);
  Int_t countRods(Int_t);
  void resetComments(void);
  ClassDef(HTofParOraIo,0) // TOF parameter I/O from Oracle
};

#endif  /* !HTOFPARORAIO_H */
