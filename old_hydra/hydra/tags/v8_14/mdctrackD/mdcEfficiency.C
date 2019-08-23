////////////////////////////////////////////////////////////////////
//Using:
//  root [0] .x mdcTrackD.C();
//  or
//  root [0] .x mdcTrackD.C("file_name"); (inDir will used !!!)
////////////////////////////////////////////////////////////////////
  gROOT->Reset();
//=Input and Output files and directory=============================
  Char_t* inDir = "/d/hades/apr01/hld/114";
  Int_t refrunID = 20001; // run number used to initialize rtdb
  Char_t* inFiles[]={
    "xx01114023224.hld",
    "xx01114024826.hld",
    "xx01114031107.hld",
    "xx01114053832.hld"
  };

  Char_t* outFile="apr2001.root";
  Bool_t makeTree=kTRUE;  // do tree or not, if=kTRUE the HMdcClusterToHit task
                          // will added to tasks list

  // Parameters input files:
  Char_t typeFile1='a';  // ='a'-ASCII or 'r'-root file
  //Char_t* parFile1 = "lookup_apr01_corr_2304_cal_1000.txt";
  Char_t* parFile1 = "/net/home6/kp1/hejdar/hydra/v5_31/macros/Mdc/data2/testpar.txt";
  //Char_t* parFile1 = "/u/pechenov/Hydra/Param/lookup_apr01.txt";
  Char_t typeFile2='n';  // ='a'-ASCII or 'r'-root file; ='n' - first input only
  //Char_t* parFile2 = "kick_pl_new.par";

  // List of modules:
  Int_t mod[6][4]=                                                                    
    {                                                                                  
      {0,1,1,0},   // moduledef for sector 1       //  208 205 206                   
      {0,1,0,0},   // moduledef for sector 2       //  209                           
      {0,0,0,0},   // ...                          //  210                           
      {0,0,0,0},                                   //  211 214                       
      {0,0,0,0},                                   //  212                           
      {0,0,0,0}                                    //  213                           
    };
 

  //List of unpakers:                                                                    
  Int_t mdcUnpacker[]={205,206,208,209,210,211,212,213,214};
  //Int_t mdcUnpacker[]={201,202,203,205,206,207,208,209,210,211,212,213,214};

//===Mdc time cuts======================================================= 
  // cutStatus=0 - HMdcCalibrater1, HMdcTrackFinder 
  //   and HMdcEfficiency will not do time cuts.
  // cutStatus=1 - HMdcCalibrater1 will not do time cuts,
  //   HMdcTrackFinder and HMdcEfficiency will do time cuts.
  // cutStatus=2 - HMdcCalibrater1 will not time cuts,
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
  Int_t calibrater1Ver=3; // see class HMdcCalibrater1
  Int_t cutStatus=1;
  Int_t sourceCutPar=1;
  Float_t cutTime1L[4]={ 800.,  550.,  400.,  400.};
  Float_t cutTime1R[4]={1300., 1300., 1900., 1950.};
  Float_t cutTime2L[4]={ 900.,  600.,  500.,  500.};
  Float_t cutTime2R[4]={1400., 1450., 1900., 1900.};
  Float_t cutLeft  [4]={  10.,   22.,   30.,   35.};  // cutLeft < driftTime1-driftTime2 < cutRight
  Float_t cutRight [4]={ 250.,  500.,  600.,  800.};  // If cutLeft=cutRight=0. the cut will be ignore

  
//=Parameters of track finder and efficiency tasks:=====================
  // Type of working of Dubna cluster finder:
  // isCoilOff=kTRUE(kFULSE) - The current in coil is  switched off(on).
  // typeClustFinder=1(0)
  // isCoilOff=kTRUE, typeClustFinder=1:
  //    The tracks will searched in each MDC (1-4) independent
  // isCoilOff=kTRUE, typeClustFinder=0:
  //   The tracks will searched in all MDC of sector together
  //   (needs in a alignment of MDC's)
  // isCoilOff=kFALSE, typeClustFinder=1:
  //   The tracks will searched in MDC1, MDC2 (segment 1) independent,
  //   in MDC3, MDC4 (segment 2) together, but information about
  //   hits in segment 2 will not used for estimation of efficiency
  //   (it will be done in the future).
  // isCoilOff=kFALSE, typeClustFinder=0:
  //   The tracks will searched in MDC1, MDC2 (segment 1) together
  //   (needs in a alignment of MDC's) and
  //   in MDC3, MDC4 (segment 2) alsow together, but information about
  //   hits in segment 2 will not used for estimation of efficiency
  //   (it will be done in the future).
  // For isCoilOff=kFALSE the program needs in HKickPlane parameters
  Bool_t isCoilOff=kTRUE;
  Int_t typeClustFinder=1; // =1,0 1-independent,

  // HMdcClus persistency:
  Bool_t persistency=kFALSE;    // write or not the HMdcClus data in output file

  // Parameters for cluster finder for estimatin of efficiency:
  Int_t level4s1=4;
  Int_t level5s1=100000;
  Int_t level4s2=4;
  Int_t level5s2=100000;

  // List of num. of working layers in modules: lLayers[sec][mod]
  //Int_t nLayers[6][4]={ {3,6,6,6}, {6,6,6,6}, {6,6,6,6},
  //                      {4,6,6,6}, {6,6,6,6}, {5,6,6,6} };
//======================================================================
  HMdcEfficiency* mdcEff;
  HMdcEffMenu* effMenu;
//======================================================================

void mdcEfficiency(Char_t* fileI=0){

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

  source->setDirectory(inDir);
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
    rtdb->setSecontInput(inp2);
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


  // Tasks:-------------------------------------------------
  HMdcTrackFinder* trackFinder;
  HTaskSet* tasks = new HTaskSet("MDC","Mdc Track Finding Display");
  tasks->connect(new HMdcCalibrater1("MdcCali1","MdcCali1",
      calibrater1Ver,(cutStatus<=1)?0:1));
  tasks->connect((trackFinder=new HMdcTrackFinder("MdcTrackD","MdcTrackD",
                                     isCoilOff,typeClustFinder)),"MdcCali1");
  tasks->connect((mdcEff=new HMdcEfficiency("MdcEff","MdcEff")),"MdcTrackD");
  if(makeTree) {tasks->connect((new HMdcClusterToHit("ClHit","ClHit")),"MdcEff");
    tasks->connect((HTask*)NULL,"ClHit");
  } else tasks->connect((HTask*)NULL,"MdcEff");
  gHades->getTaskSet("real")->connect(tasks);
  gHades->getTaskSet("real")->connect(NULL,tasks);
  gHades->getTaskSet("real")->print();

//====Seting of parameters of mdcTrackFinder===============
  trackFinder->setPersistency(persistency);
  trackFinder->setLevel(level4s1, level5s1, level4s2, level5s2);
//  trackFinder->setNLayers(&nLayers[0][0]);
//=========================================================

  if (!gHades->init()) {
    rtdb->closeFirstInput();
    rtdb->closeSecondInput();
    delete gHades;
    return;
  }


// Set output file
  Int_t compLevel = 2;
  gHades->setOutputFile(outFile,"RECREATE","runs",compLevel);

  if(makeTree) gHades->makeTree();

  effMenu=HMdcEffMenu::makeMenu(mdcEff);

//  delete gHades;
}
