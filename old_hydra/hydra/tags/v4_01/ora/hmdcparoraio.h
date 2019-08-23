#ifndef HMDCPARORAIO_H
#define HMDCPARORAIO_H

#include "hdetpario.h"
#include "TArrayI.h"

class HOraConn;
class HMdcDetector;
class HMdcRawStruct;
class HMdcGeomStruct;
class HMdcMboReadout;
class HMdcCalPar;
class HMdcLookupGeom;
class HMdcDigitPar;
class HMdcLayerGeomPar;
class HMdcGeomPar;
class HGeomVolume;
class HGeomTransform;

class HMdcParOraIo : public HDetParIo {
private:
  HOraConn *pConn;       // pointer to Oracle connection class
  Int_t id[6][4];        // id-numbers of all Mdc's
  Bool_t isActiv;        // flag is kTRUE if the id numbers have been read
  Long_t viewsRunId;     // RunId for which the views were created
  Int_t pVersId;         // version for positions of the modules
  TArrayI* initModules;  // array of modules initialized from Oracle
public: 
  HMdcParOraIo(HOraConn* p=0);
  ~HMdcParOraIo();

  Bool_t init(HParSet*,Int_t*);
  Bool_t read(HMdcRawStruct*,Int_t*);
  Bool_t read(HMdcGeomStruct*,Int_t*);
  Bool_t read(HMdcMboReadout*,Int_t*);
  Bool_t read(HMdcCalPar*,Int_t*);
  Bool_t read(HMdcLookupGeom*,Int_t*);
  Bool_t read(HMdcDigitPar*,Int_t*);
  Bool_t read(HMdcLayerGeomPar*,Int_t*);
  Bool_t read(HMdcGeomPar*,Int_t*);

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
  Int_t getDigiVersion(Long_t runId);
  Int_t getReadoutVersion(Long_t runId);
  Int_t getGeomVersion(Long_t runId);
  Bool_t execProcRuns(Long_t runId);
  Long_t readNumber(char* statement, Long_t in1);
  Bool_t readModGeomNames(HMdcGeomPar*,Int_t*);
  Bool_t readLayerGeomNames(HMdcGeomPar*,Int_t*);
  Long_t readGeomIndex(const Text_t*);
  Bool_t readGeomShape(HGeomVolume*, Long_t,  Long_t);
  Int_t readGeomPoints(HGeomVolume*, Long_t, Long_t);
  Bool_t readGeomTransform(HGeomTransform*,Text_t*,Long_t, Long_t);

  ClassDef(HMdcParOraIo,0) // MDC parameter I/O from Oracle
};

#endif  /* !HMDCPARORAIO_H */

