//*-- AUTHOR : Ilse Koenig
//*-- Modified : 21/05/99

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
#include "hmdcparoraio.h"
#include "hades.h"
#include "hspectrometer.h"
#include <stdio.h>
#include <iostream.h>

ClassImp(HParOraIo)

HParOraIo::HParOraIo() {
  // default constructor
  // creates the connection class but doesn't open a connection
  pConn=new HOraConn();
  detParIoList=new TList();
}

HParOraIo::~HParOraIo() {
  // default constructor closes an open connection
  delete detParIoList;
  delete pConn;
}

Bool_t HParOraIo::open() {
  // opens connection to database Hades as user Hades
  // user Hades has Readonly access to Oracle tables
  // all open()-functions activate the detetctor I/Os
  gHades->getSetup()->activateParIo(this);
  return pConn->open();
}

Bool_t HParOraIo::open(char *userName) {
  // opens connection to database Hades for user given by name
  // asks for password
  gHades->getSetup()->activateParIo(this);
  return pConn->open(userName);
}

Bool_t HParOraIo::open(char *dbName, char *userName) {
  // opens connection to database with name dbName for user given by name
  // asks for password
  gHades->getSetup()->activateParIo(this);
  return pConn->open(dbName,userName);
} 

void HParOraIo::close() {
  // closes the connection with automatic ROLLBACK
  pConn->close();
  detParIoList->Delete();
}

void HParOraIo::print() {
  // prints information about the database connection
  pConn->print();
  cout<<"input/output for detectors: "<<'\n';
  detParIoList->ls();
}

void HParOraIo::setDetParIo(Text_t* detIo) {
  // creates interface class for a special detector
  if (detParIoList->FindObject(detIo)) return;
  if (strcmp(detIo,"HMdcParIo")==0) {
    HMdcParOraIo* p=new HMdcParOraIo(pConn);
    detParIoList->Add(((HDetParIo*)p));
    return;
  }
  cerr<<"Parameter-I/O "<<detIo<<" not yet implemented in Oracle!"<<endl;
}  
