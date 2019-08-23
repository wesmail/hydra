#ifndef HORACONN_H
#define HORACONN_H

#include <string.h>
#include "TObject.h"

class HOraConn : public TObject {
protected:
  void *henv;         // ODBC environment pointer
  void *hdbc;         // ODBC database pointer
  void *hstmt;        // ODBC statement pointer
  char dbName[21];    // database name (may be a remote database)
  char userName[21];  // name of user
  Bool_t isConnected; // ==kTrue if connection is open
public:
  HOraConn();
  ~HOraConn();
  Bool_t open();
  Bool_t open(char *userName);
  Bool_t open(char *dbName, char *userName);
  void close();
  Bool_t check() {return isConnected;}  // kTRUE if connection is open
  void print();
  void showErrorMessage(char* errorloc);
  Long_t getRunId();
  void* getHenv() {return henv;}
  void* getHdbc() {return hdbc;}
  void* getHstmt() {return hstmt;}
private:
  Bool_t openConnection(char*);
  char* getPassword();

  ClassDef(HOraConn,0) // Connection class to Oracle
};

#endif  /* !HORACONN_H */

