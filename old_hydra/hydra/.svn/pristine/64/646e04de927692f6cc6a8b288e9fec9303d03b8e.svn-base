////////////////////////////////////////////////////////////////////
//Using:
//  root [0] .x target.C();
//  or
//  root [0] .x target.C("file_name");
//  or
//  root [0] .x target.C("file_name","inDir_name");
////////////////////////////////////////////////////////////////////
gROOT->Reset();

//========Event loop end condition:==================================
// Double_t timeLimit - time limit for run macro in minutes.
// Int_t maxNumEv - limit for number of events
// Event loop end condition: number of calculated events==maxNumEv or 
// runing tasks real time >= timeLimit 
Double_t timeLimit=3;
Int_t maxNumEv=500000;

//========Time canvas(es) refreshing=================================
// Bool_t batch=kFALSE - show canvas(es) and refresh them each
//                      "timeRefresh" seconds (in real time)
//        batch=kTRUE -- show canvas(es) at the end of event loop
Bool_t batch=kFALSE;
Double_t timeRefresh=10; // (sec.)

//========Automatical canvas(es) saving:=============================
// Possible types are: .gif, .ps, .eps, .root, .svg
// The files names will: canvas_name.gif (ps,...)
// Char_t* filesTypes=0; - don't save canvas(es) automaticaly
Char_t* filesTypes=".gif.root";

//========Histogram parameters:======================================
// nBins<=0; - use default parameters (900,-1000.,800.)
Int_t nBins=900;
Double_t zmin=-1000.; // (mm)
Double_t zmax=800.;   // (mm)

//========Plot type:=================================================
// plotType=0 - Theta vs Zmin 
//         =1 - Zmin vs event number
//         =2 - X vs Y on the plane Z=target pos.
Int_t plotType=0;

//========Input file and dir.:=======================================
Char_t* inDir = "/misc/hadeb03/nov02/hld";
Char_t* inFiles[]={
  "be02309205830.hld"
};
Int_t refrunID = -1000; // run number used to initialize rtdb, 
                        // <-1 refrunId from file

//========Parameters input:==========================================
//First param. input:
Char_t typeFile1='o';  // 'a'-ASCII, 'r'-root file, 'o'-oracle
Char_t* parFile1 = "/u/mdc/lookupfiles/november2001/lookup_mdc_nov01.txt";
// Second param. input:
Char_t typeFile2='n';  // ='a'-ASCII or 'r'-root file; ='n' - first input only  
Char_t* parFile2 ="/u/hadesdst/may01/gen1/params/params_MAY01_field_all_051001.root";

