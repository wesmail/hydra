///*-- AUTHOR : Ilse Koenig
//*-- Created : 16/08/2004 by Ilse Koenig

//////////////////////////////////////////////////////////////////////////////////////
//
// HOraSlowManager
//
// Manager class for slowcontrol data (Singleton with global pointer gHOraSlowManager)
//
//////////////////////////////////////////////////////////////////////////////////////

#include "horaslowmanager.h"
#include "horaslowpartition.h"
#include "horaslowchannel.h"
#include "horaslowreader.h"
#include "TObjString.h"

ClassImp(HOraSlowManager)

HOraSlowManager::HOraSlowManager() {
  // Default constructor
  gHOraSlowManager=this;
  partition=0;
  pOraReader=0;
  hldfileFilter=0;
}

HOraSlowManager::~HOraSlowManager() {
  // Destructor
  channels.Delete();
  clearHldfileFilter();
  if (partition) {
    delete partition;
    partition=0;
  }
  if (pOraReader) {
    delete pOraReader;
    pOraReader=0;
  }
  gHOraSlowManager=0;
}

Bool_t HOraSlowManager::openOraInput() {
  // Connnects to Oracle (is done automatically when needed)
  if (pOraReader==0) {
    pOraReader=new HOraSlowReader();
  }
  return pOraReader->open();
}

void HOraSlowManager::closeOraInput() {
  // Closes the Oracle connection
  if (pOraReader) pOraReader->close();
}

HOraSlowPartition* HOraSlowManager::setPartition(const char* pName,
                   const char* t1,const char* t2) {
  // Sets the data partition and the time range (typically smaller then the time range of the partition)
  Int_t t1len=strlen(t1);
  Int_t t2len=strlen(t2);
  Bool_t wrongFormat=kFALSE;
  if (t1len>0) {
    if (t1len<10||t1[4]!='-'||t1[7]!='-') wrongFormat=kTRUE;
  }
  if (wrongFormat==kFALSE&&t2len>0) {
    if (t2len<10||t2[4]!='-'||t2[7]!='-') wrongFormat=kTRUE;
  }
  if (wrongFormat) {
    Error("setPartition","Dates must be specified in date format yyyy-mm-dd hh:mi:ss");
    return 0;
  }
  TString name(pName);
  name.ToLower();
  if (partition&&name!=partition->GetName()) {
    delete partition;
  }
  partition=new HOraSlowPartition(name);
  partition->setStartTime(t1);
  partition->setEndTime(t2);
  TIter next(&channels);
  HOraSlowChannel* p=0;
  while((p=(HOraSlowChannel*)next())) {
    p->setPartition(partition);
  }
  return partition;  
}

HOraSlowChannel* HOraSlowManager::addChannel(const char* name) {
  // Adds a channel to the list of channels
  HOraSlowChannel* p=getChannel(name);
  if (p==0) {
    p=new HOraSlowChannel(name);
    p->setPartition(partition);
    channels.Add(p);
  }
  return p;  
}

void HOraSlowManager::removeChannel(const char* name) {
  // Removes a channel from the list of channels
  TObject* p=channels.FindObject(name);
  if (p) {
    channels.Remove(p);
    delete p;
  }
}

void HOraSlowManager::removeAllChannels() {
  // Removes all channels from the list of channels 
  channels.Delete();
}

HOraSlowChannel* HOraSlowManager::getChannel(const char* name) {
  // Returns a pointer to the channel objects specified by name
  return (HOraSlowChannel*)(channels.FindObject(name));
}

void HOraSlowManager::addHldfileFilter(const char* s) {
  // Adds an hld-file to the list of hld-files to be applied as filter for output and drawing
  if (!hldfileFilter) hldfileFilter=new TList();
  hldfileFilter->Add(new TObjString(s));
}

void HOraSlowManager::clearHldfileFilter() {
  // Clears the hld-file filter
  if (hldfileFilter) {
    hldfileFilter->Delete();
    delete hldfileFilter;
    hldfileFilter=0;
  }
}

Bool_t HOraSlowManager::readArchiverRates() {
  // Reads the rates of the channel archiver 
  if (!partition) {
    Error("readSummary","No partition defined!");
    return kFALSE;
  }
  Bool_t rc=openOraInput();
  if (!rc) return rc;
  return pOraReader->readArchiverRates(partition);
}

Bool_t HOraSlowManager::readSummary() {
  // Reads the summary information for all channels
  if (!partition) {
    Error("readSummary","No partition defined!");
    return kFALSE;
  }
  Bool_t rc=openOraInput();
  if (!rc) return rc;
  if (!partition->getRunPeriods()) {
    rc=pOraReader->readRunPeriods(partition);
  }  
  if (!rc) return rc;
  partition->setHldFileFilter(hldfileFilter);
  HOraSlowChannel* ch=0;
  TListIter iter(&channels);
  while((ch=(HOraSlowChannel*)iter.Next())&&rc) {
    rc=pOraReader->readChannelMetaData(partition,ch);
    if (!rc) break;
    rc=pOraReader->readChannelRunSum(partition,ch);
  }
  return rc;
}

void HOraSlowManager::writeSummaryToAscii(const char* filename,Int_t opt) {
  // Writes the periods and the channel summary information to an ASCII file
  // option opt 0 = all periods
  //            1 = only hld-files
  //            2 = apply hld-file filter
  if (strlen(filename)==0||partition==0) return;
  fstream file;
  file.open(filename,ios::out);
  if (file.rdbuf()->is_open()==0) {
    Error("open","Failed to open file %s",filename);
    return;
  }
  partition->write(file,opt);
  HOraSlowChannel* ch=0;
  TListIter iter(&channels);
  while((ch=(HOraSlowChannel*)iter.Next())) {
    ch->writeMetaData(file);
    ch->writeRunSumData(file,opt);
  }
  file.close();
  cout<<"Data of partition "<<partition->GetName()
      <<" written to ASCII file"<<endl;
}

HOraSlowManager* gHOraSlowManager;
