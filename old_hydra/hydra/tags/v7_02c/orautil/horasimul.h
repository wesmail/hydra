#ifndef HORASIMUL_H
#define HORASIMUL_H

#include "TObject.h"
#include "TString.h"

class HDbConn;
class HOraSimUtil;
class HOraSimEventFileStorage;
class HOraSimRunStorage;

class HOraSimul : public TObject {
private:
  HDbConn* pConn;                           // pointer to the connection class
  HOraSimUtil* pUtil;                       // pointer to the utility class
  HOraSimEventFileStorage* pEvtFileStorage; // pointer to class to store event generator files  
  HOraSimRunStorage* pRunStorage;           // pointer to class to store simulation runs  
public:
  HOraSimul();
  ~HOraSimul();
  Bool_t open(TString, TString dbName="db-hades.gsi.de");
  void close();
  Bool_t check();
  void print();
  HOraSimEventFileStorage* getEvtFileStorage();
  HOraSimRunStorage* getRunStorage();
//  Bool_t setExperiment(const char*);
private:
  ClassDef(HOraSimul,0) // Class to store simulation runs in Oracle
};

#endif  /* !HORASIMUL_H */
