#ifndef HMDCPARORAIO_H
#define HMDCPARORAIO_H

#include "hdetparoraio.h"
#include "TArrayI.h"
#include "TString.h"

class HOraConn;
class HMdcDetector;
class HMdcRawStruct;
class HMdcGeomStruct;
class HMdcMboReadout;
class HMdcCalParRaw;
class HMdcLookupGeom;
class HMdcDigitPar;
class HMdcLayerGeomPar;
class HMdcGeomPar;

class HMdcParOraIo : public HDetParOraIo {
private:
  TArrayI* mdcIds;       // id-numbers of all Mdc's (access by sector and plane)
  TArrayI* modIds;       // id-numbers of all Mdc modules (access by id)
  Int_t pVersId;         // version for positions of the modules
  TArrayI* initModules;  // array of modules initialized from Oracle
  Double_t* lookupgeomDate; // actual date range of MdcLookupGeom
  Double_t* mboreadoutDate; // actual date range of MdcMboReadout
  Double_t* digiparDate;    // actual date range of MdcDigiPar
  Double_t* cal1parDate;    // actual date range of MdcCalParRaw
  TArrayI* cal1parVersions; // actual module versions of MdcCalParRaw
  Int_t runStart;           // start time of actual run
  TString author;           // creator of parameters
  TString description;      // description of parameters
  TString runsUsed;         // runs used to define parameters
public:
  HMdcParOraIo(HOraConn* p=0);
  ~HMdcParOraIo();

  Bool_t init(HParSet*,Int_t*);
  Bool_t read(HMdcRawStruct*,Int_t*);
  Bool_t read(HMdcGeomStruct*,Int_t*);
  Bool_t read(HMdcMboReadout*,Int_t*);
  Bool_t read(HMdcCalParRaw*,Int_t*);
  Bool_t read(HMdcLookupGeom*,Int_t*);
  Bool_t read(HMdcDigitPar*,Int_t*);
  Bool_t read(HMdcLayerGeomPar*,Int_t*);
  Bool_t read(HMdcGeomPar*,Int_t*);

  Int_t write(HParSet*);
  Bool_t write(HMdcDetector* p) {return kTRUE;}
  Int_t writePar(HMdcCalParRaw*);
  const char* getAuthor(){return author.Data();}
  const char* getDescription(){return description.Data();}
  const char* getRunsUsed(){return runsUsed.Data();}
  void setAuthor(const char* s) {author=s;}
  void setDescription(const char* s) {description=s;}
  void setRunsUsed(const char* s) {runsUsed=s;}

  void printIds();
  void printInfo(Text_t*);

private:
  Int_t readIds();
  Int_t getCal1parVersion(Int_t runId);
  Int_t getLookupVersion(Int_t runId);
  Int_t getReadoutVersion(Int_t runId);
  Int_t getDigiparVersion(Int_t);
  Bool_t readModGeomNames(HMdcGeomPar*,Int_t*);
  Bool_t readLayerGeomNames(HMdcGeomPar*,Int_t*);
  void clearVersDate(Double_t*);
  Int_t createCal1parVers(HMdcCalParRaw*);
  Int_t countTdcChannels(Int_t);
  void resetComments(void);
  ClassDef(HMdcParOraIo,0) // MDC parameter I/O from Oracle
};

#endif  /* !HMDCPARORAIO_H */
