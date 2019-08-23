////////////////////////////////////////////////////////////////////
//Using:
//  root [0] .x target.C();
//  or
//  root [0] .x target.C("file_name");
//  or
//  root [0] .x target.C("file_name","inDir_name");
////////////////////////////////////////////////////////////////////
gROOT->Reset();
//=Input and Output files and directory=============================
Char_t* inDir = "/misc/lxhadesdaq/jan05";
    //"/misc/hadeb03/jan05";

Double_t timeLimit=25;    // time limit for run macro in minutes.
Int_t maxNumEv=10000;  // limit for number of events
Double_t timeRefresh=10;  // refresh canvas(es) each timeRefresh seconds 

//===Trigger selecting===================================================
//  UChar_t trMustHas=0b00000000; //0b00100100;
//  UChar_t trCanHas =0b11111111; //0b00000000;
  
//                  0b00000001 - Multiplicity 4                 
//                  0b00000010 - Minimum Bias (?)               
//                  0b00000100 - Multiplicity 2                 
//                  0b00001000 - Small Angle Scattering (Smash) 
//                  0b00010000 - Cosmics                        
//                  0b00100000 - Opposite Sector Coincidence    
//                  0b01000000 - Pulser (?)                     
//                  0b10000000 - ?   

Int_t refrunID = -1000; // run number used to initialize rtdb, 
                        // <-1 refrunId from file
Char_t* inFiles[]={"be05031211557.hld"
//    "tt04240184102.hld"
//    "co04237201125.hld"
//  "be04038035337.hld"
};

// Parameters input files:
// First input:
Char_t typeFile1='o';  // 'a'-ASCII, 'r'-root file, 'o'-oracle
Char_t* parFile1 = "testoraR3.txt";
//      "/u/mdc/lookupfiles/november2001/lookup_mdc_nov01.txt";

// Second input:
Char_t typeFile2='a';  // ='a'-ASCII or 'r'-root file; ='n' - first input only  
Char_t* parFile2 ="idealMdc_jan05_alex.txt";
//    "targPar3.txt";
// "/u/hadesdst/may01/gen1/params/params_MAY01_field_all_051001.root";

// List of modules:
Int_t mdcMods[6][4]=
//    {
//     {1,1,1,1},   // moduledef for sector 1       //  208 205 206
//     {1,1,1,1},   // moduledef for sector 2       //  209
//     {1,1,0,0},   // ...                          //  210
//     {1,1,1,0},                                   //  211 214
//     {1,1,1,1},                                   //  212
//     {1,1,1,0}                                    //  213
//   };
  {
    {0,0,0,0},   // moduledef for sector 1       //  208 205 206
    {1,1,0,0},   // moduledef for sector 2       //  209
    {0,0,0,0},   // ...                          //  210
    {0,0,0,0},                                   //  211 214
    {0,0,0,0},                                   //  212
    {0,0,0,0}                                    //  213
  };

// ----------------  Unpacker configuration --------------------------
// Int_t mdcUnpackers[6][4] = { {201,207,213,219},
//                              {202,208,214,220},
//                              {203,209,215,  0},
//                              {204,210,216,221},
//                              {205,211,217,222},
//                              {206,212,218,  0} };
Int_t mdcUnpackers[6][4] = { {201,207,213,219},
                             {202,208,214,220},
                             {203,209,215,221},
                             {204,210,216,222},
                             {205,211,217,223},
                             {206,212,218,224} };

Int_t trigMods[]       = {1};

//===Mdc time cuts=======================================================
// cutStatus=0 - HMdcCalibrater1, HMdcTrackFinder
//   and HMdcTargetMon will not do time cuts.
// cutStatus=1 - HMdcCalibrater1 will not do time cuts,
//   HMdcTrackFinder and HMdcEfficiency will do time cuts.
// cutStatus=2 - HMdcCalibrater1 will do time cuts,
//   HMdcTrackFinder and HMdcEfficiency will not do time cuts.
// sourceCutPar=0 - using param. of time cuts from parameters files or oracle
// sourceCutPar=1 - seting of cut parameters in this macro.
//
// For selecting and checking cuts look on the time distributions
// with out any cuts (cutStatus=0;).
// Attention! The cuts in this example of macros seted for non calibreted
// data (HMdcCalibrater1("MdcCali1","MdcCali1",3,0)). If you use
// the calibrated data you need to change of cut.
//
Int_t calibrater1Ver=3; // see class HMdcCalibrater1
Int_t cutStatus=0;
Int_t sourceCutPar=1;
// Float_t cutTime1L[4]={   0.,    0.,    0.,    0.};
// Float_t cutTime1R[4]={1000., 1000., 1700., 1800.};
// Float_t cutTime2L[4]={   0.,    0.,    0.,    0.};
// Float_t cutTime2R[4]={1000., 1000., 1700., 1800.};
// Float_t cutLeft  [4]={  20.,   20.,   20.,   20.};  // cutLeft < driftTime1-driftTime2 < cutRight
// Float_t cutRight [4]={ 350.,  350.,  600.,  700.};
Float_t cutTime1L[4]={ 700.,  700.,  250.,  300.};
Float_t cutTime1R[4]={1300., 1300., 1500., 1600.};
Float_t cutTime2L[4]={ 700.,  700.,  250.,  400.};
Float_t cutTime2R[4]={1400., 1400., 1800., 1800.};
Float_t cutLeft  [4]={  20.,   20.,   20.,   20.};  // cutLeft < driftTime1-driftTime2 < cutRight
Float_t cutRight [4]={ 300.,  300.,  500.,  500.};

