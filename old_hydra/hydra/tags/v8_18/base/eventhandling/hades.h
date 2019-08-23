#ifndef HADES_H
#define HADES_H

#include "TObject.h"
#include "TObjString.h"
#include "TClonesArray.h"
#include "TString.h"
#include "TArrayI.h"
#include "TNamed.h"
#include "TBranch.h"
#include "TBrowser.h"
#include "TCollection.h"
#include "TTree.h"
#include "TGlobal.h"
#include "TFile.h"
#include "TError.h"
#include "TStopwatch.h"

#define MAXEVID 16

class HDataSource;
class HldFileOutput;
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
    Size_t             fOutputSizeLimit;
    Int_t              fCycleNumber;       //  Number of current output file
    TString            fOutputFileName;
    Bool_t             fFirstEventLoop;
    Int_t              fSplitLevel;        //  Indicates the split level (0,1,2)
    HDataSource*       fDataSource;        //! Data source where data are taken from
    HDataSource*       fSecondDataSource;  //! 2nd data source where data are taken from
    HldFileOutput*     fHldOutput;         //! Hld file output
    HEvent*            fCurrentEvent;      //  Event under reconstruction
    HTaskSet*          fTask;              //  Task for each event.
    TFile*             fOutputFile;        //! File used to store the output tree
    HSpectrometer*     setup;              //! Spectrometer s setup.
    HTree*             fTree;              //  Output tree
    HRuntimeDb*        rtdb;               //! Runtime database of reconstruction parameters.
    TObjArray*         fTaskList;          //  List of Task for each type of events.
    UChar_t            quiet;              //  1 - quiet mode, 2 - silence mode
    Int_t              fCounter;           //  Counter display for event loop
    ErrorHandlerFunc_t oldHdlr;            //! Original ROOT error handler
    HMessageMgr*       msgHandler;         //! Message handler with three different outputs
    Int_t              reqEvents;          //! number of requested events from eventloop
    Bool_t             enableCloseInput;   //! Enable closing oracle input
    TStopwatch         fTimer;             //! Timer for event loop stoping by time quota
    static Int_t       EventCounter;       //! Event counter, can be retrieved via gHades->getEventCounter()
    static Int_t       doEmbedding;        //! embedding mode flag 0=No embedding,
    //                                                             1=realistic embedding,
    //                                                             2=keep Geant embedding
    //                                         can be retrived via gHades->getEmbeddingMode()
    static Bool_t forceNoSkip;             //! kTRUE = don't skip events in unpackers when trigger tag mismatch is detected

    Int_t evIdMap[MAXEVID];                //! look-up table for event id mapping
    Int_t treeBufferSize;                  //! size of the buffer of a branch (default 8000)
    void  recreateOutput (void);
    void  initTaskSetsIDs(Int_t version);
    Int_t getCurrentRunId(HDataSource* source);

public:
    Hades(void);
    ~Hades(void);
    static Hades*  instance                  (void) { return (gHades) ? gHades : new Hades; }
    Bool_t         init                      (void);
    Int_t          eventLoop                 (Int_t nEvents = kMaxInt,Int_t firstEvent = 0, Double_t timeQuota = 1.e+20); // *MENU*
    void           makeCounter               (Int_t counter = 1000) {fCounter = counter;}
    //----------------------------------------------
    // switches + counters etc
    void           setQuietMode              (UChar_t mode=1)       { quiet = mode;         }
    Int_t          getNumberOfRequestedEvents(void)                 { return reqEvents;     }
    Int_t          getEventCounter           ()                     { return EventCounter;  }
    void           resetEventCounter         ()                     { EventCounter = 0;     }
    Int_t          getEmbeddingMode          ()                     { return doEmbedding;   }
    void           setEmbeddingMode          (Int_t mode)           { doEmbedding = mode;   }
    Int_t          getEmbeddingRealTrackId   ()                     { return -500;          }
    Bool_t         getForceNoSkip            ()                     { return forceNoSkip;   }
    void           setForceNoSkip            (Bool_t noskip = kTRUE){ forceNoSkip = noskip; }
    //----------------------------------------------
    // setup rtdb + detector
    HRuntimeDb*    getRuntimeDb              (void) { return rtdb;       }
    HSpectrometer* getSetup                  (void) { return setup;      }
    HMessageMgr*   getMsg                    (void) { return msgHandler; }
    //----------------------------------------------
    // setup event
    HEvent*&       getCurrentEvent           (void);
    void           setEvent                  (HEvent *ev);
    //----------------------------------------------
    //  setup data source
    void           setDataSource             (HDataSource *dataS);
    HDataSource*   getDataSource             (void) const         {return fDataSource;        }
    void           setSecondDataSource       (HDataSource *dataS);
    void           setEnableCloseInput       (Bool_t flag = kTRUE){ enableCloseInput = flag;  }
    HDataSource*   getSecondDataSource       (void) const         { return fSecondDataSource; }
    //----------------------------------------------
    // setup tasks
    HTask*         getTask                   (Char_t *taskName);
    HTaskSet*      getTask                   (void);
    HTaskSet*      getTaskSet                (Int_t nEvtId);
    HTaskSet*      getTaskSet                (Char_t *evtTypeName);
    Bool_t         initTasks                 ();
    Bool_t         reinitTasks               ();
    Bool_t         finalizeTasks             ();
    //----------------------------------------------
    // output file handling
    void           closeOutput               ();
    void           setOutputSizeLimit        (Size_t l)           { fOutputSizeLimit = l; }
    void           setSplitLevel             (Int_t splitLevel);
    Int_t          getSplitLevel             (void);
    Int_t          getTreeBufferSize         ()                   { return treeBufferSize; }
    void           setTreeBufferSize         (Int_t size)         { treeBufferSize = size; }
    Bool_t         setOutputFile             (Text_t *name,Option_t *opt,Text_t *title,Int_t comp);
    TFile*         getOutputFile             ()                   { return fOutputFile;    }
    // setup hld file output
    Bool_t         setHldOutput              (Text_t*,Text_t* fileSuffix="f_",Option_t* option="NEW");
    HldFileOutput* getHldOutput              (void) const         { return fHldOutput;     }
    //----------------------------------------------
    // setup tree + browser
    HTree*         getTree                   (void);
    Bool_t         makeTree                  (void);
    void           activateTree              (TTree *tree);
    Bool_t         IsFolder                  (void) const;
    void           Browse                    (TBrowser *b);
    Int_t          setAlgorithmLayout        (Text_t *fileName);
    Int_t          setEventLayout            (Text_t *fileName);
    Int_t          setConfig                 (Text_t *fileName);
    //----------------------------------------------
    // mapping id -> tasks sets
    Int_t          mapId                     (Int_t id);
    Bool_t         isCalibration             ();
    Bool_t         isReal                    ();
    void           printDefinedTaskSets      ();
    void           defineTaskSets            ();

    ClassDef(Hades,1) //Control class for Hades reconstruction
};


#endif // !HADES
