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
  HDbConn* pConn;     // pointer to the connection class
public:
  HOraSlowReader();
  ~HOraSlowReader();
  Bool_t open();
  void close();
  void print();
  Bool_t isOpen();
  Bool_t readPartition(HOraSlowPartition*);
  Bool_t readRunPeriods(HOraSlowPartition*);
  Bool_t readArchiverRates(HOraSlowPartition*);
  Bool_t readChannelRunSum(HOraSlowPartition*,HOraSlowChannel*);  
  Bool_t readChannelMetaData(HOraSlowPartition*,HOraSlowChannel*);
  Bool_t readRawData(HOraSlowChannel*,const char*,const char*);
  Bool_t readOnlineRawData(HOraSlowChannel*,const char*,const char*);
private:
  Bool_t readOnlineRunPeriods(HOraSlowPartition*);
  Bool_t readOnlineArchiverRates(HOraSlowPartition*);
  Bool_t readOnlineChannelRunSum(HOraSlowPartition*,HOraSlowChannel*);  
  Bool_t readOnlineChannelMetaData(HOraSlowChannel*);
  Int_t readChannelId(HOraSlowChannel*);
  Int_t readOnlineChannelId(HOraSlowChannel*);
  Bool_t readPeriods(char*,TObjArray*,Int_t,Int_t,Int_t);
  Bool_t readOnlinePeriods(TObjArray*,Int_t,Int_t,Int_t);
  Int_t readRawF(HOraSlowChannel*,const char*,const char*);
  Int_t readOnlineRawF(HOraSlowChannel*,const char*,const char*);
  Int_t readRawI(HOraSlowChannel*,const char*,const char*);
  Int_t readOnlineRawI(HOraSlowChannel*,const char*,const char*);
  ClassDef(HOraSlowReader,0) // Class to read slowcontrol data from Oracle
};

#endif  /* !HORASLOWREADER */
