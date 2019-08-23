#ifndef HADES_H
#define HADES_H

#include <TObject.h>
#include <TObjString.h>
#include <TClonesArray.h>
#include <TString.h>
#include <TArrayI.h>
#include <TNamed.h>
#include <TBranch.h>
#include <TBrowser.h>
#include <TCollection.h>
#include <TTree.h>
#include <TGlobal.h>
#include <TFile.h>

class HDataSource;
class HEvent;
class HTree;
class HRuntimeDb;
class HTaskSet;
class HSpectrometer;

class Hades : public TObject {
protected:
  Bool_t fSplitLevel; // Indicates the split level (0,1,2)
  HDataSource *fDataSource; //! Data source where data are taken from
  HEvent *fCurrentEvent; //Event under reconstruction
  HTaskSet *fTask; //Task for each event.
  TFile *fOutputFile; //!File used to store the output tree
  HSpectrometer *setup; //! Spectrometer s setup.
  //HError *fLatestError; 
    HTree *fTree; //Output tree
   HRuntimeDb *rtdb; //! Runtime database of reconstruction parameters.
   //HHistBrowser fHistBrowser;
    TObjArray *fTaskList; // List of Task for each type of events.
public:
  Hades(void);
  ~Hades(void);
  Int_t eventLoop(Int_t nEvents=kMaxInt,Int_t startEvent=0);// *MENU*
  void setDataSource(HDataSource *dataS);
  //HDataSource *getDataSource(void);

  void setSplitLevel(Int_t splitLevel);
  Int_t getSplitLevel(void);

  HRuntimeDb* getRuntimeDb(void) {return rtdb;}  
  HSpectrometer* getSetup(void) {return setup;}

  Bool_t setOutputFile(Text_t *name,Option_t *opt,Text_t *title,Int_t comp);
  TFile* getOutputFile(){return fOutputFile;}
  Int_t setAlgorithmLayout(Text_t *fileName);
  Int_t setEventLayout(Text_t *fileName);//
  Int_t setConfig(Text_t *fileName);
  Bool_t init(void);

  //Int_t loadLibrary(Text_t *file);
  //Int_t unloadLibrary(Text_t *file);

  HEvent *&getCurrentEvent(void);
  void setEvent(HEvent *ev);
  //Bool_t loadEvent(Int_t nEvent);
  HTaskSet *getTask(void);
  HTaskSet *getTaskSet(Int_t nEvtId);
  HTaskSet *getTaskSet(Char_t *evtTypeName); 
  //HError *getErrorInfo();
  HTree *getTree(void);
  Bool_t makeTree(void);
  void activateTree(TTree *tree);
  //void drawCurrentEvent(void);
  Bool_t IsFolder(void);
  void Browse(TBrowser *b);
  Bool_t initTasks();
  Bool_t reinitTasks();
  Bool_t finalizeTasks();

  ClassDef(Hades,1) //Control class for Hades reconstruction 
};	
 
R__EXTERN Hades *gHades;
 
#endif // !HADES
