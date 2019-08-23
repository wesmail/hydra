#ifndef HRICHPARORAIO_H
#define HRICHPARORAIO_H

#include "hdetparoraio.h"
#include "TArrayI.h"
#include "TArrayF.h"
#include "TString.h"

class HOraConn;
class HRichDetector;
class HParSet;
class HRichCalPar;
class HRichGeometryPar;
class HRichMappingPar;

class HRichParOraIo : public HDetParOraIo {
private:
  Int_t maxModules;        // maximum number of modules in a sector
  TArrayI* richIds;        // id-numbers of all Rich sectors
  Int_t pVersId;           // version for the positioning of the modules
  TArrayI* initModules;    // array of modules initialized from Oracle
  Double_t* calparDate;    // actual date range of RichCalPar
  TArrayI* calparVersions; // actual module versions of RichCalPar
  Int_t runStart;          // start time of actual run
  TString runsUsed;        // runs used to define parameters
public:
  HRichParOraIo(HOraConn* p=0);
  ~HRichParOraIo();
  Bool_t init(HParSet*,Int_t*);
  Bool_t read(HRichCalPar*,Int_t*);
  Bool_t read(HRichGeometryPar*,Int_t*);
  Bool_t read(HRichMappingPar*,Int_t*);
  Int_t write(HParSet*);
  Bool_t write(HRichDetector* p) {return kTRUE;}
  Int_t writePar(HRichCalPar*);
  Int_t writePar(HRichGeometryPar*);
  Int_t writePar(HRichMappingPar*);
  Int_t startIo(void);
  const char* getRunsUsed() {return runsUsed.Data();}
  void setRunsUsed(const char* s) {runsUsed=s;}
  void printIds();
  void printInfo(Text_t*);
private:
  Int_t readIds();  
  void clearVersDate(Double_t*);
  void resetComments(void);
  Int_t getCalparVersion(Int_t runId);
  Int_t createCalparVers(HRichCalPar*);
  ClassDef(HRichParOraIo,0) // RICH parameter I/O from Oracle
};

#endif  /* !HRICHPARORAIO_H */
