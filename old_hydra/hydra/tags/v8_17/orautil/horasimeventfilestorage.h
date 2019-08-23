#ifndef HORASIMEVENTFILESTORAGE_H
#define HORASIMEVENTFILESTORAGE_H

#include "TObject.h"
#include "TString.h"
#include "TList.h"

class HOraSimEventFileConfig;
class HOraSimUtil;

class HOraSimEventFileStorage : public TObject {
private:
  HOraSimUtil* pUtil;              // pointer to the utility class
  TList* eventFiles;               // list of event generator files
  HOraSimEventFileConfig* config;  // configuration of event generator files
public:
  HOraSimEventFileStorage(HOraSimUtil* p=0);
  ~HOraSimEventFileStorage();
  Bool_t readFromFile(const Char_t*);
  Bool_t store();
  void print();
private:
  void addNewEvtFile(TString&);
  Int_t storeConfig();
  ClassDef(HOraSimEventFileStorage,0) // Class to store event files for HGEANT in Oracle
};

#endif  /* !HORASIMEVENTFILESTORAGE_H */
