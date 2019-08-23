#include "hades.h"
#include <iostream.h>
#include <TClass.h>
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TInterpreter.h>
#include <TDatime.h>
#include "htree.h"
#include "hdebug.h"
#include "hruntimedb.h"
#include "htaskset.h"
#include "hspectrometer.h"
#include "hdataobject.h"
#include "hdatasource.h"
#include "heventheader.h"
#include "hevent.h"
#include <string.h>
#include <TSystem.h>
#include "hrecevent.h"
#include "hmessagemgr.h"

//*-- Author : Manuel Sanchez
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
//Hades Begin_Html <a href="Hades_class.gif">(UML Diagram)</a> End_Html
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

ClassImp(Hades)

Hades::Hades(void) {
  //Default constructor for Hades. It sets the global pointer gHades.
  //This constructor also creates one HDebug object if none exists.
  gHades=this;
  if (!gDebuger) gDebuger=new HDebug;

  fCurrentEvent=NULL;
  fDataSource=NULL;
  fOutputFile=NULL;
  fSplitLevel=2;
  fTree=NULL;
  fCounter = 0;
  rtdb=HRuntimeDb::instance();
  setup=new HSpectrometer();
  gROOT->GetListOfBrowsables()->Add(this,"HYDRA");
  fTaskList = new TObjArray(10);

  //Adding according to event triggers id  
  fTask=new HTaskSet("RealEvents","Main task for real events");
  fTaskList->AddAt(fTask, kRealEvent);
  fTaskList->AddAt(new HTaskSet("SimulationEvents",
                   "Main task for simulation events"), kSimulationEvent);
  fTaskList->AddAt(new HTaskSet("CalibrationEvents", 
                   "Main task for calibration events"), kCalibrationEventLow); 
  
  quiet = kFALSE;
  fFirstEventLoop = kTRUE;
  fCycleNumber = 0;
  fOutputSizeLimit = 1900000000;

  // Adding new messsage handler
  oldHdlr = GetErrorHandler();
  msgHandler = new HMessageMgr("Hades","Hades");
  SetErrorHandler(&msgHandler->hydraErrorHandler);
}


Hades::~Hades(void) {
  //Closes the reconstruction program. If there are unsaved data, it saves them
  //When using Hades within a macro don't forget to call the destructor, 
  //or data won't be saved.
  closeOutput();
  if (fTree) delete fTree;
  if (fCurrentEvent) delete fCurrentEvent;
  if (fDataSource) delete fDataSource;
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
  //macro all the information to run the analisys is given. This is an 
  //alternative way of initialization without using setEventLayout(), 
  //setAlgorithmLayout()...
  //
  // For more information on initialization see the initialization section in
  // the HYDRA manual.
  //
  // Within this macro the user is responsible of calling the Hades::init() 
  //function before the call to Hades::makeTree();

  //Antes de nada hay que hacer limpieza. (Esto falta)
  //#warning "add cleanup to Hades::setConfig..."
  //FIXME: add cleanup to Hades::setConfig...

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
  
  if (!initTasks()) return kFALSE;
  return kTRUE;
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

HTaskSet* Hades::getTask() {
  return getTaskSet(kRealEvent);
}

HTask* Hades::getTask(Char_t *name) {   // find task by name
  HTaskSet* taskSet;
  HTask* task=NULL;
  if ( (taskSet=(HTaskSet*)fTaskList->At(kSimulationEvent)) ) {
     if ( (task=taskSet->getTask(name)) ) return task;
  }
  if ( (taskSet=(HTaskSet*)fTaskList->At(kRealEvent)) ) {
     if ( (task=taskSet->getTask(name)) ) return task;
  }
  if ( (taskSet=(HTaskSet*)fTaskList->At(kCalibrationEventLow)) ) {
     if ( (task=taskSet->getTask(name)) ) return task;
  }
  return NULL;   // not found in any of the sets
}

HTaskSet* Hades::getTaskSet(Int_t nEvtId) {
  if (nEvtId == kSimulationEvent) 
            return (HTaskSet*)fTaskList->At(kSimulationEvent);
  if (nEvtId == kRealEvent)
            return (HTaskSet*)fTaskList->At(kRealEvent);
  if (nEvtId >= kCalibrationEventLow && nEvtId <= kCalibrationEventHigh)
            return (HTaskSet*)fTaskList->At(kCalibrationEventLow);
  return NULL;
}

HTaskSet* Hades::getTaskSet(Char_t *evtTypeName) {
  if (strcmp(evtTypeName, "simulation")==0)
      return getTaskSet(kSimulationEvent);
  if (strcmp(evtTypeName, "real")==0)
      return getTaskSet(kRealEvent);
  if (strcmp(evtTypeName, "calibration")==0)
      return getTaskSet(kCalibrationEventLow);
 
  return NULL;
}




HEvent *&Hades::getCurrentEvent(void) {
  //Returns a pointer to the event in reconstruction
  return fCurrentEvent;
}

void Hades::setEvent(HEvent *ev) {
   //This function sets the event layout to that given by "ev". The user first
   //builds the event layout by creating an HEvent object and then call this
   //function; which sets the data member fCurrentEvent.
   //
   //This function is tipically called from the macro invoked in 
   //setEventLayout()
   if (fCurrentEvent) delete fCurrentEvent;
   fCurrentEvent=ev;
}

void Hades::setDataSource(HDataSource *dataS) {
  // Method used to establish the data source where data are taken from. For 
  //that purpose an object of a class derived from HDataSource is created and 
  //then the setDataSource() method is called giving a pointer to this object 
  //as a parameter
  //
  //  This function should be called typically from the macro invoked with 
  //setConfig()
   fDataSource=dataS;
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
  //in tree which correspond to the branches which would eventually be 
  //generated for the output Root tree of events if the function makeTree() is
  //called.
  //
  // This mechanism allows to only read those branches in "tree" which are 
  //neccesary to fill an event as defined in setEventLayout()
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
    tree->SetBranchStatus("*",0);
    if (splitLevel==0){
      //cout << "activating event" << endl;
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
  //Creates an output tree from the information in the event structure 
  //and according to the splitLevel (see setSplitLevel() )
   TBranch *b=NULL;
   Bool_t r=kFALSE;
   Text_t treeTitle[255];
   
   //gDirectory->GetFile()->Dump();
   if (fTree) delete fTree;

   sprintf(treeTitle,"T.%i",fSplitLevel);

   fOutputFile->cd(); //Make sure output file is current file
   
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
			 &fCurrentEvent,4000,99);
	//fTree->GetListOfBranches()->Add(b);
	fCurrentEvent->makeBranch(b);
      }
      if (b) {
         r=kTRUE;
      }
   }
   return r;
}

