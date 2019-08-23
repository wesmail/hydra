#ifndef HRFIOROOTSOURCE_H
#define HRFIOROOTSOURCE_H

#include "hdatasource.h"
#include "haddef.h"
#include "hgeantdef.h"
#include "calodef.h"
#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
#include <map.h>
#include <TString.h>



class HRFIORootSource : public HDataSource {
private:
  TChain *fInput;         //TTree to be read.
  //TFile *fInputFile;    //Input file.
  TString fDirectory;
  TString fArchive;
  Stat_t fEntries;
  Bool_t fPersistency;
  Int_t fCursor;          //Number of next event.
  Int_t fSplitLevel;      //Split level of input tree
  Int_t fCurrentRunId;
  Int_t fCurrentRefId;
  map<Int_t,Int_t> fRefIds;//!
  HEvent *fEventInFile;   //! Pointer to the event structure in file
  Bool_t fileExists(const TString &name);
  Bool_t access(const TString &name, Int_t mode);
public:
  HRFIORootSource(Bool_t fPersistent=kTRUE);
  ~HRFIORootSource(void);
  EDsState getNextEvent(void);
  Bool_t init(void);
  Bool_t reinit(void) {return kTRUE; }
  Bool_t finalize(void) {return kTRUE;}
  Int_t getCurrentRunId(void) {return fCurrentRunId;}
  Int_t getCurrentRefId(void) {return fCurrentRefId;}
  void setRefId(Int_t runId, Int_t refId) {fRefIds[runId]=refId;}
  Text_t const *getCurrentFileName(void) {return "";}
  Bool_t getEvent(Int_t eventN);
  void setDirectory(Text_t dirName[]);
  void setArchive(Text_t archiveName[]);
  const char* getDirectory(void){return fDirectory.Data();}
  const char* getArchive(void){return fArchive.Data();}
  Bool_t addFile(Text_t file[]);
  Bool_t setInput(Text_t *fileName,Text_t *archiveName,Text_t *treeName);
  Bool_t setInput(Text_t *fileName,Text_t *treeName);
  Bool_t disableCategory(Cat_t aCat);
  void deactivateBranch(Text_t *branchName);
  TTree *getTree(void);
  Int_t getSplitLevel(void);
  void Clear(void);
  ClassDef(HRFIORootSource,1) //Data source to read Root files from the robot.
};

#endif /* !HRFIOROOTSOURCE_H */


 