//========List of modules:===========================================
Int_t mdcMods[6][4]=
  { {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {1,1,0,0},
    {0,0,0,0} };

//========Unpacker configuration:====================================
Int_t mdcUnpackers[6][4] = { {201,207,213,217},
                             {202,208,  0,  0},
                             {203,209,214,  0},
                             {204,210,215,218},
                             {205,211,  0,  0},
                             {206,212,216,  0} };

//========Mdc time cuts:=============================================
// cutStatus=0 - HMdcCalibrater1, HMdcTrackFinder
//   and HMdcTargetMon will not do time cuts.
// cutStatus=1 - HMdcCalibrater1 will not do time cuts,
//   HMdcTrackFinder and HMdcEfficiency will do time cuts.
// cutStatus=2 - HMdcCalibrater1 will do time cuts,
//   HMdcTrackFinder and HMdcEfficiency will not do time cuts.
// sourceCutPar=0 - using param. of time cuts from parameters files or oracle
// sourceCutPar=1 - seting of cut parameters in this macro.
Int_t calibrater1Ver=3; //1; // see class HMdcCalibrater1
Int_t cutStatus=2;
Int_t sourceCutPar=1;
Float_t cutTime1L[4]={ 300.,  300.,    0.,  300.};
Float_t cutTime1R[4]={1300., 1300., 1700., 1800.};
Float_t cutTime2L[4]={ 350.,  350.,    0.,  300.};
Float_t cutTime2R[4]={1500., 1500., 1700., 1800.};
Float_t cutLeft  [4]={  20.,   20.,    0.,   20.};  // cutLeft < driftTime1-driftTime2 < cutRight
Float_t cutRight [4]={ 500.,  500.,    0.,  500.};  // If cutLeft=cutRight=0. the cut will be ignore

//========Cluster finder parameters:=================================
Int_t level4s1=3;
Int_t level5s1=4;
Int_t level4s2=3;
Int_t level5s2=4;
  
//=Parameters of track finder and efficiency tasks:=====================
// isCoilOff=kTRUE(kFULSE) - The current in coil is  switched off(on).
Bool_t isCoilOff=kTRUE;



//======================================================================
HMdcTargetMon* targetMon;

void targetMon(Char_t* fileI=0, Char_t* inDirI=0){

  Hades* myHades = new Hades();
  Int_t splitLevel=2;
  gHades->setSplitLevel(splitLevel);
  HSpectrometer* spec=gHades->getSetup();

  // MDC configuration
  HMdcDetector *mdc=new HMdcDetector;
  for(Int_t i=0;i<6;i++) mdc->setModules(i, mdcMods[i]);
  spec->addDetector(mdc);

  HRuntimeDb* rtdb=gHades->getRuntimeDb();

  //Set the data source (here from Lmd file)
  HldFileSource *source=new HldFileSource;
  
  gHades->setDataSource(source);

  if(inDirI) source->setDirectory(inDirI);
  else source->setDirectory(inDir);
  if(refrunID<-1) rtdb->addRun(refrunID);
  if(fileI) {
    if(refrunID<-1) source->addFile(fileI);
    else source->addFile(fileI,refrunID);
  } else {
    Int_t numInFiles=sizeof(inFiles)/sizeof(*inFiles);
    for(Int_t n=0; n<numInFiles; n++) {
      if(refrunID<-1) source->addFile(inFiles[n]);
      else source->addFile(inFiles[n],refrunID);
    }
  }
  
  for(Int_t s=0; s<6; s++) for (Int_t m=0; m<4; m++) {
    if(mdcMods[s][m] && mdcUnpackers[s][m]) {
      HMdcUnpacker *unpacker=new HMdcUnpacker(mdcUnpackers[s][m]); 
      unpacker->setQuietMode();
      unpacker->setDecodeVersion(1); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      source->addUnpacker( unpacker);
    }
  }

//-----Parameters input:------------------------
  if(typeFile1 == 'o') {
    HParOraIo *ora;
    ora = new HParOraIo ;
    ora->open();
    if (!ora->check()) {
      Error("ora->check","no connection to Oracle\nexiting...!");
      exit(EXIT_FAILURE);
    }
    rtdb->setFirstInput(ora);
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
    rtdb->setSecondInput(inp2);
  } else if(typeFile2 == 'r') {
    HParRootFileIo *inp2=new HParRootFileIo;
    inp2->open(parFile2,"READ");
    rtdb->setSecondInput(inp2);
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
  tasks->connect(new HMdcCalibrater1("MdcCali1","MdcCali1",
      calibrater1Ver,(cutStatus<=1)?0:1));
  tasks->connect((trackFinder=new HMdcTrackFinder("MdcTrackD","MdcTrackD",
                                     isCoilOff,1)),"MdcCali1");
  tasks->connect((new HMdcClusterToHit("ClHit","ClHit")),"MdcTrackD");
  tasks->connect((targetMon=new HMdcTargetMon("TargMon","TargMon",
      isCoilOff,plotType)),"ClHit");
  tasks->connect((HTask*)NULL,"TargMon");
  gHades->getTaskSet("real")->connect(tasks);
  gHades->getTaskSet("real")->connect(NULL,tasks);
  gHades->getTaskSet("real")->print();


//====Seting of HMdcTargetMon parameters========================
  targetMon->setMode(batch,filesTypes);
  if(!batch) targetMon->setTimeStep(timeRefresh);
  if(nBins>0) targetMon->setHistPar(nBins,zmin,zmax); 
//====Seting of MdcTrackFinder parameters=======================
  trackFinder->setLevel(level4s1, level5s1, level4s2, level5s2);
//==============================================================

  if (!gHades->init()) {
    rtdb->closeFirstInput();
    rtdb->closeSecondInput();
    delete gHades;
    return;
  }


  // Set output file:----------------------------
  // Int_t compLevel = 2;
  // gHades->setOutputFile(outFile,"RECREATE","runs",compLevel);
  // gHades->makeTree();
  
  gHades->eventLoop(maxNumEv,0,timeLimit);

  delete gHades;
}
