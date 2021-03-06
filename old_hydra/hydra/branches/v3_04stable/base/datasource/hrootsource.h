#ifndef HROOTSOURCE_H
#define HROOTSOURCE_H

#include "hdatasource.h"
#include "haddef.h"
#include <TFile.h>
#include <TTree.h>

class HRootSource : public HDataSource {
private:
  TTree *fInputTree; //TTree to be read.
  TFile *fInputFile; //Input file.
  Int_t fCursor; //Number of next event.
  Int_t fSplitLevel; //Split level of input tree
  HEvent *fEventInFile; //! Pointer to the event structure in file
public:
  HRootSource(void);
  ~HRootSource(void);
  EDsState getNextEvent(void);
  Bool_t init(void);
  Bool_t finalize(void) {return kTRUE;}
  Bool_t getEvent(Int_t eventN);
  Bool_t setInput(Text_t *fileName,Text_t *treeName);
  Bool_t disableCategory(Cat_t aCat);
  void deactivateBranch(Text_t *branchName);
  TTree *getTree(void);
  Int_t getSplitLevel(void);
  void Clear(void);
  ClassDef(HRootSource,1) //Data source to read Root files.
};

#endif /* !HROOTSOURCE_H */


