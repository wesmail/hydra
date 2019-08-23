using namespace std;
#include <string.h>
#include <iostream> 
#include <iomanip>
#include <TClass.h>
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TInterpreter.h>
#include <TDatime.h>
#include <TSystem.h>
#include "hades.h"
#include "htree.h"
#include "hdebug.h"
#include "hruntimedb.h"
#include "htaskset.h"
#include "hspectrometer.h"
#include "hdataobject.h"
#include "hdatasource.h"
#include "heventheader.h"
#include "hevent.h"
#include "hrecevent.h"
#include "hmessagemgr.h"
#include "hldfileoutput.h"

//*-- Author : Manuel Sanchez
//*-- Modified : 09/02/2004 by R. Holzmann
//*-- Modified : 16/09/2003 by R. Holzmann
//*-- Modified : 21/02/2003 by I. Koenig
//*-- Modified : 29/10/2002 by R. Holzmann
//*-- Modified : 24/04/2002 by Joern Wuestenfeld
//*-- Modified : 07/01/2002 by Ilse Koenig
//*-- Modified : 23/11/2001 by Ilse Koenig
//*-- Modified : 10/05/2001 by R. Holzmann
//*-- Modified : 21/04/2001 by R. Holzmann
//*-- Modified : 15/02/2000 by Ilse Koenig
//*-- Modified : 03/03/2000 by Ilse Koenig
//*-- Modified : 01/02/2000 by Manuel Sanchez
//*-- Modified : 16/3/99 by R. Holzmann
//*-- Modified : 26/11/98 by Ilse Koenig
//*-- Modified : 3/11/98/ by Manuel Sanchez
//*-- Modified : 1/10/1998 by Manuel Sanchez
//*-- Modified : 27/05/98 12:01 by Manuel Sanchez
//*-- Copyright : GENP (Univ. Santiago de Comp.)
/////////////////////////////////////////
//  Hades Begin_Html <a href="Hades_class.gif">(UML Diagram)</a> End_Html
//
//  Hades is the main control class for the reconstruction program.
//
//  There must be one and only one object of this class in the program. This 
//  object will be accesible anywhere in the reconstruction program or the 
//  external macros through the global pointer gHades.
//
//  See the initialization section in the HYDRA manual for more information..
/////////////////////////////////////////////////////

R__EXTERN TTree *gTree;

Int_t Hades::EventCounter=0;

ClassImp(Hades)

Hades::Hades(void) {
  //Default constructor for Hades. It sets the global pointer gHades.
  //This constructor also creates one HDebug object if none exists.
  gHades=this;
  if (!gDebuger) gDebuger=new HDebug;

  fCurrentEvent=NULL;
  fDataSource=NULL;
  fSecondDataSource=NULL;
  fHldOutput=NULL;
  fOutputFile=NULL;
  fSplitLevel=2;
  fTree=NULL;
  fCounter = 0;
  rtdb=HRuntimeDb::instance();
  setup=new HSpectrometer();
  gROOT->GetListOfBrowsables()->Add(this,"HYDRA");

  defineTaskSets();

  quiet = 0;
  fFirstEventLoop = kTRUE;
  fCycleNumber = 0;
  fOutputSizeLimit = 1900000000;
  TTree::SetMaxTreeSize(2000000000); // set >fOutputSizeLimit to avoid conflict
                                     // with Root's TTree::Fill() size checking
  treeBufferSize=4000;
  // Adding new messsage handler
  oldHdlr = GetErrorHandler();
  msgHandler = new HMessageMgr("Hades","Hades");
  SetErrorHandler(&msgHandler->hydraErrorHandler);
  reqEvents = 0;
}


