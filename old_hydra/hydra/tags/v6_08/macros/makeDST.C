#! /home/manuel/bin/hydra
{
//////////////////////////////////////////////////////////////////////////
//  makeDST.C                                                           //
//                                                                      //
//  This is the configuration file for DST production. For the options  //
//  and default parameters of the H***TaskSets, see the html            //
//  documentation for the individual task sets.  This currently         //
//  applies to HMdcTaskSet and HKickTaskSet (e.g. mdc hit-finder        //
//  configuration, kick plane mode, etc.).                              //
//                                                                      //
//     Use with analyze("makeDST.C",nEvents);                           //
//                                                                      //
//     Last Modified 24.11.01                                           //
//     Dan Magestro                                                     //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

// -------------- Input and Output files and directory----------------

  TString inFile   = "xx01322172045.hld";
  TString inDir    = "/d/hades/trigger/";
  TString outFile = "testDST.root";

//---------------  Parameter input file for real data ----------------

  TString rootParFile = "/u/hadesdst/nov01/gen1/params/allParamTest341.root";

//------------ Do not write these categories to the Tree -------------

  Cat_t notPersistentCat[20] = {

       catMdcRaw, catShowerRaw, catStartRaw, catTofRaw, catTofinoRaw
       //  ,catMdcCal1,catMdcCal2,catRichCal,catShowerCal
  };

// ----------------  Unpacker configuration --------------------------

  Int_t richUnpackers[20] = { 100,101,102,103,104,105,
                              106,107,108,109,110,111 };

  Int_t mdcUnpackers[20] = { 201,202,203,205,206,207,208,
                             209,210,211,212,213,214,215 };

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

  HldFileSource *source=new HldFileSource;
  source->setDirectory(inDir.Data());
  source->addFile(inFile.Data(),1);
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
  HParRootFileIo *input=new HParRootFileIo;
  input->open(rootParFile.Data(),"READ");
  rtdb->setFirstInput(input);

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

  source->addUnpacker( new HMatchUUnpacker(1,0));

// ----------- Build task sets (using H***TaskSet::make) -------------
// ---- By default, the make() function returns the full task set ----
// ----- See the html documentation for the H***TaskSet options ------

  HTask *startTasks  = HStartTaskSet::make();
  HTask *richTasks   = HRichTaskSet::make();
  HTask *showerTasks = HShowerTaskSet::make();
  HTask *tofTasks    = HTofTaskSet::make();
  HTask *tofinoTasks = HTofinoTaskSet::make();
  HTask *mdcTasks    = HMdcTaskSet::make("","NoStartAndNoCal,MagnetOff,Dubna,ChamberClusters");
  HTask *kickTasks   = HKickTaskSet::make("","lowres,debug");

//------------------------ Master task set ---------------------------

  HTaskSet *masterTaskSet = gHades->getTaskSet("real");

  gHades->getTaskSet("real")->add(startTasks);
  //  gHades->getTaskSet("real")->add(tofTasks);
  gHades->getTaskSet("real")->add(tofinoTasks);
  gHades->getTaskSet("real")->add(richTasks);
  gHades->getTaskSet("real")->add(showerTasks);
  gHades->getTaskSet("real")->add(mdcTasks);
  gHades->getTaskSet("real")->add(kickTasks);

  gHades->getTaskSet("real")->isTimed(kTRUE);
  gHades->makeCounter(1);

  //Initialization
  printf("gHades->init()\n");
  if (!gHades->init()) printf("Error\n");
 
  rtdb->getContainer("MdcCal2Par")->setStatic();

  //Set output (change if you want)
  gHades->setOutputFile(outFile.Data(),"RECREATE","Test",2);

  gHades->makeTree();
  HRecEvent *ev=gHades->getCurrentEvent();
  HMUData *mu=ev->getPartialEvent(catMatchU)->getSubHeader();
  mu->setTriggerCodeFirstFormat(1);
 
}








