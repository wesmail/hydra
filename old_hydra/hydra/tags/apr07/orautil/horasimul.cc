//*-- AUTHOR : Ilse Koenig
//*-- Created : 19/09/2002

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////////
//
// HOraSimul()
//
// Interface class to database Oracle for simulation runs and event generator files
//
///////////////////////////////////////////////////////////////////////////////////

using namespace std;
#include "horasimul.h"
#include "hdbconn.h"
#include "horasimutil.h"
#include "horasimeventfilestorage.h"
#include "horasimrunstorage.h"
#include <iostream>
#include <iomanip>

ClassImp(HOraSimul)
  
HOraSimul::HOraSimul() {
  // default constructor
  // creates the connection class but doesn't open a connection
  pConn=new HDbConn();
  pUtil=new HOraSimUtil(pConn);
  pEvtFileStorage=0;
}

HOraSimul::~HOraSimul() {
  // default constructor closes an open connection
  close();
  if (pEvtFileStorage) delete pEvtFileStorage;
  pEvtFileStorage=0;
  if (pUtil) delete pUtil;
  pUtil=0;
  if (pConn) delete pConn;
  pConn=0;
}

Bool_t HOraSimul::open(TString userName, TString dbName) {
  return pConn->connectDb(userName,dbName);
}

void HOraSimul::close() {
  pConn->closeDbConn();
}

Bool_t HOraSimul::check() {
  if (pConn->isOpen() && !pConn->isReadonly()) return kTRUE;
  return kFALSE;
}

void HOraSimul::print() {
  if (pConn->isOpen()) {
    //    cout<<pConn->getUserName()<<" connected to Oracle with ";
    //    if (pConn->isReadonly()) cout<<"Readonly access\n";
    //    else cout<<"Write access\n";
    cout<<"Connected to Oracle as user "<<pConn->getUserName()<<endl;
  } else cout<<"No connection to Oracle open\n";
}

HOraSimEventFileStorage* HOraSimul::getEvtFileStorage() {
  if (!pEvtFileStorage) pEvtFileStorage=new HOraSimEventFileStorage(pUtil);
  return pEvtFileStorage;
}

HOraSimRunStorage* HOraSimul::getRunStorage() {
  if (!pRunStorage) pRunStorage=new HOraSimRunStorage(pUtil);
  return pRunStorage;
}
