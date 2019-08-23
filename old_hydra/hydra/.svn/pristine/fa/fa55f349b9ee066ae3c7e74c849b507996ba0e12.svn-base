//*-- AUTHOR : Ilse Koenig
//*-- Modified : 02/11/98

/////////////////////////////////////////////////////////////
//HOraIo
//
//  Interface class to database Oracle
//  It is derived from interface base class HParIo
//
//  It contains pointers to a list of interface classes
//  (inherited from the base class) and to the connection
//  class.
//  Every detector has its own interface class.
//  
/////////////////////////////////////////////////////////////

#include "horaio.h"
#include "horamdc.h"
#include "hades.h"
#include "hspectrometer.h"
#include <stdio.h>
#include <iostream.h>

ClassImp(HOraIo)

HOraIo::HOraIo() {
  // default constructor
  // creates the connection class but doesn't open a connection
  pConn=new HOraConn();
  detParIoList=new TList();
}

HOraIo::~HOraIo() {
  // default constructor closes an open connection
  delete detParIoList;
  delete pConn;
}

Bool_t HOraIo::open() {
  // opens connection to database Hades as user Hades
  // user Hades has Readonly access to Oracle tables
  // all open()-functions activate the detetctor I/Os
  gHades->getSetup()->activateParIo(this);
  return pConn->open();
}

Bool_t HOraIo::open(char *userName) {
  // opens connection to database Hades for user given
  // by name
  // asks for password
  gHades->getSetup()->activateParIo(this);
  return pConn->open(userName);
}

Bool_t HOraIo::open(char *dbName, char *userName) {
  // opens connection to database with name dbName for
  // user given by name
  // asks for password
  gHades->getSetup()->activateParIo(this);
  return pConn->open(dbName,userName);
} 

void HOraIo::close() {
  // closes the connection with automatic ROLLBACK
  pConn->close();
  detParIoList->Delete();
}

void HOraIo::print() {
  // prints information about the database connection
  pConn->print();
  cout<<"input/output for detectors: "<<'\n';
  detParIoList->ls();
}

void HOraIo::setDetParIo(Text_t* detIo) {
  // creates interface class for a special detector
  if (detParIoList->FindObject(detIo)) return;
  if (strcmp(detIo,"HMdcParIo")==0) {
    HOraMdc* p=new HOraMdc(pConn);
    detParIoList->Add(((HDetParIo*)p));
    return;
  }
  cerr<<"Parameter-I/O "<<detIo<<" not yet implemented in Oracle!"<<endl;
}  
