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
  Char_t* inDir = "/misc/haddisk2/nov01/hld/339";
  
  Int_t nEvSkp=20;   // Faktor skiping: 1 event processe nEvSkp skip.
  Int_t maxNumEv=1000000;

  Int_t refrunID = 20001; // run number used to initialize rtdb
  Char_t* inFiles[]={
    "xx01339021204.hld"
  };

  // Parameters input files:
  Char_t typeFile1='a';  // ='a'-ASCII or 'r'-root file
  Char_t* parFile1 = "/u/mdc/lookupfiles/november2001/lookup_mdc_nov01.txt";

  Char_t typeFile2='n';  // ='a'-ASCII or 'r'-root file; ='n' - first input only  
  //Char_t* parFile2 =
  // "/u/hadesdst/may01/gen1/params/params_MAY01_field_all_051001.root";

  // List of modules:
  Int_t mod[6][4]=
    {
      {0,0,0,0},   // moduledef for sector 1       //  208 205 206
      {1,1,0,0},   // moduledef for sector 2       //  209
      {0,0,0,0},   // ...                          //  210
      {0,0,0,0},                                   //  211 214
      {0,0,0,0},                                   //  212
      {0,0,0,0}                                    //  213
    };


  //List of unpakers:
Int_t mdcUnpacker[]={ 201,202,203,204,205,206,207,208,209,
                      210,211,212,213,214,215,216,217};
//    {201,202,203,215,208,209,210,211,212,213,214,205,207,206};
  //Int_t mdcUnpacker[]={205,206,208,209,210,211,212,213,214};
  //Int_t mdcUnpacker[]={201,202,203,205,206,207,208,209,210,211,212,213,214};


//===Mdc time cuts=======================================================
  // cutStatus=0 - HMdcCalibrater1, HMdcTrackFinder
  //   and HMdcTargetMon will not do time cuts.
  // cutStatus=1 - HMdcCalibrater1 will not do time cuts,
  //   HMdcTrackFinder and HMdcEfficiency will do time cuts.
  // cutStatus=2 - HMdcCalibrater1 will not do time cuts,
  //   HMdcTrackFinder and HMdcEfficiency will not do time cuts.
  // sourceCutPar=0 - using param. of time cuts from parameters files
  // sourceCutPar=1 - seting of cut parameters in this macro.
  //
  // For selecting and checking cuts look on the time distributions
  // with out any cuts (cutStatus=0;).
  // Attention! The cuts in this example of macros seted for non calibreted
  // data (HMdcCalibrater1("MdcCali1","MdcCali1",3,0)). If you use
  // the calibrated data you need to change of cut.
  //
  Int_t calibrater1Ver=3; //1; // see class HMdcCalibrater1
  Int_t cutStatus=1;
  Int_t sourceCutPar=1;
  Float_t cutTime1L[4]={ 220.,  240.,  250.,  100.};
  Float_t cutTime1R[4]={1000., 1100., 1900., 1000.};
  Float_t cutTime2L[4]={ 260.,  260.,  400.,  300.};
  Float_t cutTime2R[4]={1200., 1300., 1600., 1200.};
  Float_t cutLeft  [4]={  22.,   22.,   58.,   35.};  // cutLeft < driftTime1-driftTime2 < cutRight
  Float_t cutRight [4]={ 400.,  500.,  600.,  800.};  // If cutLeft=cutRight=0. the cut will be ignore

//=Parameters of track finder and efficiency tasks:=====================
  // isCoilOff=kTRUE(kFULSE) - The current in coil is  switched off(on).
  Bool_t isCoilOff=kTRUE;
  Int_t typeClustFinder=1; // =1,0 1-independent,

//======================================================================
  HMdcTargetMon* targetMon;
//======================================================================

