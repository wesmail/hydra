#ifndef HORACONN_H
#define HORACONN_H

#include <string.h>
#include "TObject.h"

class HOraConn : public TObject {
protected:
  char dbName[21];        // database name (may be a remote database)
  char userName[21];      // name of user
  Bool_t isConnected;     // ==kTRUE if connection is open
  Int_t actRunId;         // actual runId (can be -1 if there are no data in Oracle)
  Int_t runStart;         // start time of actual run
  char expLocation[11];   // Beam or simulation run
  char historyDate[21];   // timestamp to retrieve historic data
  Int_t geomVersion;      // maximum version for ideal geometry
  Int_t alignmentOutVers; // current output version for alignment
public:
  HOraConn();
  ~HOraConn();
  Bool_t open();
  Bool_t open(char*);
  Bool_t open(char*, char*);
  void close();
  void print();
  void showSqlError(char*);
  Int_t getActRunId(void) { return actRunId; }
  Int_t getRunStart(Int_t);
  const char* getExpLocation() { return expLocation; }
  Bool_t setHistoryDate(const char*);
  const char* getHistoryDate() { return historyDate; }
  Bool_t setParamRelease(const char*);
  Bool_t setParamRelease(Int_t);
  Int_t getIdealGeomVersion();
  Int_t getAlignmentOutputVersion() { return alignmentOutVers; }
  void setAlignmentOutputVersion(Int_t v) { alignmentOutVers=v; }
private:
  Bool_t openConnection(char*);
  char* getPassword();

  ClassDef(HOraConn,0) // Connection class to Oracle
};

#endif  /* !HORACONN_H */
