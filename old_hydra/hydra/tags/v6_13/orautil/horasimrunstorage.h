#ifndef HORASIMRUNSTORAGE_H
#define HORASIMRUNSTORAGE_H
#include "TObject.h"
#include "TString.h"

class HOraSimUtil;
class HOraSimRun;
class HOraSimRunConfig;

class HOraSimRunStorage : public TObject {
private:
  HOraSimUtil* pUtil;        // pointer to the utility class
  TList* runs;               // list of simulation runs
  TString project;
  TString subProject;
  Int_t projectId; 
public:
  HOraSimRunStorage(HOraSimUtil* p=0);
  ~HOraSimRunStorage();
  void setProjectName(const char* s) { project=s; }
  void setSubProjectName(const char* s) { subProject=s; }
  const char* getProjectName() { return project.Data(); }
  const char* getSubProjectName() { return subProject.Data(); }
  Bool_t readAllFromFile(const char*);
  Bool_t store();
  void print();
private:
  Int_t getProjectId();    
  Bool_t readRunFromFile(const char* s,Int_t,TString&,TString&,
                       TString&,TString&,TString&,TString&,TString&);
  Int_t readLastConfig(Int_t,HOraSimRunConfig*);
  Int_t storeConfig(HOraSimRunConfig*);
  Bool_t storeRun(HOraSimRun*,Int_t,Int_t);
  ClassDef(HOraSimRunStorage,0) // Class to store simulation runs in Oracle
};

#endif  /* !HORASIMRUNSTORAGE_H */
