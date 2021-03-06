#ifndef HORADETRUNINFOIO_H
#define HORADETRUNINFOIO_H

using namespace std;
#include "TObject.h"
#include "TString.h"

class HDbConn;

class HOraDetRunInfoIo : public TObject {
private:
  HDbConn* pConn;     // pointer to connection class
  Bool_t hasErrors;   // kTRUE, if current application returned errors
  TString peopleId;   // Identifier of a HADES member
  Int_t setupVersion; // version of the detector setup
  Int_t numChanges;   // number of changes since last commit/rollback
public:
  HOraDetRunInfoIo();
  ~HOraDetRunInfoIo();
  Bool_t open(const char*,const char* dbName="db-hades");
  void close();
  void print();
  Bool_t isOpen();
  Int_t getSetupVersion() {return setupVersion;}
  Int_t getNumChanges() {return numChanges;}
  const char* getPeopleId(const char*,const char*);
  Bool_t setDetectorSetupVersion(const char*,Int_t);
  Int_t createComment(const char*,const char*);
  Bool_t setRunQuality(const char*,Int_t,Int_t);
  void finish();
  void rollback();  
private:
  void commit();
  ClassDef(HOraDetRunInfoIo,0) // Interface to the analysis run info in Oracle
};

#endif  /* !HORADETRUNINFOIO */
