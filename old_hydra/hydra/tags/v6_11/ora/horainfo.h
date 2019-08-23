#ifndef HORAINFO_H
#define HORAINFO_H

#include "TObject.h"

class HOraConn;

class HOraInfo : public TObject {
private:
  HOraConn* pConn;   // pointer to Oracle connection class
public:
  HOraInfo(HOraConn* p=0) {pConn=p;}
  ~HOraInfo(void) {}
  void showSqlError(char*);
  void showRunStart(Int_t);  
  Int_t getRunId(Text_t*);
  ClassDef(HOraInfo,0) // utility class for the interface to Oracle
};

#endif  /* !HORAINFO_H */