void targetMon(Char_t* fileI=0, Char_t* inDirI=0){

  Hades* myHades = new Hades();
  gHades->setQuietMode();
  Int_t splitLevel=2;
  gHades->setSplitLevel(splitLevel);
  HSpectrometer* spec=gHades->getSetup();

  // MDC configuration
  HMdcDetector *mdc=new HMdcDetector;
  for(Int_t i=0;i<6;i++) mdc->setModules(i, mod[i]);
  spec->addDetector(mdc);

  HRuntimeDb* rtdb=gHades->getRuntimeDb();

  //Set the data source (here from Lmd file)
  HldFileSource *source=new HldFileSource;
  Int_t numUnp=sizeof(mdcUnpacker)/sizeof(mdcUnpacker[]);
  for(Int_t n=0; n<numUnp; n++)
      source->addUnpacker(new HMdcUnpacker(mdcUnpacker[n],kFALSE));
  gHades->setDataSource(source);

  if(inDirI) source->setDirectory(inDirI);
  else source->setDirectory(inDir);
  rtdb->addRun(refrunID);
  if(fileI) source->addFile(fileI);
  else {
    Int_t numInFiles=sizeof(inFiles)/sizeof(*inFiles);
    for(Int_t n=0; n<numInFiles; n++) source->addFile(inFiles[n]);
  }

  if(typeFile1 == 'a') {
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
    rtdb->setSecondInput(inp2);
  } else if(typeFile2 == 'r') {
    HParRootFileIo *inp2=new HParRootFileIo;
    inp2->open(parFile2,"READ");
    rtdb->setSecondInput(inp2);
  }

  if(cutStatus==1 || (cutStatus==2 && sourceCutPar==1)) {
    HMdcTimeCut* fcut=new HMdcTimeCut();
    rtdb->addContainer(fcut);
    if(sourceCutPar==1) {
      fcut->setCut(cutTime1L, cutTime1R, cutTime2L,cutTime2R, cutLeft,cutRight);
      fcut->setStatic(kTRUE);
    }
    if(cutStatus==1) fcut->setIsUsed(kFALSE);
  }

  
  Int_t kvnt;
  kvnt=100/(1+nEvSkp);
  if(kvnt==0) kvnt=1;
  // Tasks:-------------------------------------------------
  HMdcTrackFinder* trackFinder;
  HTaskSet* tasks = new HTaskSet("MDC","Mdc Track Finding Display");
  tasks->connect(new HMdcCalibrater1("MdcCali1","MdcCali1",
      calibrater1Ver,(cutStatus<=1)?0:1));
  tasks->connect((trackFinder=new HMdcTrackFinder("MdcTrackD","MdcTrackD",
                                     isCoilOff,1)),"MdcCali1");
  tasks->connect((new HMdcClusterToHit("ClHit","ClHit")),"MdcTrackD");
  tasks->connect((targetMon=new HMdcTargetMon("TargMon","TargMon",isCoilOff,
      kvnt+kvnt*nEvSkp)),
      "ClHit");
  tasks->connect((HTask*)NULL,"TargMon");
  gHades->getTaskSet("real")->connect(tasks);
  gHades->getTaskSet("real")->connect(NULL,tasks);
  gHades->getTaskSet("real")->print();

//====Seting of parameters of mdcTrackFinder===============
//  trackFinder->setLevel(level4s1, level5s1, level4s2, level5s2);
//=========================================================

  if (!gHades->init()) {
    rtdb->closeFirstInput();
    rtdb->closeSecondInput();
    //   delete gHades;
    return;
  }


// Set output file
  // Int_t compLevel = 2;
  // gHades->setOutputFile(outFile,"RECREATE","runs",compLevel);

  // gHades->makeTree();

  Int_t nev;
  Int_t nevTot;
  nevTot=0;
  if(nEvSkp==0) gHades->eventLoop(maxNumEv);
  else {
    while(nevTot<maxNumEv) { 
      nev=gHades->eventLoop(kvnt,kvnt*nEvSkp);
      if(nev<=0) break;
      nevTot += nev;
    }
  }
  targetMon->drawCanvCut();
  
  delete gHades;
}
