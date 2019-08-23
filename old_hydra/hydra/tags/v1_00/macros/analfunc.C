// This macro defines the "analyze" functions. 
//
// There are two analyze() functions.
//
//1.- analyze(Text_t *config,Int_t nEvents)
//  This function runs the reconstruction with the options specified in the 
//  configuration file "config". "nEvents" is an optional parameters indicating
//  the number of events to be analyzed (as default all events are analyzed).
//
//2.-Bool_t analyze(iFile,oFile,uFile,nEv,firstEvent);
//  This second analyze function is a handy way to proccess the data in an 
//  input file with a generic function and store the results in an output 
//  file. The meaning of the parameters is:
//
// Input:
//  iFile --> is the input file where the events' data are read from
//  oFile --> is the file where the output objects (histograms...) will be
//            stored.
//  uFile --> This is the file where the "User" class is defined. 
//            For each event in the input file,the User::execute(HEvent *event)
//            is called. This is where the user says what to do with the 
//            data in the input file (fill histograms ...)
//  nEv ----> This is an optional parameter specifying the number of events
//            to be processed. As default all the events in the input tree
//            are processed. If nEv is greater than the number of events in
//            the input tree; then it is reduced to the number of events in the
//            input tree.
//  firstEvent --> Optional parameter corresponding to the number of the first
//                 event to be processed. For example, if you want to process
//                 the events 5 to 20 then firstEvent=5 and nEv=15.



Bool_t analyze(Text_t *config,Int_t nEvents=0) {
  gROOT->Reset();
  Hades *myHades=NULL;
  TStopwatch timer;
  Int_t evN=0;

  myHades=new Hades;

  printf("Setting configuration...\n");
  gHades->setConfig(config);

  printf("Processing events...\n");
  timer.Reset();
  timer.Start();
  if (nEvents<1) {
    evN=gHades->eventLoop();
  } else {
    evN=gHades->eventLoop(nEvents);
  }

  myHades->getTaskSet("real")->printTimer();
  myHades->getTaskSet("simulation")->printTimer();
  
  delete myHades;
  timer.Stop();

  printf("------------------------------------------------------\n");
  printf("Events processed: %i\n",evN);
  printf("Real time: %f\n",timer.RealTime());
  printf("Cpu time: %f\n",timer.CpuTime());
  if (evN) printf("Performance: %f s/ev\n",timer.CpuTime()/evN);
  
  return kTRUE;
}


Bool_t analyze(Text_t *iFile,Text_t *oFile,Text_t *uFile,Int_t nEv=0,Int_t firstEvent=0) {
  gROOT->Reset();
  Bool_t r=kTRUE;
  Bool_t split=kFALSE;
  Int_t nEvents=0;
  TStopwatch timer;
  TFile *input=new TFile(iFile);
  TTree *tree=input->Get("T");
  
  timer.Start();
  if (nEv==0) 
    nEvents=tree->GetEntries(); 
  else 
    nEvents=nEv;
  Char_t sl=*(strchr(tree->GetTitle(),'.')+1);
  switch (sl) {
  case '0' : split=kFALSE; break;
  case '1' : split=kTRUE; break;
  case '2' : split=kTRUE; break;
  default : split=kTRUE;
  }
  input->Close();
  delete input;
  if (split) {
    if (firstEvent>nEvents) firstEvent=nEvents;
    if (oFile && uFile) analizeSplit(iFile,oFile,uFile,nEvents,firstEvent);
    else r=kFALSE;
  } else {
    if (firstEvent>nEvents) firstEvent=nEvents;
    if (oFile && uFile) analizeNoSplit(iFile,oFile,uFile,nEvents,firstEvent);
    else r=kFALSE;
  }
  timer.Stop();

  printf("------------------------------------------------------\n");
  printf("Events processed: %i\n",nEvents);
  printf("Real time: %f\n",timer.RealTime());
  printf("Cpu time: %f\n",timer.CpuTime());
  printf("Performance: %f evs/sec\n",timer.CpuTime()/nEvents);
  
  return r;
}



