// File: analysisSimDST.C
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 02/10/31 11:20:16
//
Bool_t analysisSimDST(TString inputDir,TString inputFile,TString outputDir,TString paramSource="ORACLE",Int_t nEvents=100000,Int_t startEvt=0) {

  gROOT->Reset();
  Hades *myHades=NULL;
  TStopwatch timer;
  Int_t evN=0;

  myHades=new Hades;

//---------------  Set batch (needed for TCanvas's) ------------------

  gROOT->SetBatch();

//-----------------------------------------------------------------
  TString outputFile   = inputFile;
  outputFile.Remove(outputFile.Length()-5,outputFile.Length());
  outputFile = outputDir+"/"+outputFile+"_dst.root";

  TString outDirQA    = outputDir+"/QA";

//---------------  Reference run id -----------------------------
  Int_t refId = 0;
  if (inputFile.Contains("c1c")) refId = 4;
  if (inputFile.Contains("c2c")) refId = 3;
  if (refId==0) Warning("reference run for parameters not set properly");

//---------------  Parameter input file for simulated data ----------------
 
TString rootParFile = "./allParOracle@2002-10-31_11.10.root";

//------------ Do not write these categories to the Tree -------------

  Cat_t notPersistentCat[20] = {

       catMdcRaw, catShowerRaw, catStartRaw, catTofRaw, catTofinoRaw
        ,catMdcCal1,catRichCal,catMdcCal2,catShowerCal
      
  };

//----------------- Detector setup configuration ---------------------

  Int_t richMods[]   = {1};

  Int_t mdcMods[6][4] = { {0,1,1,1},
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
  
  HRootSource *source=new HRootSource;
  source->setDirectory(inputDir.Data());
  source->addFile(inputFile.Data());
  source->setRefId(source->getCurrentRunId(),refId);
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

//   spec->getDetector("Start")->setModules(-1,startMods);
//   spec->getDetector("TBox")->setModules(0,trigMods);

  for (Int_t is=0; is<6; is++) {
     spec->getDetector("Shower")->setModules(is,showerMods);
     spec->getDetector("Tof")->setModules(is,tofMods);
     spec->getDetector("Tofino")->setModules(is,tofinoMods[is]);
     spec->getDetector("Mdc")->setModules(is,mdcMods[is]);
     spec->getDetector("Rich")->setModules(is,richMods);
  }

// -------------  RuntimeDb input: NO NEED TO CHANGE  ----------------

  HRuntimeDb* rtdb=gHades->getRuntimeDb();

  // to init from ASCII file you need to change code by hand here
  if (paramSource.Contains("ORACLE"))
  {
      HParOraIo* input=new HParOraIo;
      input->open();
      rtdb->setFirstInput(input);
  }
  if (paramSource.Contains("ROOT"))
  {
      HParRootFileIo *input=new HParRootFileIo;
      input->open(rootParFile.Data(),"READ");
      if (!rtdb->getFirstInput()) rtdb->setFirstInput(input);
      else rtdb->setSecondInput(input);
  }

// -------------  RuntimeDb output: save the containers init from Oracle ----
  if (paramSource.Contains("ORACLE"))
  {
     HParRootFileIo* output=new HParRootFileIo;
     TString outputDir = "./";
     TString outputName = "allParamsFromOracle.3.root";
     output->open((outputDir+outputName).Data(),"RECREATE");
     rtdb->setOutput(output);
  }

// ----------- Build TASK SETS (using H***TaskSet::make) -------------
// ---- By default, the make() function returns the full task set ----
// ----- See the html documentation for the H***TaskSet options ------

//  ----------- START ----------------------------------------------------
//  --- there is no start detector simulation up to now ------------------ 
//HTask *startTasks         = HStartTaskSet::make();
//  ----------- RICH -----------------------------------------------------     
  HTask *richTasks          = HRichTaskSet::make("simulation","noiseon");
  HTask *richIPUTasks       = HRichIPUTaskSet::make("simulation");
//  ----------- SHOWER ---------------------------------------------------
  HTask *showerTasks        = HShowerTaskSet::make("","simulation,
                                                       leprecognition");
//  ----------- TOF ------------------------------------------------------
  HTask *tofTasks           = HTofTaskSet::make("simulation");
//  ----------- TOFINO ---------------------------------------------------
  HTask *tofinoTasks        = HTofinoTaskSet::make("simulation","simulation");
//  ----------- SHOWERTOFINO ---------------------------------------------
  HTask *showerTofinoTasks  = HShowerTofinoTaskSet::make("","simulation,
                                                             leprecognition");
//  ----------- MDC ------------------------------------------------------
// task set setup (option flags) done from param src 
  HMdcTaskSet *mdctasks     = new HMdcTaskSet();
  HTaskSet *mdcTasks        = mdctasks->make("rtdb","");
//  --- special
  mdcTasks->add(new HMdcVertexFind("vertexfinder","vertexfinder",0,kFALSE)); 
//  ---------- KICK -----------------------------------------------------
  HTask *kickTasks   = HKickTaskSet::make("","simulation,lowres,tofclust");
//  ---------- EVT FILTER -----------------------------------------------
//  simulate 1st lvl trigger, mult of tof + tofino hits
  HTaskSet *evtFilterTasks = new HTaskSet("EvtFilter",
					  "List of Evt Filter Tasks");
  evtFilterTasks->connect( new HRichEvtFilter("Tof+Tofino_Mult",
					      "Tof+Tofino_Mult","tofmult",4));

//----------------------- Quality Assessment -------------------------

  HQAMaker *qaMaker = new HQAMaker("qa.maker","qa.maker");
   qaMaker->setOutputDir(outDirQA.Data());
   qaMaker->setSamplingRate(5);
   qaMaker->setIntervalSize(5000);

//------------------------ Master task set ---------------------------

  HTaskSet *masterTaskSet = gHades->getTaskSet("simulation");

  gHades->getTaskSet("simulation")->add(tofTasks);
  gHades->getTaskSet("simulation")->add(tofinoTasks);
  gHades->getTaskSet("simulation")->add(evtFilterTasks);
  gHades->getTaskSet("simulation")->add(richTasks);
  gHades->getTaskSet("simulation")->add(richIPUTasks);
  gHades->getTaskSet("simulation")->add(mdcTasks);
  gHades->getTaskSet("simulation")->add(showerTasks);
  gHades->getTaskSet("simulation")->add(kickTasks);
  gHades->getTaskSet("simulation")->add(showerTofinoTasks);
  gHades->getTaskSet("simulation")->add(qaMaker);

  gHades->getTaskSet("simulation")->isTimed(kTRUE);
  gHades->makeCounter(1000);

//------------------------ Initialization ----------------------------

  printf("gHades->init()\n");
  if (!gHades->init()) printf("Error\n");

//----------------- Set non-persistent categories --------------------

  HEvent *event = gHades->getCurrentEvent();
  // for(Int_t i=0;i<20;i++)
  // event->getCategory(notPersistentCat[i])->setPersistency(kFALSE);

//-------------------------- Set output ------------------------------

  gHades->setOutputFile(outputFile.Data(),"RECREATE","Test",2);
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

  if (paramSource.Contains("ORACLE"))
  {
    if (!rtdb->initContainers(refId))
	Error("analysisSimDST.C","RTDB not initialized for ref run");
    rtdb->writeContainers();
    rtdb->setContainersStatic();
    rtdb->saveOutput();
    rtdb->print();
  }  

  myHades->getTaskSet("simulation")->printTimer();
  
  delete myHades;
  timer.Stop();

  printf("------------------------------------------------------\n");
  printf("Events processed: %i\n",evN);
  printf("Real time: %f\n",timer.RealTime());
  printf("Cpu time: %f\n",timer.CpuTime());
  if (evN) printf("Performance: %f s/ev\n",timer.CpuTime()/evN);
  if (evN) printf("Performance (real): %f ev/s\n",evN/timer.RealTime());
  return kTRUE;
}

