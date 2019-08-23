#ifndef HDETPARORAIO_H
#define HDETPARORAIO_H

#include "hdetpario.h"

class HOraConn;
class HRun;
class HDetGeomPar;
class HGeomVolume;
class HGeomTransform;

class HDetParOraIo : public HDetParIo {
private:
  HOraConn* pConn;   // pointer to Oracle connection class
  HRun* actContVers; // pointer to the actual list of container versions
  Int_t actRunId;    // actual runId (can be -1 if there are no data in Oracle)
  Double_t* geompar_date; // stores the actual date range of the geometry
public:
  HDetParOraIo(HOraConn* p=0);
  virtual ~HDetParOraIo(void);
  void commit(void);
  void rollback(void);
  void showSqlError(const char*);
  Int_t getActRunId(void) { return actRunId; }
  Int_t getRunStart();
  Int_t getPredefVersion(HParSet*);
protected:
  Bool_t readGeometry(HDetGeomPar*,Int_t*);
  virtual Int_t readGeomVersion(Int_t);
  Int_t readGeomIndex(const Text_t*);
  Bool_t readGeomShape(HGeomVolume*, Int_t,  Int_t);
  Int_t readGeomPoints(HGeomVolume*, Int_t, Int_t);
  Bool_t readGeomTransform(HGeomTransform*,Text_t*,Int_t, Int_t);
  ClassDef(HDetParOraIo,0) // base class for the detector interface to Oracle
};

#endif  /* !HDETPARORAIO_H */
