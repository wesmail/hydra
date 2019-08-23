#ifndef HGEOMORACONN_H
#define HGEOMORACONN_H

#include <string.h>
#include "TObject.h"
#include "TString.h"

class HGeomOraConn : public TObject {
protected:
  char dbName[31];        // database name (may be a remote database)
  char userName[31];      // name of user
  Bool_t isConnected;     // ==kTRUE, if connection is open
  Bool_t writable;        // ==kTRUE, if writable
  Int_t actRunId;         // actual runId (can be -1 if there are no data in Oracle)
  Int_t runStart;         // start time of actual run
  char expLocation[31];   // Beam or simulation run
  char historyDate[21];   // timestamp to retrieve historic data
  TString simulRefRun;    // name of simulation reference run 
  Bool_t needsServerCheck; // kTRUE before first call to getRunStart
public:
  HGeomOraConn();
  ~HGeomOraConn();
  Bool_t open();
  Bool_t open(char*);
  Bool_t open(char*, char*);
  Bool_t isOpen() {return isConnected;}
  Bool_t isWritable() {return writable;}
  void close();
  void print();
  void showSqlError(char*,char* msg="");
  void showSqlError(char*,const char* msg);
  void commit();
  void rollback();
  Int_t getActRunId(void) { return actRunId; }
  Int_t getRunStart(Int_t);
  Int_t getActRunStart() { return runStart; }
  const char* getSimulRefRun() {return simulRefRun.Data();}
  const char* getHistoryDate() { return historyDate; }
  const char* getExpLocation() { return expLocation; }
  Bool_t setHistoryDate(const char*);
  Bool_t setParamRelease(const char*);
  Bool_t setParamRelease(Int_t);
  Bool_t setSimulRefRun(const char*);
  Bool_t setRunId(Int_t id);
private:
  Bool_t openConnection(char*);
  char* getPassword();
  Int_t checkServerLoad();
  ClassDef(HGeomOraConn,0) // Connection class to Oracle for geometry
};

#endif  /* !HGEOMORACONN_H */
