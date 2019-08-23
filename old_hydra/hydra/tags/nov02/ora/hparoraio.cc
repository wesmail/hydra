//*-- AUTHOR : Ilse Koenig
//*-- Modified : 27/10/2002 by I.Koenig

///////////////////////////////////////////////////////////////////////////////
// HParOraIo
//
// Interface class to database Oracle
// It is derived from interface base class HParIo.
//
// It contains a pointer to the connection class (type HOraConn)  and a list
// of interface classes for the detectors. Each detector has its own interface
// class all inherited from a common base class.
//
// This class replaces the former class HOraIo.  
///////////////////////////////////////////////////////////////////////////////

#include "hparoraio.h"
#include "hdetpario.h"
#include "horainfo.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hspecparoraio.h"
#include "hmdcparoraio.h"
#include "htofparoraio.h"
#include "htofinoparoraio.h"
#include "hrichparoraio.h"
#include "hshowerparoraio.h"
#include "hstartparoraio.h"
#include "hkickparoraio.h"
#include "htriggerparoraio.h"
#include <stdio.h>
#include <iostream.h>

ClassImp(HParOraIo)

HParOraIo::HParOraIo() {
  // default constructor
  // creates the connection class but doesn't open a connection
  pConn=new HOraConn();
  autoWritable=kFALSE;
}


HParOraIo::~HParOraIo() {
  // default constructor closes an open connection
  close();
  if (pConn) delete pConn;
}


Bool_t HParOraIo::open() {
  // opens connection to database Hades as user Hades
  // user Hades has Readonly access to Oracle tables
  // all open()-functions activate the detetctor I/Os
  isConnected=pConn->open();
  return activateDetIo();
}


Bool_t HParOraIo::open(char *userName) {
  // opens connection to database Hades for user given by name
  // asks for password
  isConnected=pConn->open(userName);
  return activateDetIo();
}


Bool_t HParOraIo::open(char *dbName, char *userName) {
  // opens connection to database with name dbName for user given by name
  // asks for password
  isConnected=pConn->open(dbName,userName);
  return activateDetIo();
} 


void HParOraIo::close() {
  // closes the connection with automatic ROLLBACK
  pConn->close();
  isConnected=kFALSE;
  detParIoList->Delete();
  if (pInfo) delete pInfo;
}


void HParOraIo::print() {
  // prints information about the database connection
  pConn->print();
  if (isConnected) {
    TIter next(detParIoList);
    HDetParIo* io;
    cout<<"detector I/Os: ";
    while ((io=(HDetParIo*)next())) {
      cout<<" "<<io->GetName();
    }
    cout<<'\n';
  }
}

Bool_t HParOraIo::setHistoryDate(const char* timestamp) {
  // Sets the date to retrieve historic data
  // Returns kFALSE when the date string cannot be converted to a valid date.
  return pConn->setHistoryDate(timestamp);
}

Bool_t HParOraIo::setParamRelease(const char* releaseName) {
  // Sets the history date to the creation date of the parameter release give by name
  // Returns kFALSE when the release is not found.
  return pConn->setParamRelease(releaseName);
}

Bool_t HParOraIo::activateDetIo() {
  // creates the I/O for all detector in the setup
  if (isConnected==kFALSE) return kFALSE;
  pInfo=new HOraInfo(pConn);
  HDetParIo* io=new HSpecParOraIo(pConn);
  detParIoList->Add(io);
  TList* detSet=gHades->getSetup()->getListOfDetectors();
  TIter next(detSet);
  HDetector* det;
  while ((det=(HDetector*)next())) {
    const char* name=det->GetName();
    io=0;
    if (strcmp(name,"Mdc")==0) io=new HMdcParOraIo(pConn);
    if (strcmp(name,"Tof")==0) io=new HTofParOraIo(pConn);
    if (strcmp(name,"Tofino")==0) io=new HTofinoParOraIo(pConn);
    if (strcmp(name,"Rich")==0) io=new HRichParOraIo(pConn);
    if (strcmp(name,"Shower")==0) io=new HShowerParOraIo(pConn);
    if (strcmp(name,"Start")==0) io=new HStartParOraIo(pConn);
    if (strcmp(name,"Trigger")==0) io=new HTriggerParOraIo(pConn);
    if (io) detParIoList->Add(io);
  }
  return kTRUE;
}

void HParOraIo::setDetParIo(Text_t* ioName) {
  // creates the specified I/O
  if (isConnected) {
    TObject* io=detParIoList->FindObject(ioName);
    if (!io) {
      if (strcmp(ioName,"HMdcParIo")==0) io=new HMdcParOraIo(pConn);
      if (strcmp(ioName,"HTofParIo")==0) io=new HTofParOraIo(pConn);
      if (strcmp(ioName,"HTofinoParIo")==0) io=new HTofinoParOraIo(pConn);
      if (strcmp(ioName,"HRichParIo")==0) io=new HRichParOraIo(pConn);
      if (strcmp(ioName,"HShowerParIo")==0) io=new HShowerParOraIo(pConn);
      if (strcmp(ioName,"HStartParIo")==0) io=new HStartParOraIo(pConn);
      if (strcmp(ioName,"HKickParIo")==0) io=new HKickParOraIo(pConn);
      if (strcmp(ioName,"HTriggerParIo")==0) io=new HTriggerParOraIo(pConn);
      if (io) detParIoList->Add(io);
    }
  }
}    
