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
  Bool_t open(Char_t *userName);
  Bool_t open(Char_t *dbName, Char_t *userName);
  Bool_t reconnect();
  void close();
  void disconnect();
  Bool_t check() {return isConnected;}  // kTRUE if connection is open
  void print();
  void setDetParIo(const Text_t*);
  HOraInfo* getOraInfo() {return pInfo;}
  Bool_t setHistoryDate(const Char_t*);
  Bool_t setParamRelease(const Char_t*);
  TList* getListOfRuns(const Char_t*,const Char_t* startAt="",const Char_t* endAt="");
private:
  Bool_t activateDetIo();
  ClassDef(HParOraIo,0) // Class for parameter I/O from Oracle
};

#endif  /* !HPARORAIO_H */

