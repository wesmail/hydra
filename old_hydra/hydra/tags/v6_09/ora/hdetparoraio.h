#ifndef HDETPARORAIO_H
#define HDETPARORAIO_H

#include "hdetpario.h"
#include "hgeomtransform.h"

class HOraConn;
class HRun;
class HDetGeomPar;
class HSpecGeomPar;
class HGeomVolume;
class HParCond;

class HParOraSet : public TNamed {
public:
  Int_t contextId;         // Id of context
  Double_t versDate[2];    // Actual date range of parameters
  HParOraSet(const char* pName);
  ~HParOraSet() {}
  void clearVersDate();      
  ClassDef(HParOraSet,0)
};

class HOraGeomObj : public TNamed {
public:
  TObject* pObj;            // pointer to object of type HModGeomPar or HGeomVolume
  Char_t   volType;         // M: Module, R: Reference module, C: inner Component
  Int_t    objType;         // type of the geometry object in Oracle
  TObject* refObj;          // pointer to the mother volume of a component
  Int_t    objId;           // id of the geometry volume in Oracle
  HOraGeomObj(const char* pName="", TObject* p=0, Char_t vType='M',
              TObject* r=0, Int_t pObjType=1) {
    SetName(pName);
    pObj=p;
    volType=vType;
    refObj=r;
    objType=pObjType;
    objId=-1;
  }
  ~HOraGeomObj() {}
  ClassDef(HOraGeomObj,0)
};

class HDetParOraIo : public HDetParIo {
private:
  HOraConn* pConn;       // pointer to Oracle connection class
  HRun* actContVers;     // pointer to the actual list of container versions
  Int_t actRunId;        // actual runId (can be -1 if there are no data in Oracle)
  TList* containerList;  // list of parameter containers
public:
  HDetParOraIo(HOraConn* p=0);
  virtual ~HDetParOraIo(void);
  void commit(void);
  void rollback(void);
  void showSqlError(const char*);
  Int_t getActRunId(void) { return actRunId; }
  Int_t getRunStart(HParSet* pPar=0);
  const char* getExpLocation();
  Int_t getPredefVersion(HParSet*);
  const char* getHistoryDate();
  Bool_t readCond(HParCond*,Int_t*);
  Int_t writeCond(HParCond*);
  Bool_t readGeometry(HDetGeomPar*,Int_t*,Int_t contGeomVers=-1,
                      Bool_t includeAlignment=kTRUE);
  Int_t writeAlignment(HDetGeomPar*);
protected:
  void setChanged(HParSet*);
  Int_t getIdealGeomVersion();
  Int_t readGeomVersion(HDetGeomPar*,Int_t&,Int_t&,Bool_t includeAlignment=kTRUE);
  Bool_t readIdealGeomTransform(TList*,Int_t);
  Int_t readAlignmentTransform(TList*,Int_t);
  Bool_t readGeomPoints(TList*,Int_t);
  HParOraSet* getOraSet(HParSet*);
  Int_t getContextId(const char*,const char*);
  Int_t getAlignmentOutputVersion(HParSet*,const char*);
  Bool_t writeTransform(Int_t,const char*,Int_t,const HGeomTransform&);
private:
  Int_t createParamVers(HParCond*);
  Int_t createAlignmentVers(HParSet*,Int_t);
  ClassDef(HDetParOraIo,0) // base class for the detector interface to Oracle
};

#endif  /* !HDETPARORAIO_H */
