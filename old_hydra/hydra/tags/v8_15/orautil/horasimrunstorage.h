#ifndef HORASIMRUNSTORAGE_H
#define HORASIMRUNSTORAGE_H
#include "TObject.h"
#include "TString.h"

class HOraSimUtil;
class HOraSimRun;
class HOraSimRunConfig;

class HOraSimRunStorage : public TObject {
private:
  HOraSimUtil* pUtil;  // pointer to the utility class
  TList* runs;         // list of simulation runs
  TString project;     // name of simulation project
  TString subProject;  // name of simulation sub-project
  Int_t generation;    // generation of simulation
  Int_t simRefRunId;   // simulation reference run
public:
  HOraSimRunStorage(HOraSimUtil* p=0);
  ~HOraSimRunStorage();
  void setProjectName(const char* s) { project=s; }
  void setSubProjectName(const char* s) { subProject=s; }
  void setGeneration(const Int_t n) { generation=n; }
  const char* getProjectName() { return project.Data(); }
  const char* getSubProjectName() { return subProject.Data(); }
  Int_t getGeneration() { return generation; }
  Int_t getSimRefRunId() { return simRefRunId; }
  Bool_t readAllFromFile(const char*);
  Bool_t store();
  void print();
private:
  Bool_t readRunFromFile(const char* s,TString&,TString&,TString&,
                         TString&,TString&,TString&,TString&);
  Int_t readSimRefRunId();
  Int_t readConfig(HOraSimRunConfig*);
  Int_t storeConfig(HOraSimRunConfig*);
  Bool_t storeRun(HOraSimRun*);
  ClassDef(HOraSimRunStorage,0) // Class to store simulation runs in Oracle
};

#endif  /* !HORASIMRUNSTORAGE_H */
