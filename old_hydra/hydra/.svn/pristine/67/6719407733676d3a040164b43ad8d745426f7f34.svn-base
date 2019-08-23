#ifndef HORAMDC_H
#define HORAMDC_H

#include "hdetpario.h"
#include "TArrayI.h"

class HOraConn;
class HMdcDetector;
class HMdcRawStruct;
class HMdcGeomStruct;
class HMdcCalPar;
class HMdcLookupGeom;

class HOraMdc : public HDetParIo {
protected:
  HOraConn *pConn;  // pointer to Oracle connection class
  Int_t id[6][4];     // id-numbers of all Mdc's
  Bool_t isActiv;   // flag is kTRUE if the id numbers have been read
  Int_t pVersId;    // id for positions of the modules
  TArrayI* initModules;  // array of modules initialized from Oracle
public: 
  HOraMdc(HOraConn* p=0);
  ~HOraMdc();

  Bool_t init(HParSet*,Int_t*);
  Bool_t read(HMdcRawStruct*,Int_t*);
  Bool_t read(HMdcGeomStruct*,Int_t*);
  Bool_t read(HMdcCalPar*,Int_t*);
  Bool_t read(HMdcLookupGeom*,Int_t*);

  Int_t write(HParSet*);
  Bool_t write(HMdcDetector* p);
  Int_t update(HMdcCalPar*);

  void printIds();
  void printInfo(Text_t*);

  void resetActivFlag(Bool_t f=kFALSE) {isActiv=f;}
protected:
  Int_t readIds();
  Int_t getCalparVersion(Long_t runId);
  Int_t getLookupVersion(Long_t runId);
  Bool_t execProcRuns(Long_t runId);
  ClassDef(HOraMdc,0) // MDC parameter I/O from Oracle

};

#endif  /* !HORAMDC_H */

