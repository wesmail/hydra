#ifndef HRUNTIMEDB_H
#define HRUNTIMEDB_H

#include "TList.h"

class HParIo;
class HParFileIo;
class HParSet;
class HEventFile;

class HRuntimeDb {
protected:
  TList* containerList;   // list of parameter containers
  TList* eventFiles;      // list of event files
  HParIo* firstInput;     // first (prefered) input for parameters
  HParIo* secondInput;    // second input (used of not found in first input)
  HParIo* output;         // output
  Int_t currentFilePos;   // position of current event file in the list
  Bool_t versionsChanged; // flag for write of list eventFiles (set kTRUE by each write)
public:
  HRuntimeDb(void);
  virtual ~HRuntimeDb(void);

  Bool_t addContainer(HParSet* container);
  HParSet* getContainer(Text_t* name);
  void removeContainer(Text_t* name);
  void clearContainerList(void);
  Bool_t initContainers(void);
  void setContainersStatic(Bool_t f=kTRUE);
  Bool_t writeContainers(void);
  Bool_t writeContainer(HParSet* cont, HEventFile* file);

  void addEventFile(Text_t* name,Text_t* refFile="");
  void insertEventFile(Text_t* name,Int_t position,Text_t* refFile="");
  Int_t getEventFilePosition(Text_t*name);
  Int_t getNumEventFiles(void);
  void setCurrentEventFile(Int_t n=-1);
  HEventFile* getNextEventFile(void);
  HEventFile* getCurrentEventFile(void);
  HEventFile* getEventFile(Int_t n=0);
  HEventFile* getEventFile(Text_t* name);
  void removeEventFile(Int_t n=0);
  void removeEventFile(Text_t* name);
  TList* getEventFileList(void);
  void clearEventFileList(void);
  Bool_t setInputVersion(Text_t* eventFileName,Text_t* container,
                        Int_t version,Int_t inputNumber);
  Bool_t setRootOutputVersion(Text_t* eventFileName,Text_t* container,Int_t version);
  void setVersionsChanged(Bool_t f=kTRUE) {versionsChanged=f;}
  void resetInputVersions(void);
  void resetOutputVersions(void);
  void resetAllVersions(void);

  Bool_t readAll(void);
  void writeSetup(void);
  void writeVersions(void);
  void saveOutput(void);

  Bool_t setFirstInput(HParIo* inp1);
  Bool_t setSecondInput(HParIo* inp2);
  Bool_t setOutput(HParIo* output);
  HParIo* getFirstInput(void);
  HParIo* getSecondInput(void);
  HParIo* getOutput(void);
  void closeFirstInput(void);
  void closeSecondInput(void);
  void closeOutput(void);

  void print(void);

  Int_t findOutputVersion(HParSet* cont);

  ClassDef(HRuntimeDb,0) // Class for runtime database
};

#endif  /* !HRUNTIMEDB_H */
