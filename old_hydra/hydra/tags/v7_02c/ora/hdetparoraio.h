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
class HParamBinObj;
class HGeomOraDetVersion;

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
  TObject* refObj;          // pointer to the mother volume of a component
  Int_t    objId;           // id of the geometry volume in Oracle
  Int_t    volId;           // id of the volume points
  Int_t    transId;         // id of the volume transformation
  HOraGeomObj(const char* pName="", TObject* p=0, Char_t vType='M', TObject* r=0) {
    SetName(pName);
    pObj=p;
    volType=vType;
    refObj=r;
    objId=-1;
    volId=-1;
    transId=-1;
  }
  ~HOraGeomObj() {}
  ClassDef(HOraGeomObj,0)
};

class HDetParOraIo : public HDetParIo {
private:
  HOraConn* pConn;        // pointer to Oracle connection class
  HRun* actContVers;      // pointer to the actual list of container versions
  Int_t actRunId;         // actual runId (can be -1 if there are no data in Oracle)
  TList* containerList;   // list of parameter containers
protected:
  class HParOraBlob : public TObject {
    public:
      HParamBinObj* binaryParam;   // Pointer to object in parameter list
      Int_t         blobId;        // Id of BinaryLargeObject
      HParOraBlob(HParamBinObj* o=0,Int_t id=0) { binaryParam=o; blobId=id; }
      ~HParOraBlob() {}
  };
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
  Bool_t readDetectorGeometry(HDetGeomPar*,Int_t*,HGeomOraDetVersion*);
  Int_t writeAlignment(HDetGeomPar*);
protected:
  void setChanged(HParSet*);
  void addGeomOraSet(HDetGeomPar*);
  Int_t getDetectorId(const char*);
  Int_t getGeomVersion(const char*,HGeomOraDetVersion*);
  Int_t getGeomVersion(Int_t,HGeomOraDetVersion*);
  Bool_t readIdealGeometry(TList*,HGeomOraDetVersion*);
  Bool_t readGeomPoints(TList*);
  Bool_t readGeomTransform(TList*);
  Int_t getAlignmentVersion(HParOraSet*,const char*);
  Int_t readAlignmentTransform(TList*,Int_t);
  HParOraSet* getOraSet(HParSet*);
  Int_t getContextId(const char*,const char*);
  Int_t getAlignmentOutputVersion(HParSet*,const char*);
  Bool_t writeTransform(Int_t,const char*,const HGeomTransform&);
private:
  Bool_t readBlob(HParamBinObj* obj,Int_t lobId);
  Int_t createParamVers(HParCond*);
  Int_t createAlignmentVers(HParSet*,Int_t);
  Int_t storeBlob(UChar_t*, Int_t);
  ClassDef(HDetParOraIo,0) // base class for the detector interface to Oracle
};

#endif  /* !HDETPARORAIO_H */
