#ifndef HORASLOWREADER_H
#define HORASLOWREADER_H

using namespace std;
#include "TObject.h"
#include "TString.h"

class HDbConn;
class HOraSlowPartition;
class HOraSlowPeriod;
class HOraSlowChannel;

class HOraSlowReader : public TObject {
private:
  HDbConn* pConn;     // pointer to connection class
public:
  HOraSlowReader();
  ~HOraSlowReader();
  Bool_t open();
  void close();
  void print();
  Bool_t isOpen();
  Bool_t readPartition(HOraSlowPartition*);
  Int_t readChannelId(HOraSlowChannel*);
  Bool_t readRunPeriods(HOraSlowPartition*);
  Bool_t readArchiverRates(HOraSlowPartition*);
  Bool_t readChannelRunSum(HOraSlowPartition*,HOraSlowChannel*);  
  Bool_t readChannelMetaData(HOraSlowPartition*,HOraSlowChannel*);
  Bool_t readRawData(HOraSlowChannel*,const char*,const char*);
private:
  Bool_t readPeriods(char*,TObjArray*,Int_t,Int_t,Int_t);
  Int_t readRawF(HOraSlowChannel*,const char*,const char*);
  Int_t readRawI(HOraSlowChannel*,const char*,const char*);
  ClassDef(HOraSlowReader,0) // Class to read slowcontrol data from Oracle
};

#endif  /* !HORASLOWREADER */
