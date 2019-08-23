// @(#)$Id: pid.C,v 1.1 2002-11-08 17:04:31 jaskula Exp $
//This macro defines the "analyze" functions.
//
//There are two analyze() functions.
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
//Input:
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

Bool_t ana(Int_t nEvents=0,Int_t startEvt =0) {

  gROOT->Reset();
  TStopwatch timer;
  Int_t evN=0;

  if(gHades == NULL)
      new Hades;

  printf("Setting configuration...\n");

//---------------  Set HPidParticleFiller (connected later to taks list)

HPidParticleFiller *pPartFiller = new HPidParticleFiller();

  //pPartFiller->setPdfCondition(1, 0.0, kTRUE);
  //pPartFiller->setCLCondition(100, 0.9, kTRUE);
  //pPartFiller->setCondition(100, 0.9, "<=", kTRUE, kFALSE);
  //pPartFiller->setCondition(1, 0.0, ">=", kTRUE, kTRUE);
  pPartFiller->setCondition(1, 0.0, ">=", kTRUE);

//---------------  Set batch (needed for TCanvas's) ------------------

  gROOT->SetBatch();

//-----------------------------------------------------------------
  TString inputDir = ".";
  TString inDir    = inputDir;
  TString outFile = "./particles.root";

//---------------  Parameter input file for real data ----------------

//------------ Do not write these categories to the Tree -------------

Cat_t notPersistentCat[] = {
       catMdcRaw, catShowerRaw, catStartRaw, catTofRaw, catTofinoRaw
       , catMdcCal1,catRichCal,catMdcCal2
       , catShowerCal
       , catMdcHit
       , catShowerHit, catShowerPID, catRichPID, catStartHit
       , catTofinoCal
       , catKickTrack, catKickMdcTrk,catPidTrackCand,catPidCandidate
};

// ------------ Set input data file: ------------------ --------------

  HRootSource *source=new HRootSource;
  source->setDirectory(inDir.Data());
  source->addFile("./xx01340013549_dst.root");
  gHades->setDataSource(source);

// ----------Add detectors to the setup: NO NEED TO CHANGE -----------

// no detectors needed on this level

//------------------------ Add PID Tasks -----------------------------

  gHades->getTaskSet("real")->add(pPartFiller);

//------------------------ Initialization ----------------------------

  printf("gHades->init()\n");

  gHades->makeCounter(1000);
  if(!gHades->init())
     printf("Error\n");

//----------------- Set not persistent categories --------------------

  HEvent *event = gHades->getCurrentEvent();
  for(Int_t i=0;i<sizeof(notPersistentCat)/sizeof(Cat_t);i++)
  {
  HCategory *hCat = event->getCategory(notPersistentCat[i]);

     if(hCat != NULL)
       hCat->setPersistency(kFALSE);
  }

//-------------------------- Set output ------------------------------

  gHades->setOutputFile(outFile.Data(),"RECREATE","Test",2);
  gHades->makeTree();

//--------------------------------------------------------------------

  printf("Processing events...\n");
  timer.Reset();
  timer.Start();
  if (nEvents<1) {
    evN=gHades->eventLoop();
  } else {
    evN=gHades->eventLoop(nEvents,startEvt);
  }

  gHades->getTaskSet("real")->printTimer();

  delete gHades;
  timer.Stop();

  printf("------------------------------------------------------\n");
  printf("Events processed: %i\n",evN);
  printf("Real time: %f\n",timer.RealTime());
  printf("Cpu time: %f\n",timer.CpuTime());
  if (evN) printf("Performance: %f s/ev\n",timer.CpuTime()/evN);

  return kTRUE;
}