Hades::~Hades(void) {
  //Closes the reconstruction program. If there are unsaved data, it saves them
  //When using Hades within a macro don't forget to call the destructor, 
  //or data won't be saved.
  closeOutput();
  if (fTree) delete fTree;
  if (fCurrentEvent) delete fCurrentEvent;
  if (fHldOutput) delete fHldOutput;
  if (fDataSource) delete fDataSource;
  if (fSecondDataSource) delete fSecondDataSource;
  if (rtdb) delete rtdb;
  if (setup) delete setup;
  if (fTaskList) {
     fTaskList->Delete();
     delete fTaskList;
     fTaskList = NULL;
     fTask = NULL;
  }
  if(msgHandler) delete msgHandler;
  SetErrorHandler(oldHdlr);
  gHades=NULL;
}

void Hades::defineTaskSets() {  // set up empty task sets for all trigger types
  fTask=NULL;

  fTaskList = new TObjArray(19);
  fTaskList->AddAt(new HTaskSet("simulation",
                   "Main task for simulation events"), 0);

  fTaskList->AddAt(new HTaskSet("real1",
                   "Main task for real1 events"), 1);
  fTaskList->AddAt(new HTaskSet("real2",
                   "Main task for real2 events"), 2);
  fTaskList->AddAt(new HTaskSet("real3",
                   "Main task for real3 events"), 3);
  fTaskList->AddAt(new HTaskSet("real4",
                   "Main task for real4 events"), 4);
  fTaskList->AddAt(new HTaskSet("real5",
                   "Main task for real5 events"), 5);

  fTaskList->AddAt(new HTaskSet("special1",
                   "Main task for SPEC1 events"), 6);
  fTaskList->AddAt(new HTaskSet("offspill", 
                   "Main task for off-spill events"), 7); 
  fTaskList->AddAt(new HTaskSet("special3",
                   "Main task for SPEC3 events"), 8);
  fTaskList->AddAt(new HTaskSet("MDCcalibration", 
                   "Main task for MDC calibration events"), 9); 
  fTaskList->AddAt(new HTaskSet("special5",
                   "Main task for SPEC5 events"), 10);
  fTaskList->AddAt(new HTaskSet("illegal1",
                   "Main task for ILLEGAL1 events"), 11);
  fTaskList->AddAt(new HTaskSet("illegal2",
                   "Main task for ILLEGAL2 events"), 12);
  fTaskList->AddAt(new HTaskSet("beginrun",
                   "Main task for BEGIN RUN events"), 13);
  fTaskList->AddAt(new HTaskSet("endrun",
                   "Main task for END RUN events"), 14);
  fTaskList->AddAt(new HTaskSet("illegal5",
                   "Main task for ILLEGAL5 events"), 15);

  fTaskList->AddAt(new HTaskSet("real",
                   "Main task for all real events"), 16);
  fTaskList->AddAt(new HTaskSet("calibration", 
                   "Main task for all calibration events"), 17);
  fTaskList->AddAt(new HTaskSet("all",
                   "Main task for all events"), 18);
}

Int_t Hades::mapId(Int_t id) {
  if (id>=0 && id<MAXEVID) return evIdMap[id];
  else return -1;
}

Int_t Hades::setAlgorithmLayout(Text_t *fileName) {
  //This method is used to build the reconstructor's graph. For that purpose
  //the macro in the file "fileName" is executed.
  //
  //Returns 0 if successfull.
  return gROOT->Macro(fileName);
}

Int_t Hades::setEventLayout(Text_t *fileName) {
  //This method is intended to build the event's structure. For that purpose 
  //the macro in the file "fileName" is executed.
  //
  //Returns 0 if succesfull.
  if (fTree) delete fTree;
  fTree=NULL; 
  return gROOT->Macro(fileName);
}

Int_t Hades::setConfig(Text_t *fileName) {
  // This method is used to interpret a "configuration macro". Within this
  // macro all the information to run the analisys is given. This is an 
  // alternative way of initialization without using setEventLayout(), 
  // setAlgorithmLayout()...
  //
  // For more information on initialization see the initialization section in
  // the HYDRA manual.
  //
  // Within this macro the user is responsible of calling the Hades::init() 
  // function before the call to Hades::makeTree();

  if (gROOT->Macro(fileName)) return kTRUE;
  return kFALSE;
}

