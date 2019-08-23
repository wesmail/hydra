#ifndef HGEOMORACONN_H
#define HGEOMORACONN_H

#include <string.h>
#include "TObject.h"
#include "TString.h"

class HGeomOraConn : public TObject {
protected:
  Char_t dbName[31];        // database name (may be a remote database)
  Char_t userName[31];      // name of user
  Bool_t isConnected;     // ==kTRUE, if connection is open
  Bool_t writable;        // ==kTRUE, if writable
  Int_t actRunId;         // actual runId (can be -1 if there are no data in Oracle)
  Int_t runStart;         // start time of actual run
  Char_t expLocation[31];   // Beam or simulation run
  Char_t historyDate[21];   // timestamp to retrieve historic data
  TString simulRefRun;    // name of simulation reference run 
  Bool_t needsServerCheck; // kTRUE before first call to getRunStart
public:
  HGeomOraConn();
  ~HGeomOraConn();
  Bool_t open();
  Bool_t open(Char_t*);
  Bool_t open(Char_t*, Char_t*);
  Bool_t isOpen() {return isConnected;}
  Bool_t isWritable() {return writable;}
  void close();
  void print();
  void showSqlError(Char_t*,Char_t* msg="");
  void showSqlError(Char_t*,const Char_t* msg);
  void commit();
  void rollback();
  Int_t getActRunId(void) { return actRunId; }
  Int_t getRunStart(Int_t);
  Int_t getActRunStart() { return runStart; }
  const Char_t* getSimulRefRun() {return simulRefRun.Data();}
  const Char_t* getHistoryDate() { return historyDate; }
  const Char_t* getExpLocation() { return expLocation; }
  Bool_t setHistoryDate(const Char_t*);
  Bool_t setParamRelease(const Char_t*);
  Bool_t setParamRelease(Int_t);
  Bool_t setSimulRefRun(const Char_t*);
  Bool_t setRunId(Int_t id);
private:
  Bool_t openConnection(Char_t*);
  Char_t* getPassword();
  Int_t checkServerLoad();
  ClassDef(HGeomOraConn,0) // Connection class to Oracle for geometry
};

#endif  /* !HGEOMORACONN_H */
