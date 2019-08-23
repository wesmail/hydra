#ifndef HORASIMUTIL_H
#define HORASIMUTIL_H

#include "TObject.h"

class HDbConn;

class HOraSimUtil : public TObject {
private:
  HDbConn* pConn;               // pointer to the connection class
public:
  HOraSimUtil(HDbConn* p=0) { pConn=p; }
  ~HOraSimUtil() {}
  Bool_t isOpen();
  void commit();
  void rollback();
  void showSqlError(const Char_t*);
private:
  ClassDef(HOraSimUtil,0) // Utility class for storage of simulation runs in Oracle
};

#endif  /* !HORASIMUTIL_H */