Bool_t Hades::init(void) {
  // This function initializes Hades. This includes initialization of the
  // reconstructors, the data source and the detectors.
  rtdb->initContainers(fDataSource->getCurrentRunId(),
	       fDataSource->getCurrentRefId());
  setup->init();
  fDataSource->setEventAddress(&fCurrentEvent);
  if (!fDataSource->init()){  
     printf("\nError in fDataSource->init()\n");
     return kFALSE;
  }
  if (fSecondDataSource) {
     fSecondDataSource->setEventAddress(&fCurrentEvent);
     if (!fSecondDataSource->init()){  
        printf("\nError in fSecondDataSource->init()\n");
        return kFALSE;
     }
  }
  Int_t vers=((fCurrentEvent->getHeader()->getVersion()));
  initTaskSetsIDs(vers);

  if (!initTasks()) return kFALSE;
  return kTRUE;
}

Bool_t Hades::isCalibration() { // is event a calibration event?
  HEventHeader *head = fCurrentEvent->getHeader(); 
  if (!head) return kFALSE;
  Int_t id = head->getId();
  Int_t version = head->getVersion() & 0x1;
  if (version==0) {
    if (id==5 || id==7) return kTRUE;
  }
  else if (version==1) {
    if (id==7 || id==9) return kTRUE;
  }
  return kFALSE;
}

Bool_t Hades::isReal() { // is event a real event?
  HEventHeader *head = fCurrentEvent->getHeader();
  if (!head) return kFALSE;
  Int_t id = head->getId();
  Int_t version = head->getVersion() & 0x1;
  if (version==0) {
    if (id==1) return kTRUE;
  }
  else if (version==1) {
    if (id==1 || id==2 || id==3 || id==4 || id==5) return kTRUE;
  }
  return kFALSE;
}

Bool_t  Hades::initTasks() {
  HTaskSet* task;
  TIter iter(fTaskList);
   
  while((task = (HTaskSet*)iter())) {
     if (!task->init())
        return kFALSE;
  }
  return kTRUE;
}

Bool_t Hades::reinitTasks() {
  HTaskSet* task;
  TIter iter(fTaskList);
   
  while((task = (HTaskSet*)iter())) {
     if (!task->reinit())
        return kFALSE;
  }
   
  return kTRUE;
}

Bool_t Hades::finalizeTasks() {
  HTaskSet* task;
  TIter iter(fTaskList);
   
  while((task = (HTaskSet*)iter())) {
     if (!task->finalize())
        return kFALSE;
  }
   
  return kTRUE;
}

HTaskSet* Hades::getTask() {  // returns default set (=real events)
  return getTaskSet("real");
}

HTask* Hades::getTask(Char_t *name) {   // search task by name in all sets
  HTaskSet* taskSet;
  HTask* task=NULL;
  for (Int_t i=0; i<fTaskList->GetSize(); i++) {
     if ( (taskSet=(HTaskSet*)fTaskList->At(i)) ) {
       if ( (task=taskSet->getTask(name)) ) return task;  // first occurence
     }
  }
  return NULL;   // not found in any of the sets
}

HTaskSet* Hades::getTaskSet(Int_t nEvtId) {  // find task set by event id

  Int_t ind = mapId(nEvtId);   // old/new mapping

  if (ind>=0 && ind<fTaskList->GetSize()) return (HTaskSet*)fTaskList->At(ind);
  else return NULL;
}

HTaskSet* Hades::getTaskSet(Char_t *evtTypeName) { // find task set by name

  HTaskSet* taskSet;
  for (Int_t i=0; i<fTaskList->GetSize(); i++) {
     if ( (taskSet=(HTaskSet*)fTaskList->At(i)) ) {
       if (strcmp(taskSet->GetName(),evtTypeName)==0) return taskSet;
     }
  }

  return NULL;
}


