#ifndef HORASLOWMANAGER_H
#define HORASLOWMANAGER_H

#include "TObject.h"
#include "TList.h"
#include "TGlobal.h"

class HOraSlowManager;
class HOraSlowPartition;
class HOraSlowReader;
class HOraSlowChannel;

R__EXTERN HOraSlowManager* gHOraSlowManager;

class HOraSlowManager : public TObject {
private:
  HOraSlowPartition* partition;     // Data partition 
  TList              channels;      // List of channels
  HOraSlowReader*    pOraReader;    // Interface to Oracle
  TList*             hldfileFilter; // List of hld-files to be applied as filter for output and drawing
public:
  HOraSlowManager();
  ~HOraSlowManager();
  static HOraSlowManager* instance(void) {
    return (gHOraSlowManager)?gHOraSlowManager:new HOraSlowManager;
  }
  Bool_t openOraInput();
  void closeOraInput();
  HOraSlowReader* getOraReader() {return pOraReader;}
  HOraSlowPartition* setPartition(const char*,const char* startTime="",
                                  const char* endTime="");
  HOraSlowPartition* getPartition() {return partition;}
  TList& getChannels() {return channels;}
  HOraSlowChannel* addChannel(const char*);
  HOraSlowChannel* getChannel(const char*);
  void removeChannel(const char*);
  void removeAllChannels();
  Bool_t readSummary();
  Bool_t readArchiverRates();
  void writeSummaryToAscii(const char*,Int_t opt=0);
  void addHldfileFilter(const char*);
  void clearHldfileFilter();
private:
  ClassDef(HOraSlowManager,0) // Class to manage slowcontrol data
};

#endif  /* !HORASLOWMANAGER */
