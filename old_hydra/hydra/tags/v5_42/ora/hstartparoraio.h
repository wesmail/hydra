#ifndef HSTARTPARORAIO_H
#define HSTARTPARORAIO_H

#include "hdetparoraio.h"
#include "TArrayI.h"
#include "TString.h"

class HOraConn;
class HStartDetector;
class HStartCalPar;
class HStartLookup;
class HStartGeomPar;

class HStartParOraIo : public HDetParOraIo {
private:
  Int_t maxModules;        // maximum number of modules
  Int_t maxStrips;         // maximum number of strips
  TArrayI* startIds;       // number of strips of all Start modules in the actual setup
  Int_t pVersId;           // version for the positioning of the modules
  TArrayI* initModules;    // array of modules initialized from Oracle
  Double_t* calparDate;    // actual date range of StartCalPar
  Double_t* lookupDate;    // actual date range of StartLookup
  TArrayI* calparVersions; // actual module versions of StartCalPar
  Int_t runStart;          // start time of actual run
  TString author;          // creator of parameters
  TString description;     // description of parameters
  TString runsUsed;        // runs used to define parameters
public:
  HStartParOraIo(HOraConn* p=0);
  ~HStartParOraIo();
  Bool_t init(HParSet*,Int_t*);
  Bool_t read(HStartCalPar*,Int_t*);
  Bool_t read(HStartLookup*,Int_t*);
  Bool_t read(HStartGeomPar*,Int_t*);
  Int_t write(HParSet*);
  Bool_t write(HStartDetector* p) {return kTRUE;}
  Int_t writePar(HStartCalPar*);
  Int_t startIo(void);
  const char* getAuthor() {return author.Data();}
  const char* getDescription() {return description.Data();}
  const char* getRunsUsed() {return runsUsed.Data();}
  void setAuthor(const char* s) {author=s;}
  void setDescription(const char* s) {description=s;}
  void setRunsUsed(const char* s) {runsUsed=s;}
  void printNStrips();
  void printInfo(Text_t*);
private:
  Int_t readIds();
  Int_t getCalparVersion(Int_t runId);
  Int_t getLookupVersion(Int_t runId);
  Bool_t readModGeomNames(HStartGeomPar*,Int_t*);
  Bool_t readCompGeomNames(HStartGeomPar*,Int_t*);
  Int_t createCalparVers();
  void clearVersDate(Double_t*);
  void resetComments(void);
  Int_t countStrips(Int_t);
  ClassDef(HStartParOraIo,0) // START detector parameter I/O from Oracle
};

#endif  /* !HSTARTPARORAIO_H */