HEvent *&Hades::getCurrentEvent(void) {
  //Returns a pointer to the event in reconstruction
  return fCurrentEvent;
}

void Hades::setEvent(HEvent *ev) {
   // This function sets the event layout to that given by "ev". The user first
   // builds the event layout by creating an HEvent object and then call this
   // function; which sets the data member fCurrentEvent.
   //
   // This function is typically called from the macro invoked in 
   // setEventLayout()
   if (fCurrentEvent) delete fCurrentEvent;
   fCurrentEvent=ev;
}

void Hades::setDataSource(HDataSource *dataS) {
  // Method used to establish the data source where data are taken from. For 
  // that purpose an object of a class derived from HDataSource is created and 
  // the setDataSource() method is called to pass a pointer to this object 
  //
  //  This function should be called typically from the macro invoked with 
  //  setConfig()
   fDataSource=dataS;
}

void Hades::setSecondDataSource(HDataSource *dataS) {
  // Set second data source (needed e.g. for event merging).
  // Must be a Root source!
   if (dataS!=NULL) {
     if (strcmp(dataS->IsA()->GetName(),"HRootSource")==0) 
          fSecondDataSource=dataS;
     else fSecondDataSource=NULL;
   }
   else fSecondDataSource=NULL;
} 

Bool_t Hades::setHldOutput(Text_t* filedir,Text_t* fileSuffix,Option_t* option) {
  // Creates an HLD file output
  if (fHldOutput) delete fHldOutput;
  if (fDataSource && fDataSource->InheritsFrom("HldSource")) {
    fHldOutput=new HldFileOutput((HldSource*)fDataSource,filedir,fileSuffix,option);
    return kTRUE;
  } else {
    Error("setHldOutput","No HLD data source!");
    return kFALSE;
  } 
}

void Hades::setSplitLevel(Int_t splitLevel) {
  //Method used to control the shape of the output Root tree for the events..
  //Three split levels are supported:
  //
  //  splitLevel=0 ---> An only branch is created for the event, which is 
  //                    stored as a whole (i.e. the whole HEvent is stored as 
  //                    one piece).
  //
  //  splitLevel=1 ---> The partial events are stored as a whole, meanwhile the
  //                    top of the event class (tracks, header ...) is stored 
  //			creating one TBranch per data member
  //
  //  splitLevel=2 ---> One branch per data member is created (with little 
  //			exceptions, see Root automatic split rules). However
  //			the categories (see HCategory) still can decide how the
  //			split is done (by creating their own branches). This
  //                    is the default value set by the Hades constructor
  //
  fSplitLevel=splitLevel;
}

Int_t Hades::getSplitLevel(void) {
  //Returns the current splitLevel (see setSplitLevel() )
  return fSplitLevel;
}

Bool_t Hades::setOutputFile(Text_t *name,
			    Option_t *opt,
			    Text_t *title,
			    Int_t comp) {
  //Sets the output file, giving its name, compresion level...
  //For more information on the parameters see the constructor of TFile
  //
  //  This method allocates the file indicated taking care of saving the
  //current file if any. If the file does not exist or the user wants it to be 
  //overwritten, then opt="RECREATE"; if the file already exists and is to be 
  //updated then opt="UPDATE"...
  //
  fOutputFileName = name;
  fOutputFileName.Remove(fOutputFileName.Length()-4-1);
  closeOutput();
  fOutputFile=new TFile(name,opt,title,comp);
  return kTRUE;
}
   
HTree *Hades::getTree(void) {
  //Returns a pointer to the current output Root tree of events
  return fTree;
}

