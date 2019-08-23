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



Bool_t analysisDSTNoField(TString inputFile,Int_t nEvents=0,Int_t startEvt =0) {

  gROOT->Reset();
  Hades *myHades=NULL;
  TStopwatch timer;
  Int_t evN=0;

  myHades=new Hades;

  printf("Setting configuration...\n");
  //comprobar si config existe

//---------------  Set batch (needed for TCanvas's) ------------------

  gROOT->SetBatch();

  //-----------------------------------------------------------------
  TString inputDir = "/tmp";
  TString outDir = "/tmp/";
  TString hldFile   = inputFile;
  TString inFile = hldFile+".hld";

  TString inDir    = inputDir;
  TString outFile = outDir+hldFile+"_dst.root";

  TString outDirQA    = "/u/hadesdst/web-docs/nov01/gen2/";
//---------------  Parameter input file for real data ----------------
  TString rootParFile1 = "/u/hadesdst/nov01/gen2/params/align_nov01_pre2.root";
  TString rootParFile2 = "/u/hadesdst/nov01/gen2/params/allParamTest341.root";

//------------ Do not write these categories to the Tree -------------

  Cat_t notPersistentCat[20] = {

       catMdcRaw, catShowerRaw, catStartRaw, catTofRaw, catTofinoRaw
       // ,catMdcCal1,catRichCal,catMdcCal2,catShowerCal
      
  };

// ----------------  Unpacker configuration --------------------------

  Int_t richUnpackers[20] = { 100,101,102,103,104,105,
                              106,107,108,109,110,111 };

  Int_t mdcUnpackers[20] = { 201,202,203,204,205,206,207,208,209,
                             210,211,212,213,214,215,216,217 };

  Int_t showerUnpackers[20] = { 300,301,302,303,304,305,
                                306,307,308,309,310,311 };

  Int_t tofUnpackers[20]    = { 411,412,413 };
  Int_t tofinoUnpackers[20] = { 412 };
  Int_t startUnpackers[20]  = { 412 };

//----------------- Detector setup configuration ---------------------

  Int_t richMods[]   = {1};

  Int_t mdcMods[6][4] = { {1,1,1,1},
                          {1,1,0,0},
                          {1,1,1,0},
                          {1,1,1,0},
                          {1,1,0,0},
                          {1,1,1,0} };      

  Int_t tofMods[22] = {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

  Int_t showerMods[3]  = {1,2,3};
  Int_t tofinoMods[][] = { {1},{1},{1},{1},{1},{1} };
  Int_t startMods[2]   = {1,0};
  Int_t trigMods[]     = {1};

// ------------ Set input data file: NO NEED TO CHANGE  --------------
  Int_t runid = 1007756140;
  HldFileSource *source=new HldFileSource;
  source->setDirectory(inDir.Data());
  source->addFile(inFile.Data(),runid);
  gHades->setDataSource(source);

// ----------Add detectors to the setup: NO NEED TO CHANGE -----------

  HSpectrometer* spec = gHades->getSetup();

  spec->addDetector(new HRichDetector);
  spec->addDetector(new HMdcDetector);
  spec->addDetector(new HTofDetector);
  spec->addDetector(new HTofinoDetector);
  spec->addDetector(new HShowerDetector);
  spec->addDetector(new HStartDetector);
  spec->addDetector(new HTBoxDetector);

// ----- Set active modules for each detector: NO NEED TO CHANGE -----

  spec->getDetector("Start")->setModules(-1,startMods);
  spec->getDetector("TBox")->setModules(0,trigMods);

  for (Int_t is=0; is<6; is++) {
     spec->getDetector("Shower")->setModules(is,showerMods);
     spec->getDetector("Tof")->setModules(is,tofMods);
     spec->getDetector("Tofino")->setModules(is,tofinoMods[is]);
     spec->getDetector("Mdc")->setModules(is,mdcMods[is]);
     spec->getDetector("Rich")->setModules(is,richMods);
  }

// -------------  RuntimeDb input: NO NEED TO CHANGE  ----------------

  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  HParRootFileIo *input1=new HParRootFileIo;
  input1->open(rootParFile1.Data(),"READ");
  rtdb->setFirstInput(input1);

  HParRootFileIo *input2=new HParRootFileIo;
  input2->open(rootParFile2.Data(),"READ");
  rtdb->setSecondInput(input2);

// ------------- Unpackers setup: NO NEED TO CHANGE ------------------

  for(Int_t i=0; richUnpackers[i]; i++)
     source->addUnpacker( new HRichUnpackerCal99(richUnpackers[i]) );

  for(Int_t i=0; mdcUnpackers[i]; i++)
     source->addUnpacker( new HMdcUnpacker(mdcUnpackers[i],kFALSE) );

  for(Int_t i=0; showerUnpackers[i]; i++)
     source->addUnpacker( new HShowerUnpacker(showerUnpackers[i]) );

  for(Int_t i=0; tofUnpackers[i]; i++)
     source->addUnpacker( new HTofUnpacker(tofUnpackers[i]) );

  for(Int_t i=0; tofinoUnpackers[i]; i++)
     source->addUnpacker( new HTofinoUnpacker(tofinoUnpackers[i]) );

  for(Int_t i=0; startUnpackers[i]; i++)
     source->addUnpacker( new HStartUnpacker(startUnpackers[i]) );

  //source->addUnpacker( new HMatchUUnpacker(1,0));

// ----------- Build task sets (using H***TaskSet::make) -------------
// ---- By default, the make() function returns the full task set ----
// ----- See the html documentation for the H***TaskSet options ------

  HTask *startTasks  = HStartTaskSet::make();
  HTask *richTasks   = HRichTaskSet::make("real");
  HTask *showerTasks = HShowerTaskSet::make();
  HTask *tofTasks    = HTofTaskSet::make("real");
  HTask *tofinoTasks = HTofinoTaskSet::make();
  HTask * showerTofinoTasks =HShowerTofinoTaskSet::make();
      
  HTask *mdcTasks    = HMdcTaskSet::make("","nostartandcal,magnetoff,dubna,chamberclusters");//read cal parameters from file
  HTask *kickTasks   = HKickTaskSet::make("","lowres,debug");

//----------------------- Quality Assessment -------------------------

  HQAMaker *qaMaker = new HQAMaker("qa.maker","qa.maker");
  qaMaker->setOutputDir(outDirQA.Data());
  qaMaker->setSamplingRate(5);
  qaMaker->setIntervalSize(5000);

//------------------------ Master task set ---------------------------

  HTaskSet *masterTaskSet = gHades->getTaskSet("real");

  gHades->getTaskSet("real")->add(startTasks);
  gHades->getTaskSet("real")->add(tofTasks);
  gHades->getTaskSet("real")->add(tofinoTasks);
  gHades->getTaskSet("real")->add(richTasks);
  gHades->getTaskSet("real")->add(showerTasks);
  gHades->getTaskSet("real")->add(mdcTasks);
  gHades->getTaskSet("real")->add(kickTasks);
  gHades->getTaskSet("real")->add(showerTofinoTasks);
  gHades->getTaskSet("real")->add(qaMaker);

  gHades->getTaskSet("real")->isTimed(kTRUE);
  gHades->makeCounter(1000);

//------------------------ Initialization ----------------------------

  printf("gHades->init()\n");
  if (!gHades->init()) printf("Error\n");
 

  // Old means files before 31.08.2001 18:00
  //  gHades->getCurrentEvent()->getHeader()->getMUData()
  // ->setTriggerCodeFirstFormat(1);

//----------------- Set not persistent categories --------------------

  HEvent *event = gHades->getCurrentEvent();
  for(Int_t i=0;i<20;i++)
     event->getCategory(notPersistentCat[i])->setPersistency(kFALSE);

//-------------------------- Set output ------------------------------

  gHades->setOutputFile(outFile.Data(),"RECREATE","Test",2);
  gHades->makeTree();
  //------------------------------------------------------------------

  printf("Processing events...\n");
  timer.Reset();
  timer.Start();
  if (nEvents<1) {
    evN=gHades->eventLoop();
  } else {
    evN=gHades->eventLoop(nEvents,startEvt);
  }

  myHades->getTaskSet("real")->printTimer();
  
  delete myHades;
  timer.Stop();

  printf("------------------------------------------------------\n");
  printf("Events processed: %i\n",evN);
  printf("Real time: %f\n",timer.RealTime());
  printf("Cpu time: %f\n",timer.CpuTime());
  if (evN) printf("Performance: %f s/ev\n",timer.CpuTime()/evN);
  
  return kTRUE;
}


