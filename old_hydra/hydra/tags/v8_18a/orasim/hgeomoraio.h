#ifndef HGEOMORAIO_H
#define HGEOMORAIO_H

#include "hgeomio.h"
#include "hgeomtransform.h"
#include "TString.h"
#include "TObjArray.h"
#include "TList.h"

class HGeomOraConn;
class HGeomMedia;
class HGeomMedium;
class HGeomSet;
class HGeomNode;
class HGeomHit;
class HGeomInterface;

class HGeomOraIo : public HGeomIo {
private:
  HGeomOraConn* pConn; // pointer to the connection class
  TList* detVersions;  // list of detector versions
  Double_t maxSince;   // lower limit for actual time range of versions
  Double_t minUntil;   // upper limit for actual time range of versions
  class HOraObj : public TObject {  // Container utility class
    public:
      TObject* pObj;
      Int_t    oraId;
      HOraObj(TObject* p=0,Int_t i=-1) {
        pObj=p;
        oraId=i;
      }
      ~HOraObj() {}
  };
  class HOraDetVers : public TNamed { // Container class for detector version 
    public:
      Int_t detectorId;       // Id of detector
      Int_t geomVersion;      // geometry version
      HOraDetVers(const Char_t* pName=0) {
        SetName(pName);
        detectorId=-1;
        geomVersion=-1;
      }
      ~HOraDetVers() {}
  };
  class HOraTransObj : public TNamed {  // Container class for transformation
    public:
      TObject*       pObj;
      Int_t          oraId;
      TString        refObj;
      HGeomTransform refTransform;
      HOraTransObj(TObject* p=0,Int_t i=-1) {
        if (p) {
          SetName(p->GetName());
          pObj=p;
        }
        oraId=i;
      }
      ~HOraTransObj() {}
  };
  class HGeomOraCopyNode : public TNamed {
    public:
      HGeomNode* pNode;
      HGeomOraCopyNode(const Char_t* name,HGeomNode* node) {
        SetName(name);
        pNode=node;
      }
      ~HGeomOraCopyNode() {}
  };
public:
  HGeomOraIo();
  ~HGeomOraIo();
  Bool_t open();
  Bool_t open(const Char_t*,Text_t* status="in");
  void close();
  void print();
  Bool_t isOpen();
  Bool_t isWritable();
  Bool_t setSimulRefRun(const Char_t*);
  Bool_t setRunId(Int_t);
  Bool_t setHistoryDate(const Char_t*);
  const Char_t* getSimulRefRun();
  Int_t getCurrentRunId();
  const Char_t* getHistoryDate();
  Bool_t read(HGeomMedia*);
  Bool_t read(HGeomSet*,HGeomMedia*);
  Bool_t read(HGeomHit*);
  Bool_t write(HGeomMedia*);
  Bool_t write(HGeomSet*);
  Bool_t write(HGeomHit*);
  Bool_t readGeomConfig(HGeomInterface*);
  Int_t checkRunidExistence(Int_t);
private:
  Int_t readGeomSetup();
  Bool_t readMaterialComposition(HGeomMedia*,TObjArray*,Int_t);
  Bool_t readOpticalProperties(HGeomMedia*,TObjArray*,Int_t);
  Bool_t readTarget(HGeomSet*,HGeomMedia*,Int_t,Int_t);
  Bool_t readVolumes(HGeomSet*,HGeomMedia*,TObjArray*,Int_t);
  Bool_t readPoints(TObjArray*,Int_t); 
  Bool_t readTransform(TList*);
  Int_t createVersion(const Char_t*,TString&,TString&);
  Int_t createHitVersion(const Char_t* part,const Char_t* hitSet);
  Bool_t insertMaterialData(Int_t,HGeomMedium*);
  Bool_t insertOpticalData(Int_t,HGeomMedium*);
  Bool_t insertVolumePoints(Int_t,HGeomNode*);
  const Char_t* calcRefTransform(HGeomNode*,HGeomTransform&);
  ClassDef(HGeomOraIo,0) // Class for geometry I/O from Oracle
};

#endif  /* !HGEOMORAIO_H */
