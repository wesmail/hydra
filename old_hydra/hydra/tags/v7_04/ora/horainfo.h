#ifndef HORAINFO_H
#define HORAINFO_H

#include "TObject.h"
#include "TString.h"
#include "TList.h"

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
  Int_t getLastRun(Text_t*);
  TList* getListOfRuns(const char* ,const char* startAt="",const char* endAt="");
private:
  TList* getListOfSimRefRuns(const char*,const char*,const char*);
  Bool_t getRunStart(const char*,TString&);
  Bool_t getSimRefRunStart(const char*,TString&);
  ClassDef(HOraInfo,0) // utility class for the interface to Oracle
};

#endif  /* !HORAINFO_H */