Int_t Hades::eventLoop(Int_t nEvents, Int_t firstEvent) {
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
#if DEBUG_LEVEL > 2
  gDebuger->enterFunc("Hades::eventLoop");
#endif
   Int_t evN=0;
   Int_t lastEvent=0;
   Int_t err=0;
   Bool_t initOk=kTRUE;
   EDsState dsCode=kDsOk;
   UInt_t evtId;
   HTaskSet *fEvTask;
   TDatime *dt = new TDatime();

   if (!(fCurrentEvent && fDataSource)) {
     Error("eventLoop","no current event or no data source set");
     return 0;
   }

   if (fCurrentEvent->InheritsFrom("HRecEvent")) { 
     //Ensure we have a monolithic streamer
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

   while ((evN<lastEvent) && (dsCode!=kDsEndData) && (dsCode!=kDsError)) {
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
	 Error("eventLoop","An error occurred in the data source reinit");
       while ((evN<lastEvent)) {
	 fCurrentEvent->Clear();

	 if (fOutputFile) { //save output if file is too big
	   if (fOutputFile->GetBytesWritten() > fOutputSizeLimit) {
	     recreateOutput();
	   }
	 }

	 if ((dsCode=fDataSource->getNextEvent())!=kDsOk) break;
	 evtId = fCurrentEvent->getHeader()->getId();
	 if (evtId < (UInt_t)fTaskList->GetSize()) {
	   fEvTask = getTaskSet(evtId);
	   
	   if (fEvTask) {
	     fEvTask->next(err);
	     
	     if(err!=0 ) {
	       if (err != kSkipEvent) {
		 if(!quiet) Warning("eventLoop",
				    "Exiting task-list with errCode:%i at event:%i \n",err,evN);
		 if (fTree) fTree->Fill();
	       }
	     } else {
	       if (fTree) fTree->Fill();
	     }
	   }else{ 
	     Warning("eventLoop"," no Tasks defined for EvtId:%i\n",evtId);
	   }
	 } else if (evtId==kStartEvent) {
	   if (fTree) fTree->Fill();
	 } else {
	   Warning("eventLoop","No tasks defined for event id: %i. Event skiped",
		   evtId);
	 }
	 evN++;
         if(fCounter && (evN%fCounter == 0) && !quiet) {
            dt->Set();
            printf(" %s  Events processed: %i\n",dt->AsSQLString() + 11, evN-firstEvent);
         }
       }
     } else dsCode = kDsEndData;
   }
   if(dsCode==kDsError) gDebuger->message("An error occurred in the event loop");
#if DEBUG_LEVEL > 2
   gDebuger->leaveFunc("Hades::eventLoop");
#endif
   fDataSource->finalize();
   if (!finalizeTasks()) Error("eventLoop","Unable to finalize");
   printf("\nTotal number of events processed: %i \n",evN-firstEvent);
   return evN-firstEvent;
}

Bool_t Hades::IsFolder(void) const {
  //Returns true. This tells the Root browser to show Hades as a folder
  //holding other objects
  return kTRUE;
}

void Hades::Browse(TBrowser *b) {
  //Used to browse the reconstructor's tree, a particular event or the 
  //reconstructor's histograms...
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



Hades *gHades;