Int_t level4s1=5;
Int_t level5s1=200; //40;
Int_t level4s2=5;
Int_t level5s2=200; //40;  
  
//=Parameters of track finder and efficiency tasks:=====================
  // isCoilOff=kTRUE(kFULSE) - The current in coil is  switched off(on).
Bool_t isCoilOff=kTRUE;
Int_t typeClustFinder=1; // =1,0 1-independent,

//======================================================================
HMdcTargetMon* targetMon;
//======================================================================

void targetMonOra(Char_t* fileI=0, Char_t* inDirI=0){

  Hades* myHades = new Hades();
  gHades->setQuietMode(); //(2); !!!!!!!!!!!!!!!!
  Int_t splitLevel=2;
  gHades->setSplitLevel(splitLevel);
  HSpectrometer* spec=gHades->getSetup();
  
  // Trigger conf.:
  Int_t startUnpackers[]  = { 416 };      // 416 for aug04; { 412 }; - old data
  Int_t startMods[6]     = {0,0,0,1,1,1}; //?{1,1,1,0,0,0}; //
  Int_t trigMods[]       = {1};
//   spec->addDetector(new HTBoxDetector);
//   spec->addDetector(new HStartDetector);
//   spec->getDetector("TBox")->setModules(0,trigMods);
//   spec->getDetector("Start")->setModules(-1,startMods);

  
  // MDC configuration
  HMdcDetector *mdc=new HMdcDetector;
  Int_t nSectors=0;
  for(Int_t s=0;s<6;s++) {
    mdc->setModules(s, mdcMods[s]);
    if(mdcMods[s][0]+mdcMods[s][1]+mdcMods[s][2]+mdcMods[s][3] > 1) nSectors++;
  }
  spec->addDetector(mdc);

  HRuntimeDb* rtdb=gHades->getRuntimeDb();

  //Set the data source (here from Lmd file)
  HldFileSource *source=new HldFileSource;
  
  gHades->setDataSource(source);

  if(inDirI) source->setDirectory(inDirI);
  else source->setDirectory(inDir);
  
  for(Int_t s=0; s<6; s++) for (Int_t m=0; m<4; m++) {
//    if(mdcMods[s][m] && mdcUnpackers[s][m]) {
      HMdcUnpacker *unpacker=new HMdcUnpacker(mdcUnpackers[s][m]); 
      unpacker->setQuietMode();
      unpacker->setDecodeVersion(1); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      source->addUnpacker( unpacker);
//    }
  }

//   for(UInt_t i=0; i<(sizeof(startUnpackers)/sizeof(Int_t)); i++)
//       source->addUnpacker( new HStartUnpacker(startUnpackers[i]) );

//-----Parameters input:------------------------

Int_t refId=1912353334; //-1000;

  if(typeFile1 == 'o') {
    HParOraIo *ora;
    ora = new HParOraIo ;
    ora->open();
    if (!ora->check()) {
      Error("ora->check","no connection to Oracle\nexiting...!");
      exit(EXIT_FAILURE);
    }
    rtdb->setSecondInput(ora);
    ora->setHistoryDate("now");
//    refId=ora->getOraInfo()->getLastRun("JAN05");
    
//     HParAsciiFileIo *inp1=new HParAsciiFileIo;
//     inp1->open("param2.txt","in");
//     rtdb->setFirstInput(inp1);
  } else if(typeFile1 == 'a') {
    HParAsciiFileIo *inp1=new HParAsciiFileIo;
    inp1->open(parFile1,"in");
    rtdb->setFirstInput(inp1);
  } else if(typeFile1 == 'r') {
    HParRootFileIo *inp1=new HParRootFileIo;
    inp1->open(parFile1,"READ");
    rtdb->setFirstInput(inp1);
  }
  
  if(typeFile2 == 'a') {
    HParAsciiFileIo *inp2=new HParAsciiFileIo;
    inp2->open(parFile2,"in");
    rtdb->setFirstInput(inp2);
  } else if(typeFile2 == 'r') {
    HParRootFileIo *inp2=new HParRootFileIo;
    inp2->open(parFile2,"READ");
    rtdb->setFirstInput(inp2);
  }
  
  
//  if(refId<-1) rtdb->addRun(refId);
  if(fileI) {
    if(refId<-1) source->addFile(fileI);
    else source->addFile(fileI,refId);
  } else {
    Int_t numInFiles=sizeof(inFiles)/sizeof(*inFiles);
    for(Int_t n=0; n<numInFiles; n++) {
      if(refId<-1) source->addFile(inFiles[n]);
      else source->addFile(inFiles[n],refId);
    }
  }
//----------------------------------------------

  if(cutStatus==1 || (cutStatus==2 && sourceCutPar==1)) {
    HMdcTimeCut* fcut=new HMdcTimeCut();
    rtdb->addContainer(fcut);
    if(sourceCutPar==1) {
      fcut->setCut(cutTime1L, cutTime1R, cutTime2L,cutTime2R, cutLeft,cutRight);
      fcut->setStatic(kTRUE);
    }
    if(cutStatus==1) fcut->setIsUsed(kFALSE);
  }
  
  // Tasks:-------------------------------------------------
  HMdcTrackFinder* trackFinder;
  HTaskSet* tasks = new HTaskSet("MDC","Mdc Track Finding Display");
  
//   tasks->connect(new HTriggerSelector("TriggerSelector","TriggerSelector",
//       trMustHas,trCanHas));
  
  tasks->connect(new HMdcCalibrater1("MdcCali1","MdcCali1",
      calibrater1Ver,(cutStatus<=1)?0:1)/*,"TriggerSelector"*/);
  
  tasks->connect((trackFinder=new HMdcTrackFinder("MdcTrackD","MdcTrackD",
                                     isCoilOff,1)),"MdcCali1");
  tasks->connect((new HMdcClusterToHit("ClHit","ClHit",-1,-1)),"MdcTrackD");
  tasks->connect((targetMon=new HMdcTargetMon("TargMon","TargMon",isCoilOff)),
      "ClHit");
//  tasks->connect((HTask*)NULL,"TargMon");
  
  HMdcVertexFind *mdcVertexFind
          = new HMdcVertexFind("vertexfinder","vertexfinder",0,kTRUE);
  tasks->connect(mdcVertexFind,"TargMon");
  
  tasks->connect((HTask*)NULL,"vertexfinder");
  
//  targetMon->drowTrigCondHist();
  targetMon->drowDownScalHist();
//  targetMon->setHistPar(750,-850.,650.);
  targetMon->setHistPar(1700,-850.,850.);
  targetMon->setFillSegFlag();
  targetMon->setRCut(20.);
  
  
  gHades->getTaskSet("real")->connect(tasks);
  gHades->getTaskSet("real")->connect(NULL,tasks);
  gHades->getTaskSet("real")->print();

//====Seting of parameters of mdcTrackFinder===============
  trackFinder->setLevel(level4s1, level5s1, level4s2, level5s2);
//=========================================================

  if (!gHades->init()) {
    rtdb->closeFirstInput();
    rtdb->closeSecondInput();
    delete gHades;
    return;
  }


  // Set output file:----------------------------
  gHades->setTreeBufferSize(8000);
  HEvent *event = gHades->getCurrentEvent();
  event->getCategory(catMdcRaw)->setPersistency(kFALSE);
  event->getCategory(catMdcCal1)->setPersistency(kFALSE);
  event->getCategory(catMdcClusInf)->setPersistency(kFALSE);
  Int_t compLevel = 2;
  gHades->setOutputFile("dst.root","RECREATE","runs",compLevel);
  gHades->makeTree();
  
  gHades->eventLoop(maxNumEv,0,timeLimit);
//  gHades->eventLoop(maxNumEv,100000,timeLimit);
    
//    HParAsciiFileIo* output=new HParAsciiFileIo;
//    output->open("targPar.txt","out");
//    rtdb->setOutput(output);
//   delete myHades;

  delete gHades;
}
