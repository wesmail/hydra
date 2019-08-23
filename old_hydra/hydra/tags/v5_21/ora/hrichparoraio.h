#ifndef HRICHPARORAIO_H
#define HRICHPARORAIO_H

#include "hdetparoraio.h"
#include "TArrayI.h"
#include "TArrayF.h"
#include "TString.h"

class HOraConn;
class HRichDetector;
class HRichCalPar;
class HRichAnalysisPar;
class HRichDigitisationPar;
class HRichGeometryPar;

class HRichParOraIo : public HDetParOraIo {
private:
  Int_t maxModules;        // maximum number of modules in a sector
  TArrayI* richIds;        // id-numbers of all Rich sectors
  Int_t pVersId;           // version for the positioning of the modules
  TArrayI* initModules;    // array of modules initialized from Oracle
  Double_t* calparDate;    // actual date range of RichCalPar
  Double_t* anaparDate;    // actual date range of RichAnaPar
  Double_t* digiparDate;   // actual date range of RichDigiPar
  TArrayI* calparVersions; // actual module versions of RichCalPar
  Int_t runStart;          // start time of actual run
  TString author;          // creator of parameters
  TString description;     // description of parameters
  TString runsUsed;        // runs used to define parameters
public:
  HRichParOraIo(HOraConn* p=0);
  ~HRichParOraIo();
  Bool_t init(HParSet*,Int_t*);
  Bool_t read(HRichCalPar*,Int_t*);
  Bool_t read(HRichAnalysisPar*,Int_t*);
  Bool_t read(HRichDigitisationPar*,Int_t*);
  Bool_t read(HRichGeometryPar*,Int_t*);
  Int_t write(HParSet*);
  Bool_t write(HRichDetector* p) {return kTRUE;}
  Int_t writePar(HRichCalPar*);
  Int_t writePar(HRichAnalysisPar*);
  Int_t writePar(HRichDigitisationPar*);
  Int_t writePar(HRichGeometryPar*);
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
  Int_t getAnaparVersion(Int_t runId);
  Int_t getDigiparVersion(Int_t run_Id);
  void clearVersDate(Double_t*);
  Int_t createCalparVers();
  Int_t createAnaparVers();
  Int_t createDigiparVers();
  Int_t getMaskVersion(TArrayI*,Int_t);
  Int_t getPatternVersion(TArrayI*,Int_t);
  Int_t getQeffVers(TArrayF*,Int_t);
  Int_t getPhLgVers(TArrayF*,Int_t);
  Int_t createMaskVersion(Int_t);
  Int_t createPatternVersion(Int_t);
  Int_t createQeffVers(Int_t);
  Int_t createPhLgVers(Int_t);
  Bool_t writeMaskMatrix(TArrayI*,Int_t,Int_t);
  Bool_t writePatternMatrix(TArrayI*,Int_t,Int_t);
  Bool_t writeQeffVec(TArrayF*,Int_t,Int_t);
  Bool_t writePhLgVec(TArrayF*,Int_t,Int_t);
  Bool_t compareMatrix(TArrayI*,TArrayI*);
  Bool_t compareVector(TArrayF*,TArrayF*);
  Bool_t readMaskMatrix(TArrayI*,Int_t,Int_t);
  Bool_t readPatternMatrix(TArrayI*,Int_t,Int_t);
  Bool_t readQeffVec(TArrayF*,Int_t,Int_t);
  Bool_t readPhLgVec(TArrayF*,Int_t,Int_t);
  Int_t countConnectedPads(Int_t);
  void resetComments(void);
  
  ClassDef(HRichParOraIo,0) // RICH parameter I/O from Oracle
};

#endif  /* !HRICHPARORAIO_H */