void analyzeNoSplit(Text_t *inputFile,Text_t *outputFile,Text_t *userFile,Int_t nEv,Int_t firstEv)
{
// This macro is a general analysis frame for the case the output tree is
// stored with split level=0. Using this frame, the user doesn't need to worry
// about initialization stuff, event loop ...
//
// This macro opens the file named "inputFile" which
// is supossed to contain a Root tree (named T). 
//
// It also loads the macro "userFile" where the user is supossed to declare and
// implement a "User" class which must have an "execute(HEvent *event)" method.
//
// Once this is done; data in the tree are read and for each event the 
// User::execute() function is invoked so it can do its job.
//
// The macro also opens a file ("outputFile") where to store the information
// produced by the User class. An example user class is given in 
// Begin_Html <a href="./User.C.html">here</a>End_Html

// Setup the input file and tree
 TFile *input=new TFile(inputFile);
 TTree *tree=input->Get("T");
 HRecEvent *event=new HRecEvent;
 Int_t step=1;
 tree->SetBranchAddress("Event",&event);

// Setup output file
 TFile output(outputFile,"RECREATE","Analisys output file");

// Load user class in Begin_Html<a href="./User.C.html">User.C</a>End_Html
 gROOT->LoadMacro(userFile);
 User user;

 Int_t i=0;
 
 step=nEv/10;
 if (step==0) step=1;
// Loop on events
 for (i=firstEv;i<nEv;i++) {
   if (i%step==0) printf("Event nr=%i\n",i); 
   tree->GetEvent(i); // Read input event
   user.execute(event);    // Do whatever the user wants to
   event->clearAll(0); // Clear event
 }

// Write of output.
 output.Write();
 input->Close();
 delete input;
}

void analyzeSplit(Text_t *iFile,Text_t *oFile,Text_t *userFile,Int_t nEv,Int_t firstEv)
{
 Hades *myHades;
 HRootSource *rootS=new HRootSource;
 HEvent *eventS=NULL;
 Int_t step=1;

 TFile *file=new TFile(iFile);
 eventS=(HEvent *)file->Get("Event");
 delete file;
 
 myHades=new Hades;
 gHades->setEvent(eventS);

 if (!rootS->setInput(iFile,"T")) 
   printf("Error: file does not contain tree\n");;
 gHades->setDataSource(rootS);

 TFile output(oFile,"RECREATE","Analysis output file");
 gROOT->LoadMacro(userFile);
 User user;

 printf("Loop start\n");
 Int_t i=0;
 step=nEv*0.10;
 if (step==0) step=1;

 gHades->getCurrentEvent()->clearAll(rootS->getSplitLevel());
 for (i=firstEv;i<nEv;i++) {
  if (i%step==0) printf("Event nr=%i\n",i); 
  rootS->getNextEvent();
  user.execute(gHades->getCurrentEvent());
  gHades->getCurrentEvent()->clearAll(rootS->getSplitLevel());
 }
 printf("Loop end\n");
 output.Write();
 delete myHades;
}

TH2F *plot2D(TTree *tree,Text_t *varexp,Text_t *selexp1,Text_t *selexp2) {
  TTreeFormula *var,*sel1,*sel2;
  TH2F *hist;
  TArrayF x,y;

  //Compile variables
  if (strlen(selexp1)) {
    sel1=new TTreeFormula("Sel1",selexp1,tree);
    if (!sel1->GetNdim()) {delete sel1; sel1=0; return NULL;}
  }
  if (strlen(selexp2)) {
    sel2=new TTreeFormula("Sel2",selexp1,tree);
    if (!sel2->GetNdim()) {delete sel1;delete sel2; sel1=0; return NULL;}
  }  
  if (strlen(varexp)) {
    var==new TTreeFormula("Var",varexp,tree);
    if (!var->getNdim()) {delete var; delete sel1; delete sel2; return NULL;}
  }

  //Create default canvas
  if (!gPad && !gProofServ) {
    if (!gROOT->GetMakeDefCanvas()) return NULL;
    (gROOT->GetMakeDefCanvas)();
  }

  delete sel1;delete sel2; delete var;
  return NULL;
}