void Hades::activateTree(TTree *tree) {
  // Sets the right branch address and branch status (=1) for all the branches
  // in tree which correspond to the branches which would eventually be 
  // generated for the output Root tree of events if the function makeTree() is
  // called.
  //
  // This mechanism allows to only read those branches in "tree" which are 
  // neccesary to fill an event as defined in setEventLayout()
#if DEBUG_LEVEL>2
  gDebuger->enterFunc("Hades::activateTree");
#endif
  Int_t splitLevel=2;
  TBranch *brEvent = 0;

  Char_t sl=*(strchr(tree->GetTitle(),'.')+1);
  switch (sl) {
  case '0' : splitLevel=0; break;
  case '1' : splitLevel=1; break;
  case '2' : splitLevel=2; break;
  default : splitLevel=fSplitLevel;
  }
  if (fCurrentEvent) {
    tree->SetBranchStatus("*",1);   // needed since Root 303-04 because of
                                    // change in TBranchElement::SetAddress()
    if (splitLevel==0) {
      tree->SetBranchAddress("Event",&fCurrentEvent);
      tree->SetBranchStatus("Event",1);
    } else {
      brEvent = tree->GetBranch("Event.");
      if (brEvent) { //Post ROOT3 splitting
  	brEvent->SetAddress(&fCurrentEvent);
	tree->SetBranchStatus("Event.",1);
	tree->SetBranchStatus("Event.*",1);
      } else { //Pre ROOT3 splitting
        brEvent = tree->GetBranch("Event");
        if (brEvent) {
	  tree->SetBranchAddress("Event",&fCurrentEvent);
	  tree->SetBranchStatus("Event",1);
	  tree->SetBranchStatus("Event.*",1);
	} else {
	  Warning("activateBranch","Event branch not found in tree");
	}
      }
      fCurrentEvent->activateBranch(tree,splitLevel);
    }
  }
#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("Hades::activateTree");
#endif
}


Bool_t Hades::makeTree(void) {
  // Creates an output tree from the information in the event structure 
  // and according to the splitLevel (see setSplitLevel() )
   TBranch *b=NULL;
   Bool_t r=kFALSE;
   Text_t treeTitle[255];
   
   //gDirectory->GetFile()->Dump();
   if (fTree) delete fTree;

   sprintf(treeTitle,"T.%i",fSplitLevel);

   if (fOutputFile) fOutputFile->cd(); // Make sure output file is current file
   
   fTree=new HTree("T",treeTitle);
   gTree=fTree;
   if (fCurrentEvent && fTree) {
      if (fSplitLevel==0)
	b=fTree->Branch("Event",
			fCurrentEvent->ClassName(),
			&fCurrentEvent,64000,0);
      else {
	b=fTree->Branch("Event.",
			 fCurrentEvent->ClassName(),
			 &fCurrentEvent,getTreeBufferSize(),99);
	//fTree->GetListOfBranches()->Add(b);
	fCurrentEvent->makeBranch(b);
      }
      if (b) {
         r=kTRUE;
      }
   }
   return r;
}

