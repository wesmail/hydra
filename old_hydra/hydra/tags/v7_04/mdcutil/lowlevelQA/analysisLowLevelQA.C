Bool_t analysisLowLevelQA(Char_t* TinputDir=NULL, 
                          Char_t* ThldFile=NULL, 
                          Char_t* ToutDir=NULL, Int_t nEvents=50, Int_t startEvt =0) {

    TString inD(TinputDir);  //Only keep command line values
    TString outD(ToutDir);   //in case they are not empty
    TString hldF(ThldFile);  //we will use them later

    gROOT->Reset();
    //---------------  Set batch (needed for TCanvas's) ------------------
    gROOT->SetBatch();

    Hades *myHades=NULL;
    TStopwatch timer;
    Int_t evN=0;
    myHades=new Hades;

    printf("Setting configuration...\n");
    //comprobar si config existe


    //-----------------------------------------------------------------
    TString inputDir ="/misc/hadeb03/nov02/hld/329/";    //"/misc/haddisk3/nov01/hld/341/";
    TString outDir   ="/d/hades02/data/";    //"/misc/kempter/mydir/sim/";
    TString hldFile  ="be02329024313";  //"xx01340235419";  //   341 xx01341012253
    if(TinputDir != NULL){
       cout << "TinputDir != \"NULL\" "<< endl;
       inputDir=inD;
       cout << inputDir << endl;

    }
    cout << inputDir << endl;
    if(ToutDir   != NULL){
       cout << "ToutDir != \"NULL\" "<< endl;
       outDir=outD;
       cout << "outDir="<< outDir<< endl;
    }
    cout << "outDir="<< outDir<< endl;
    if(ThldFile  != NULL){
       cout << "ThldFile != \"NULL\" "<< endl;
       hldFile=hldF;
       cout << "hldFile="<< hldFile<< endl;
    }
    cout << "hldFile="<< hldFile<< endl;
    ////In case all of these are null we try to read from file
    ////if it exist, otherwise default walues will be used
    //if( (TinputDir==NULL) && (ToutDir==NULL) && (ThldFile==NULL) ){
    //  //read string by string from file
    //  Char_t ffString[300];
    //  FILE* settingsFile;
    //  settingsFile= fopen( "settings.txt" , "r");
    //  if( (Int_t) settingsFile != 0){
    //      cout << "file settings.txt exist -- reading from it!!!" << endl;
    //      //...Input.Directory...
    //      fscanf(settingsFile,"%s", ffString);
    //      printf("Input dir: %s\n", ffString);
    //      inputDir = ffString;
    //      //...File.Name...
    //      fscanf(settingsFile,"%s", ffString);
    //      //printf("hld-file: %s\n", ffString);
    //      hldFile = ffString;
    //      //...Output.Dir...
    //      fscanf(settingsFile,"%s", ffString);
    //      //printf("Output dir: %s\n", ffString);
    //      outDir = ffString;
    //      //...Number.of.Events...
    //      fscanf(settingsFile,"%ld", &nEvents);
    //      //printf("Number events to be analysed: %ld\n", nEvents);
    //      //...Begining.event...
    //      fscanf(settingsFile,"%ld", &startEvt);
    //      //printf("Begining event: %ld\n", startEvt);
    //      fclose(settingsFile);
    //   }else{
    //      cout << "file settings.txt does not -- taking hard coded values" << endl;
    //   }
    //}

    printf("\nInput dir: %s\n" , (Char_t*)inputDir);
    printf("hld-file: %s\n"  , (Char_t*)hldFile); 
    printf("Output dir: %s\n", (Char_t*)outDir);
    printf("Number  Events: %ld\n", (Char_t*)nEvents);
    printf("Begining Event: %ld\n\n", (Char_t*)startEvt);


    TString inFile   = hldFile+".hld";
    TString outFile  = outDir+"/"+hldFile+"_dst.root";
    TString outROOT  = outDir+"/"+hldFile+"_lowlevelqa.root";
    //-----------------------------------------------------------------

    cout<< outFile.Data()<<endl;
    cout<<inputDir.Data()<< inFile.Data()<<endl;

    //---------------  Parameter input file for real data ----------------
    //  TString rootParFile2 = "/d/hades/sanchez/official_pars/common.nov01ref_targdisp_031201.root";
    TString rootParFile2 = "/u/hadesdst/nov01/gen2/params/align_nov01_pre2.root";
    TString rootParFile1 = "/misc/kempter/Calibration/Nov2001/params_final/out/mdcNov01_341_130202_kick.root";

    //------------ Do not write these categories to the Tree -------------

    // ----------------  Unpacker configuration --------------------------

    Int_t mdcUnpackers[24] = { 201,202,203,204,205,206,207,208,209,
    210,211,212,213,214,215,216,217,218,219,220,221,222,223 };

    //----------------- Detector setup configuration ---------------------


    Int_t mdcMods[6][4] = { {1,1,1,1},
                            {1,1,0,0},
                            {1,1,1,0},
                            {1,1,1,1},
                            {1,1,0,0},
                            {1,1,1,0} };

    // ------------ Set input data file: NO NEED TO CHANGE  --------------
    HldFileSource *source=new HldFileSource;
    Int_t refrunID=1;

    //source->setDirectory("/misc/kempter/scratchlocal1/jochen/nov2001/341");
    source->setDirectory(inputDir.Data());
    //source->addFile(inFile.Data(),refrunID);
    source->addFile(inFile.Data());

    gHades->setDataSource(source);
    // ----------Add detectors to the setup: NO NEED TO CHANGE -----------

    HSpectrometer* spec = gHades->getSetup();

    spec->addDetector(new HMdcDetector);
    for (Int_t is=0; is<6; is++) {
	spec->getDetector("Mdc")->setModules(is,mdcMods[is]);
    }

    // -------------  RuntimeDb input: NO NEED TO CHANGE  ----------------

    HRuntimeDb* rtdb=gHades->getRuntimeDb();

    //HParRootFileIo *input1=new HParRootFileIo;
    //input1->open(rootParFile1.Data(),"READ");
    //rtdb->setSecondInput(input1);
    //HParRootFileIo *input2=new HParRootFileIo;
    //input2->open(rootParFile2.Data(),"READ");
    //rtdb->setFirstInput(input2);

    HParOraIo *input2=new HParOraIo;
    input2->open();
    rtdb->setFirstInput(input2);

    // ------------- Unpackers setup: NO NEED TO CHANGE ------------------
    HMdcUnpacker* mdcunp[24];
    for(Int_t i=0; mdcUnpackers[i]; i++){
	mdcunp[i]=new HMdcUnpacker(mdcUnpackers[i],kFALSE,kFALSE);
        mdcunp[i]->setQuietMode();
        mdcunp[i]->setDecodeVersion(1);
        source->addUnpacker( mdcunp[i] );
    }

    //************************ MDC *******************
    ////////////////////
    //  Options (Defaults in CAPITAL letters)
    //  -------------------------------------
    //      "real"(default) or "simulation" -- real data or simulation data
    //      "dubna"(default) or "santiago"  -- selects hit finder
    //
    //      "NoStartAndNoCal"(default)      -- HMdcCalibrater1 mode
    //      "NoStartAndCal"
    //      "StartAndNoCal"
    //
    //      "NoTimeCuts"(default)           -- time cuts in calibrater
    //      "TimeCuts"
    //
    //      "MagnetOn"(default)             -- Magnetic field (for Dubna
    //      "MagnetOff"                         hit-finding mode; see HMdcTrackFinder)
    //
    //      "CombinedClusters"(default)     -- Cluster-finding mode; 'chamber' means
    //      "ChamberClusters"                  clusters found for individual chambers
    //                                         (for Dubna hit-finder; see HMdcTrackFinder)
    //
    //      DUBNA FITTER: mdcTasks->HMdcTaskSet::setDubnaFitter(kTRUE,kFALSE,1); (NTuple,print,version)
    //
    HMdcTaskSet *mymdctasks=new HMdcTaskSet();

    //mymdctasks->setDubnaFitter(kTRUE,kFALSE,1);

    HTaskSet *mdcTasks    = mymdctasks->make("","real,cal1,magnetoff,chamberclusters,notimecuts,nostartandcal");//read cal parameters from file
    HMdcLowLevelQA *myqa=new HMdcLowLevelQA("myqa","myqa");
    //myqa->setOutputRoot("testlowlevelqa.root");
    myqa->setOutputRoot(outROOT.Data());
    myqa->setStepSize(5000);
    mdcTasks->add(myqa);

    //------------------------ Master task set ---------------------------
      gHades->getTaskSet("real")->add(mdcTasks);
      gHades->getTaskSet("real")->isTimed(kTRUE);

      gHades->makeCounter(1000);

      //------------------------ Initialization ----------------------------

      printf("gHades->init()\n");
      if (!gHades->init()) printf("Error\n");
      //-------------------------- Set output ------------------------------

      //gHades->setOutputFile(outFile.Data(),"RECREATE","Test",2);
      //gHades->makeTree();
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

      timer.Stop();

      printf("------------------------------------------------------\n");
      printf("Events processed: %i\n",evN);
      printf("Real time: %f\n",timer.RealTime());
      printf("Cpu time: %f\n",timer.CpuTime());
      if (evN) printf("Performance: %f s/ev\n",timer.CpuTime()/evN);

      rtdb->closeFirstInput();
      rtdb->closeSecondInput();

      delete myHades;
      return kTRUE;
}


