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
#include <TError.h>

class HDataSource;
class HEvent;
class HTree;
class HRuntimeDb;
class HTask;
class HTaskSet;
class HSpectrometer;
class Hades;
class HMessageMgr;

R__EXTERN Hades *gHades;

class Hades : public TObject {
protected:
  Size_t fOutputSizeLimit;
  Int_t fCycleNumber; //Number of current output file
  TString fOutputFileName;
  Bool_t fFirstEventLoop;
  Int_t fSplitLevel; // Indicates the split level (0,1,2)
  HDataSource *fDataSource; //! Data source where data are taken from
  HEvent *fCurrentEvent; //Event under reconstruction
  HTaskSet *fTask; //Task for each event.
  TFile *fOutputFile; //!File used to store the output tree
  HSpectrometer *setup; //! Spectrometer s setup.
  HTree *fTree; //Output tree
  HRuntimeDb *rtdb; //! Runtime database of reconstruction parameters.
  TObjArray *fTaskList; // List of Task for each type of events.
  Bool_t quiet; 
  Int_t fCounter;  // Counter display for event loop
  ErrorHandlerFunc_t oldHdlr; //! Original ROOT error handler
  HMessageMgr *msgHandler;  //! Message handler with three different outputs

 void recreateOutput(void);
public:
  Hades(void);
  ~Hades(void);
  static Hades *instance(void) { return (gHades)?gHades:new Hades; }
  Int_t eventLoop(Int_t nEvents=kMaxInt,Int_t firstEvent=0);// *MENU*
  void setDataSource(HDataSource *dataS);
  HDataSource *getDataSource(void) const {return fDataSource;}

  void setSplitLevel(Int_t splitLevel);
  Int_t getSplitLevel(void);

  HRuntimeDb* getRuntimeDb(void) {return rtdb;}  
  HSpectrometer* getSetup(void) {return setup;}

  Bool_t setOutputFile(Text_t *name,Option_t *opt,Text_t *title,Int_t comp);
  TFile* getOutputFile(){return fOutputFile;}
  Int_t setAlgorithmLayout(Text_t *fileName);
  Int_t setEventLayout(Text_t *fileName);
  void  makeCounter(Int_t counter = 1000) {fCounter = counter;}
  Int_t setConfig(Text_t *fileName);
  Bool_t init(void);

  HEvent *&getCurrentEvent(void);
  void setEvent(HEvent *ev);
  HTask *getTask(Char_t *taskName);
  HTaskSet *getTask(void);
  HTaskSet *getTaskSet(Int_t nEvtId);
  HTaskSet *getTaskSet(Char_t *evtTypeName); 
  HTree *getTree(void);
  Bool_t makeTree(void);
  void activateTree(TTree *tree);
  Bool_t IsFolder(void) const;
  void Browse(TBrowser *b);
  Bool_t initTasks();
  Bool_t reinitTasks();
  Bool_t finalizeTasks();
  void closeOutput();
  void setOutputSizeLimit(Size_t l) { fOutputSizeLimit = l; } 
  void  setQuietMode() {quiet=kTRUE;}
  HMessageMgr *getMsg(void){return msgHandler;}
  ClassDef(Hades,1) //Control class for Hades reconstruction 
};	
 
 
#endif // !HADES