Int_t Hades::eventLoop(Int_t nEvents, Int_t firstEvent, Double_t timeQuota) {
  //  Executes the event loop;
  //
  //  For each new event:
  //  First, the current event is cleared.
  //  Second, a new event is read from the data source (see HDataSource)
  //  Third, the reconstruction of this event is launched (see HReconstructor)
  //  Fourth, if a tree was created (see makeTree() ) then it is filled.
  //
  //  This function returns the number of events processed. A negative value
  //  corresponds to an error
  //
  //  timeQuota - time limit for tasks in minutes. 
  //  timeQuota <= 0. - no limit
  //  Event loop end condition: evN==nEvents or runing tasks time > timeQuota
#if DEBUG_LEVEL > 2
  gDebuger->enterFunc("Hades::eventLoop");
#endif
   Int_t evN=0;
   Int_t lastEvent=0;
   Int_t err=0;
   Bool_t initOk=kTRUE;
   EDsState dsCode=kDsOk;
   EDsState dsCode2=kDsOk;
   UInt_t evtId;
   TDatime dt;
   HTaskSet *taskSet;

   reqEvents = nEvents;
   if (!(fCurrentEvent && fDataSource)) {
     Error("eventLoop","No current event or no data source set!");
     return 0;
   }

   if (fCurrentEvent->InheritsFrom("HRecEvent")) { 
     // Ensure we have a monolithic streamer
     ( (HRecEvent *)fCurrentEvent)->setExpandedStreamer(kFALSE);
   }
   fCurrentEvent->Clear();
   if (fFirstEventLoop) {

     if (!rtdb->initContainers(fDataSource->getCurrentRunId(),
			       fDataSource->getCurrentRefId(),
			       fDataSource->getCurrentFileName())) return 0;
     if(!(reinitTasks())) {
	 gDebuger->message("An error occurred in the task list init");
	 return evN;
     }
     fFirstEventLoop = kFALSE;
   }

   dsCode = fDataSource->skipEvents(firstEvent);
   if (dsCode == kDsOk) {
     evN = firstEvent;
   } else {
     evN = firstEvent;
     Warning("eventLoop","Condition %i while skipping events",dsCode);
   }

   lastEvent = evN + nEvents;

   Bool_t timerFlag=kTRUE;
   fTimer.Reset();
   timeQuota*=60.;   // minutes -> seconds
   
   while ((evN<lastEvent) && (dsCode!=kDsEndData) && (dsCode!=kDsError) 
       && timerFlag) {
     if (dsCode == kDsEndFile) {
       initOk = rtdb->initContainers(fDataSource->getCurrentRunId(),
				     fDataSource->getCurrentRefId(),
				     fDataSource->getCurrentFileName());
       if (initOk) {
         if(!(reinitTasks())) {
           gDebuger->message("An error occurred in the task list init");
           return evN;
         }         
       }
     }

     if (initOk) {
       if (!fDataSource->reinit()) 
	 Error("eventLoop","An error occurred in fDataSource->reinit()");

       if (fSecondDataSource) {
         if (!fSecondDataSource->reinit())
           Error("eventLoop","An error occurred in fSecondDataSource->reinit()");
       }

       if (fHldOutput) {
         TString cf=fDataSource->getCurrentFileName();
         Int_t l=cf.Last('/');
         TString fn=cf(l+1,cf.Length()-l);
         fHldOutput->open(fn);
       }

       while ((evN<lastEvent) && timerFlag) {

	 EventCounter++;

	 fTimer.Start(kFALSE);
         
	 fCurrentEvent->Clear();

	 if (fOutputFile) { // save output if file is too big
	   if (fOutputFile->GetBytesWritten() > fOutputSizeLimit) {
	     recreateOutput();
	   }
	 }

//	 if ((dsCode=fDataSource->getNextEvent()) != kDsOk) break;

         if (fSecondDataSource) { // must be first to have header from 1st source
           dsCode2=fSecondDataSource->getNextEvent();
           if (dsCode2==kDsError) break;
           if (dsCode2==kDsEndData) fSecondDataSource->rewind(); //easy way out
	 }

	 if ((dsCode=fDataSource->getNextEvent()) != kDsOk) break;

	 err = 99;   // init error 
         UInt_t firstev=fCurrentEvent->getHeader()->getId();

	 if((firstev!=2&&EventCounter==1) || (EventCounter==2) )
	 {   // do it only once and
	     //if ev is not a begin run ev
	     // (kStartEvent=0xd seems to be wrong)
	     Int_t vers=((fCurrentEvent->getHeader()->getVersion()));
	     initTaskSetsIDs(vers);
	     printDefinedTaskSets();
	 }

	 getTaskSet("all")->next(err);  // allways execute this task set
         if (err != kSkipEvent) {

	   evtId = fCurrentEvent->getHeader()->getId();
	   if (mapId(evtId) != -1) {
             for (Int_t i=0; i<fTaskList->GetSize();i++) {
               if ( (taskSet=(HTaskSet*)fTaskList->At(i)) ) {
                  taskSet->next(err,evtId); // execute all task sets responsible for given evtId(s),
                  if (err < 0) break;       // but stop loop on first error/fSkipEvent
               }
             }
	     if(err!=0 ) {
	       if (err != kSkipEvent) { 
	         if(!quiet && err!=99) Warning("eventLoop",
	           "Exiting task set with errCode %i at event %i (Id %i)\n",err,evN,evtId);
	         if (fTree && err != 99) fTree->Fill();      // if no task executed, skip tree filling 
                 if (fHldOutput) fHldOutput->writeEvent();
	       }
	     } else {
               if (fTree) fTree->Fill();
               if (fHldOutput) fHldOutput->writeEvent();
             }
	   } else {
	     Warning("eventLoop","No tasklist defined for event id 0x%x.\n", evtId);
             if (fHldOutput) fHldOutput->writeEvent();
	   }
         }
	 evN++;
         if(fCounter && (evN%fCounter == 0) && !quiet) {
            dt.Set();
            printf(" %s  Events processed: %i\n",dt.AsSQLString() + 11, evN-firstEvent);
         }
         timerFlag=fTimer.RealTime() < timeQuota;
       }
     } else dsCode = kDsEndData;
   }
   if(dsCode==kDsError) gDebuger->message("An error occurred in the event loop");
#if DEBUG_LEVEL > 2
   gDebuger->leaveFunc("Hades::eventLoop");
#endif
   fDataSource->finalize();
   if (!finalizeTasks()) Error("eventLoop","Unable to finalize");
   if(!timerFlag) printf("\n%.2f minutes time quota has expired.\n",
     timeQuota/60.);
   if(quiet!=2) {
     printf("\nTotal number of events processed: %i \n",evN-firstEvent);
     if (fHldOutput) {
       printf("\n\nWritten to HLD Output: Total number of events:    %i \n",
              fHldOutput->getNumTotalEvt());
       printf("                       Number of filtered events: %i \n",
              fHldOutput->getNumFilteredEvt());
     }
   } 
   return evN-firstEvent;
}

