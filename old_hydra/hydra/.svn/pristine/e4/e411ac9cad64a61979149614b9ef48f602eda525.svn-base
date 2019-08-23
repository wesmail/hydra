#include "hades.h"
#include <TClass.h>
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TInterpreter.h>
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
//*-- Author : Manuel Sanchez
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
//  See the initialization section in the HYDRA manual for more information.
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
  rtdb=new HRuntimeDb();
  setup=new HSpectrometer();
  gROOT->GetListOfBrowsables()->Add(this,"HYDRA");
  fTaskList = new TObjArray(5);

  //Adding according to event triggers id  
  fTask=new HTaskSet("RealEvents","Main task for real events");
  fTaskList->AddAt(fTask, kRealEvent);
  fTaskList->AddAt(new HTaskSet("SimulationEvents",
                   "Main task for simulation events"), kSimulationEvent);
  fTaskList->AddAt(new HTaskSet("CalibrationEvents", 
                   "Main task for calibration events"), kCalibrationEvent); 


}

// Hades::Hades(Text_t *title,
//              int *argc, char **argv) : TApplication(title,argc,argv) {
//   gHades=this;


//   fCurrentEvent=NULL;
//   fDataSource=NULL;
//   fOutputFile=NULL;
//   fSplitLevel=2;
// }

Hades::~Hades(void) {
  //Closes the reconstruction program. If there are unsaved data, it saves them
  //When using Hades within a macro don't forget to call the destructor, 
  //or data won't be saved.
  if (fOutputFile) {
    fOutputFile->cd();
    this->Write();
    //fTree->Print();
    fOutputFile->Write();
    delete fOutputFile;
    fOutputFile=NULL;
  } else {
    if (fTree) delete fTree;
  }
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
#warning "add cleanup to Hades::setConfig..."
  if (gROOT->Macro(fileName)) return kTRUE;
  return kFALSE;
}

Bool_t Hades::init(void) {
  // This function initializes Hades. This includes initialization of the
  // reconstructors, the data source and the detectors.

  setup->init();
  fDataSource->setEventAddress(&fCurrentEvent);
  if (!fDataSource->init()){  
     printf("\nError in fDataSource->init()\n");
     return kFALSE;
  }
  
  if (!initTasks()) return kFALSE;
  return rtdb->initContainers();
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

HTaskSet* Hades::getTaskSet(Int_t nEvtId) {
  return (HTaskSet*)fTaskList->At(nEvtId);
}

HTaskSet* Hades::getTaskSet(Char_t *evtTypeName) {
  if (strcmp(evtTypeName, "simulation")==0)
      return getTaskSet(kSimulationEvent);
  if (strcmp(evtTypeName, "real")==0)
      return getTaskSet(kRealEvent);
  if (strcmp(evtTypeName, "calibration")==0)
      return getTaskSet(kCalibrationEvent);
 
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
  //Method used to control the shape of the output Root tree for the events.
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
  if (fOutputFile) {
    this->Write();
    fOutputFile->Close();
    delete fOutputFile;
    fOutputFile=NULL;
  }
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
  Char_t sl=*(strchr(tree->GetTitle(),'.')+1);
  switch (sl) {
  case '0' : splitLevel=0; break;
  case '1' : splitLevel=1; break;
  case '2' : splitLevel=2; break;
  default : splitLevel=fSplitLevel;
  }
  if (fCurrentEvent) {
    if (splitLevel==0){
      //cout << "activating event" << endl;
	tree->SetBranchAddress("Event",&fCurrentEvent);
	tree->SetBranchStatus("Event",1);
     } else {
        tree->SetBranchAddress("Event",&fCurrentEvent);
	tree->SetBranchStatus("Event",1);
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
   
   fTree=new HTree("T",treeTitle);
   gTree=fTree;
   if (fCurrentEvent && fTree) {
      if (fSplitLevel==0)
	b=fTree->addBranch("Event",
			fCurrentEvent->ClassName(),
			&fCurrentEvent,64000,0);
      else {
	b=fTree->makeBranch("Event",
			 fCurrentEvent->ClassName(),
			 &fCurrentEvent,8000,1);
	fTree->GetListOfBranches()->Add(b);
	fCurrentEvent->makeBranch(b);
      }
      if (b) {
         r=kTRUE;
      }
   }
   return r;
}

Int_t Hades::eventLoop(Int_t nEvents, Int_t ) {
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
   Int_t lastEvent=evN+nEvents;
   Int_t err=0;
   EDsState dsCode=kDsOk;
   Int_t evtId;
   HTaskSet *fEvTask;
   


   if (!(fCurrentEvent && fDataSource)) {
     Error("Hades::eventLoop(...)","no current event or no data source set");
     return 0;
   }

   fCurrentEvent->Clear();
   // fDataSource->skipEvents(nStartEvent-1);  not yet implemented
   while ((evN<lastEvent) && (dsCode!=kDsEndData) && (dsCode!=kDsError)) {
     if(!(reinitTasks())) {
        gDebuger->message("An error occurred in the task list init");
        return evN;
     }
     while ((evN<lastEvent)) {
       fCurrentEvent->Clear();
       if ((dsCode=fDataSource->getNextEvent())!=kDsOk) break;
       evtId = fCurrentEvent->getHeader()->getId(); 
       fEvTask = getTaskSet(evtId);
       
       if (fEvTask) {
           fEvTask->next(err);
           if(err!=0)
              Error("Hades::EventLoop","Exiting task-list with errCode:%i 
                     at event:%i \n",err,evN);
           if (fTree) fTree->Fill();
       }else{ 
       Warning("Hades::EventLoop"," no Tasks defined for EvtId:%i\n",evtId);
       }              
       evN++;
     }
   }
   if(dsCode==kDsError) gDebuger->message("An error occurred in the event loop");
#if DEBUG_LEVEL > 2
   gDebuger->leaveFunc("Hades::eventLoop");
#endif
   fDataSource->finalize();
   finalizeTasks();
   printf("\nNumber of events processed:  %i \n",evN);
   return evN;
}

Bool_t Hades::IsFolder(void) {
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
  gDebuger->message("Adding setup");
#endif
  //if (setup) b->Add(setup,"Setup");
#if DEBUG_LEVEL>2
  gDebuger->message("Adding current event");
#endif
  if (fCurrentEvent) b->Add(fCurrentEvent,"Event");
#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("Hades::Browse");
#endif
}


void Hades::Streamer(TBuffer &R__b)
{
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> fSplitLevel;
      //R__b >> fDataSource;
      fDataSource=NULL;
      fOutputFile=NULL;
      //R__b >> fCurrentEvent;
      fCurrentEvent=(HEvent *)gDirectory->Get("Event");
      //R__b >> fRuntimeDb;
      fTree=(HTree *)gDirectory->Get("T");
   } else {
      R__b.WriteVersion(Hades::IsA());
      TObject::Streamer(R__b);
      R__b << fSplitLevel;
      //R__b << fDataSource;
      fCurrentEvent->Clear();
      //R__b << fCurrentEvent;
      fCurrentEvent->Write("Event");
      //R__b << fRuntimeDb;
      //fTree->Write(); The tree is automatically written in TFile::Write()
   }
}



Hades *gHades;





