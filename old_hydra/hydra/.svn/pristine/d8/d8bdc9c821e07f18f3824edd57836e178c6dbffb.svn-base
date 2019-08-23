#ifndef HORAIO_H
#define HORAIO_H

#include <string.h>
#include "hpario.h"
#include "horaconn.h"

class HOraIo : public HParIo {
protected:
  HOraConn* pConn;  // pointer to connection class
public:
  HOraIo();
  ~HOraIo();
  Bool_t open();
  Bool_t open(char *userName);
  Bool_t open(char *dbName, char *userName);
  void close();
  Bool_t check() {return pConn->check();}  // kTRUE if connection is open
  void print();
  void setDetParIo(Text_t* detIo);

  ClassDef(HOraIo,0) // Class for parameter I/O from Oracle
};

#endif  /* !HORAIO_H */

