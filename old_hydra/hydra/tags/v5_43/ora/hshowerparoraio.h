#ifndef HSHOWERPARORAIO_H
#define HSHOWERPARORAIO_H

#include "hdetparoraio.h"
#include "TArrayI.h"
#include "TString.h"

class HOraConn;
class HShowerDetector;
class HShowerCalPar;
class HShowerAnalysisPar;

class HShowerParOraIo : public HDetParOraIo {
private:
  TArrayI* showerIds;      // id-numbers of all Shower modules
  Int_t pVersId;           // version for the positioning of the modules
  TArrayI* initModules;    // array of modules initialized from Oracle
  Double_t* calparDate;    // actual date range of ShowerCalPar
  TArrayI* calparVersions; // actual module versions of ShowerCalPar
  Int_t runStart;          // start time of actual run
  TString author;          // creator of parameters
  TString description;     // description of parameters
  TString runsUsed;        // runs used to define parameters
public:
  HShowerParOraIo(HOraConn* p=0);
  ~HShowerParOraIo();
  Bool_t init(HParSet*,Int_t*);
  Bool_t read(HShowerCalPar*,Int_t*);
  Int_t write(HParSet*);
  Bool_t write(HShowerDetector* p) {return kTRUE;}
  Int_t writePar(HShowerCalPar*);
  Int_t startIo(void);
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
  Int_t getCalparVersion(Int_t runId);
  Int_t createCalparVers();
  void clearVersDate(Double_t*);
  void resetComments(void);
  ClassDef(HShowerParOraIo,0) // SHOWER parameter I/O from Oracle
};

#endif  /* !HSHOWERPARORAIO_H */
