#ifndef HPARORAIO_H
#define HPARORAIO_H

#include <string.h>
#include "hpario.h"
#include "horaconn.h"

class HOraInfo;

class HParOraIo : public HParIo {
private:
  HOraConn* pConn;    // pointer to the connection class
  HOraInfo* pInfo;    // pointer to the utility class
  Bool_t isConnected; // ==kTRUE if connection is open
public:
  HParOraIo();
  ~HParOraIo();
  Bool_t open();
  Bool_t open(char *userName);
  Bool_t open(char *dbName, char *userName);
  void close();
  Bool_t check() {return isConnected;}  // kTRUE if connection is open
  void print();
  HOraInfo* getOraInfo() {return pInfo;}
private:
  Bool_t activateDetIo();
  ClassDef(HParOraIo,0) // Class for parameter I/O from Oracle
};

#endif  /* !HPARORAIO_H */

