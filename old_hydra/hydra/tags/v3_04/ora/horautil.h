#ifndef HORAUTIL_H
#define HORAUTIL_H

#include "TObject.h"

class HOraConn;

class HOraUtil : public TObject {
private:
  HOraConn *pConn;       // pointer to Oracle connection class
public:
  HOraUtil(HOraConn* p=0);
  ~HOraUtil() {}
  void printOdbcDriverInfo();
  void printOdbcFunctionInfo();  
  ClassDef(HOraUtil,0) // utility class for Oracle
};

#endif  /* !HORAUTIL_H */
 



 
