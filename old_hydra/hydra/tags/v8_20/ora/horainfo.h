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
  void showSqlError(Char_t*);
  void showRunStart(Int_t);  
  Int_t getRunId(Text_t*);
  Int_t getRefRunId(Text_t*);
  Int_t getLastRun(Text_t*);
  Bool_t getDaqFilename(Int_t,TString&);
  TList* getListOfRuns(const Char_t* ,const Char_t* startAt="",const Char_t* endAt="");
private:
  TList* getListOfSimRefRuns(const Char_t*,const Char_t*,const Char_t*);
  Bool_t getRunStart(const Char_t*,TString&);
  Bool_t getSimRefRunStart(const Char_t*,TString&);
  ClassDef(HOraInfo,0) // utility class for the interface to Oracle
};

#endif  /* !HORAINFO_H */
