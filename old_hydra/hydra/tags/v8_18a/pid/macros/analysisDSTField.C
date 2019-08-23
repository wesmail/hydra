
Bool_t analysisDSTField(TString inputFile = "xx01340013549",Int_t nEvents=0,Int_t startEvt =0) {

  // gROOT->Reset();
  Hades *myHades=NULL;
  TStopwatch timer;
  Int_t evN=0;

  myHades=new Hades;

  printf("Setting configuration...\n");
  //comprobar si config existe

//---------------  Set batch (needed for TCanvas's) ------------------

  gROOT->SetBatch();

  //-----------------------------------------------------------------
  //TString inputDir = "/tmp";
  //TString inputDir = "/d/hades03/data/nov01/hld";
  TString inputDir = "/s/jaskula";
  //TString outDir = "/tmp/";
  TString outDir = "./";
  TString hldFile   = inputFile;
  TString inFile = hldFile+".hld";

  TString inDir    = inputDir;
  TString outFile = outDir+hldFile+"_dst.root";

//------------ Do not write these categories to the Tree -------------

  Cat_t notPersistentCat[] = {
       catMdcRaw, catShowerRaw, catTofRaw, catTofinoRaw
       , catMdcCal1,catRichCal,catMdcCal2
       , catShowerCal
       , catMdcHit
       , catShowerHit, catShowerPID, catRichPID
       , catTofinoCal
       , catMdcHit, catMdcSeg, catRichHit, catTofHit, catStartCal
       , catShowerTofPID, catShowerHitTof, catTofCluster
       , catMdcClusInf, catRichHitHdr, catShowerHitHdr
       , catStartHit, catStartRaw
       //, catRichRaw, catRichHitIPU
  };

// ----------------  Unpacker configuration --------------------------

  Int_t richUnpackers[] = { 100,101,102,103,104,105,
                              106,107,108,109,110,111, 0 };

  Int_t mdcUnpackers[20] = {202,203,204,205,206,207,208,209,
                             210,211,212,213,214,215,216,217, 0 };

  Int_t showerUnpackers[] = { 300,301,302,303,304,305,
                                306,307,308,309,310,311, 0 };

  Int_t tofUnpackers[]    = { 411,412,413, 0 };
  Int_t tofinoUnpackers[] = { 412, 0 };
  Int_t startUnpackers[]  = { 412, 0 };

//----------------- Detector setup configuration ---------------------

  Int_t richMods[1]   = {1};

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
  HldFileSource *source=new HldFileSource;
  source->setDirectory(inDir.Data());
  source->addFile(inFile.Data());//,1007775913);
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

  HParRootFileIo* input1=new HParRootFileIo;
  input1->open("/u/jaskula/hydra_dev/pidparams.root");
  rtdb->setFirstInput(input1);

  HParOraIo *ora=new HParOraIo() ;
  ora->open();
   rtdb->setSecondInput(ora);

// ------------- Unpackers setup: NO NEED TO CHANGE ------------------

#if 0
  for(Int_t i=0; richUnpackers[i]; i++){
     source->addUnpacker( new HRichUnpackerCal99(richUnpackers[i]) );
     source->addUnpacker( new HRichUnpackerRaw99(richUnpackers[i]) );
  }
#endif

  for(Int_t i=0; mdcUnpackers[i]; i++)
  {
     printf("Setting mdcUnpacker: %d %d\n", i, mdcUnpackers[i]);
     source->addUnpacker( new HMdcUnpacker(mdcUnpackers[i],kFALSE) );
  }

  for(Int_t i=0; showerUnpackers[i]; i++)
     source->addUnpacker( new HShowerUnpacker(showerUnpackers[i]) );

  for(Int_t i=0; tofUnpackers[i]; i++)
     source->addUnpacker( new HTofUnpacker(tofUnpackers[i]) );

  for(Int_t i=0; tofinoUnpackers[i]; i++)
     source->addUnpacker( new HTofinoUnpacker(tofinoUnpackers[i]) );

  for(Int_t i=0; startUnpackers[i]; i++)
     source->addUnpacker( new HStartUnpacker(startUnpackers[i]) );

// ----------- Build task sets (using H***TaskSet::make) -------------
// ---- By default, the make() function returns the full task set ----
// ----- See the html documentation for the H***TaskSet options ------

  HTask *startTasks  = HStartTaskSet::make();
  //HTask *richTasks   = HRichTaskSet::make("real","");
  //HTask *richIPUTasks   = HRichIPUTaskSet::make("real");
  HTask *showerTasks = HShowerTaskSet::make();
  HTask *tofTasks    = HTofTaskSet::make("real");
  HTask *tofinoTasks = HTofinoTaskSet::make();
  HTask * showerTofinoTasks =HShowerTofinoTaskSet::make();

  HMdcTaskSet *mymdctasks=new HMdcTaskSet();

  // HTaskSet *mdcTasks    = mymdctasks->make("rtdb","cal1");
  // HTaskSet *mdcTasks    = mymdctasks->make("","nostartandcal,cal1");

  HTaskSet *mdcTasks    = mymdctasks->make("rtdb","");

  mdcTasks->add(new HMdcVertexFind("vertexfinder","vertexfinder",0,kFALSE));
   // Setting for the tracking :
  //  trackfinder  (segments)
  //  dubna fitter (fit segments)
  HTask *kickTasks   = HKickTaskSet::make("","lowres,debug,tofclust");

//----------------------- Quality Assessment -------------------------
#if 0
  HQAMaker *qaMaker = new HQAMaker("qa.maker","qa.maker");
  qaMaker->setOutputDir(outDirQA.Data());
  qaMaker->setSamplingRate(5);
  qaMaker->setIntervalSize(5000);
#endif

//------------------------ Master task set ---------------------------

  HTaskSet *masterTaskSet = gHades->getTaskSet("real");

  gHades->getTaskSet("real")->add(startTasks);
  gHades->getTaskSet("real")->add(tofTasks);
  gHades->getTaskSet("real")->add(tofinoTasks);
  //gHades->getTaskSet("real")->add(richTasks);
  //gHades->getTaskSet("real")->add(richIPUTasks);
  gHades->getTaskSet("real")->add(showerTasks);
  gHades->getTaskSet("real")->add(mdcTasks);
  gHades->getTaskSet("real")->add(kickTasks);
  gHades->getTaskSet("real")->add(showerTofinoTasks);
  //gHades->getTaskSet("real")->add(qaMaker);

#if 1
  gHades->getTaskSet("real")->add(new HPidTrackFiller());

HPidReconstructor *pPidRec = new HPidReconstructor("pdf,CL,merge,debug0");
  pPidRec->setDefaultParticleIds();
  //pPidRec->addAlgorithm(new HPidAlgRich());
  //pPidRec->addAlgorithm(new HPidAlgShowerTof());
  pPidRec->addAlgorithm(new HPidAlgMomVsBeta());

  gHades->getTaskSet("real")->add(pPidRec);
#endif

  gHades->getTaskSet("real")->isTimed(kTRUE);
  gHades->makeCounter(1000);

//------------------------ Initialization ----------------------------

  printf("gHades->init()\n");
  if (!gHades->init())
    printf("Error\n");

//----------------- Set not persistent categories --------------------

  HEvent *event = gHades->getCurrentEvent();
  for(Int_t i=0; i < sizeof(notPersistentCat) / sizeof(Cat_t);i++)
  {
  HCategory *pC = event->getCategory(notPersistentCat[i]);

     printf("### Seting notPersistentCat to %3d: ", notPersistentCat[i]);

     if(pC == NULL)
        printf("NO CATEGORY\n");
     else
     {
        printf("%s\n", pC->getClassName());
        event->getCategory(notPersistentCat[i])->setPersistency(kFALSE);
     }
  }

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