Bool_t Hades::IsFolder(void) const {
  // Returns true. This tells the Root browser to show Hades as a folder
  // holding other objects
  return kTRUE;
}

void Hades::Browse(TBrowser *b) {
  // Used to browse the reconstructor's tree, a particular event or the 
  // reconstructor's histograms...
  //
  // This function is called by Root when browsing gHades with the Root browser
  //
#if DEBUG_LEVEL>2
  gDebuger->enterFunc("Hades::Browse");
#endif
  if (fTree) b->Add(fTree,"Tree");
  //b->Add(fHistBrowser);
#if DEBUG_LEVEL>2
  gDebuger->message("Adding Task list");
#endif
  if (fTaskList) b->Add(fTaskList,"Tasks");
#if DEBUG_LEVEL>2
  gDebuger->message("Adding current event");
#endif
  if (fCurrentEvent) b->Add(fCurrentEvent,"Event");
#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("Hades::Browse");
#endif
}

void Hades::closeOutput(void) {
  if (fOutputFile) {
    fOutputFile->cd();
    this->Write();
    fOutputFile->Write();
    delete fOutputFile;
    fOutputFile=NULL;
    fTree = 0;
  }  
}

void Hades::recreateOutput(void) {
  fCycleNumber++;
  Bool_t createTree = (fTree != 0);
  char name[255];
  TString opt(fOutputFile->GetOption());
  TString title(fOutputFile->GetTitle());
  Int_t comp = fOutputFile->GetCompressionLevel();

  closeOutput();
  sprintf(name,"%s_%i.root",fOutputFileName.Data(),fCycleNumber);
  
  fOutputFile = new TFile(name,"RECREATE",title.Data(),comp);
  if (createTree) makeTree();
    
}

void Hades::Streamer(TBuffer &R__b)
{
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> setup;
      rtdb->Streamer(R__b);
      R__b >> fSplitLevel;
      R__b >> fTaskList;
      fDataSource=NULL;
      fOutputFile=NULL;
      fCurrentEvent=(HEvent *)gDirectory->Get("Event");
      fTree=(HTree *)gDirectory->Get("T");
   } else {
      R__b.WriteVersion(Hades::IsA());
      TObject::Streamer(R__b);
      R__b << setup;
      rtdb->Streamer(R__b);
      R__b << fSplitLevel;
      R__b << fTaskList;
      if (fCurrentEvent) {
	if (fCurrentEvent->InheritsFrom("HRecEvent")) { //!!!!
	  Bool_t expand=((HRecEvent *)fCurrentEvent)->hasExpandedStreamer();
	  ( (HRecEvent *)fCurrentEvent)->setExpandedStreamer(kTRUE);
	  fCurrentEvent->Clear();
	  fCurrentEvent->Write("Event");
	  ( (HRecEvent *)fCurrentEvent)->setExpandedStreamer(expand);
	} else {
	  fCurrentEvent->Clear();
	  fCurrentEvent->Write("Event");
	}
	//fTree->Write(); The tree is automatically written in TFile::Write()
      }
   }
}

void Hades::printDefinedTaskSets()
{
    HTaskSet* task;
    TIter iter(fTaskList);
    Int_t size=0;
    Int_t* ids;

    Int_t version=((fCurrentEvent->getHeader()->getVersion())&0x1);
    cout<<"#################### HADES TASKSETS ############################"<<endl;
    if(version==0)cout<<"#  Version Nov02"<<endl;
    if(version==1)cout<<"#  Version Sep03"<<endl;

    while((task = (HTaskSet*)iter()))
    {
	ids=task->getIds(size);
	cout<<"#  "<<setw(15)<<task->GetName()<<" for Ids : "<<flush;
	for(Int_t i=0;i<size;i++)
	{
	    if(ids[0]==-1){cout<<"not defined"<<flush;break;}
	    if(ids[i]!=-1){cout<<ids[i]<<" "<<flush;}
	}
        cout<<""<<endl;
    }
    cout<<"################################################################"<<endl;
}
void  Hades::initTaskSetsIDs(Int_t version)
{
    // Mapping of event ids into HTaskSet numbers for DAQ type old (version=0),
    // i.e prior to Sep03, and new (version=1).
    //
    // This table will go in due time into an rtdb container.  (R.H.)
    //
    Int_t evIdTable[2][MAXEVID] = { { 0,16,17,17,17,17,17,17, /* Nov02  */
                                     17,17,-1,-1,-1,13,14,-1},
                                    { 0, 1, 2, 3, 4, 5, 6, 7, /* Sep03  */
                                      8, 9,10,-1,-1,13,14,-1}
                                   };

    for (Int_t i=0;i<MAXEVID;i++) evIdMap[i] = evIdTable[version][i];

    if (version==0) { // Nov02
	getTaskSet("simulation")->setIds(0);
	getTaskSet("real")->setIds(1);
	getTaskSet("calibration")->setIds(2,3,4,5,6,7,8,9);
	getTaskSet("beginrun")->setIds(13);
	getTaskSet("endrun")->setIds(14);
    }
    else if (version==1) { // Sep03
	getTaskSet("simulation")->setIds(0);
	getTaskSet("real")->setIds(1,2,3,4,5);
	getTaskSet("calibration")->setIds(7,9);
	getTaskSet("real1")->setIds(1);
	getTaskSet("real2")->setIds(2);
	getTaskSet("real3")->setIds(3);
	getTaskSet("real4")->setIds(4);
	getTaskSet("real5")->setIds(5);
	getTaskSet("special1")->setIds(6);
	getTaskSet("offspill")->setIds(7);
	getTaskSet("special3")->setIds(8);
	getTaskSet("MDCcalibration")->setIds(9);
	getTaskSet("special5")->setIds(10);
	getTaskSet("beginrun")->setIds(13);
	getTaskSet("endrun")->setIds(14);
    }
    else
    {
	Error("initTaskSetsIDs()","Unknow version %i ... exiting!",version);
        exit(EXIT_FAILURE);
    }

}

Hades *gHades;


