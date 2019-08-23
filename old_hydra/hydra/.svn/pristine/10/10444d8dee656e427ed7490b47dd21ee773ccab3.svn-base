{
//********************** Configuration area ********************************
Int_t mdcMods[4] = {1,1,1,1};       //Bitmap of active modules
Int_t sectors[6] = {1,1,1,1,1,1};   //Bitmap of active sectors
TString inFile   = "/d/hades/sanchez/ca2ca.all.nomdc34.root";  //Input file
TString outFile  = "test.root";  //Output file
//A maximum of two parameter files is accepted. The string "oracle" can be used 
//instead of a parameter file name for initialization from Oracle.
TString parFile1 = "oracle"; 
TString parFile2 = "/u/sanchez/hades/anal/dev/mdctrackS/mdcSPar.par";
//****************** Advanced configuration *******************************
Int_t splitLevel = 2;

//*******************************************************************
//** Macro code                         *****************************
//** You should not need to change this *****************************
//*******************************************************************
HLocation aLoc; //Used to conf. the data source and segment finders
aLoc.set(2,0,0);

HMdcDetector *mdc=new HMdcDetector;
for (Int_t sector=0;sector<6;sector++)
  if (sectors[sector] == 1) mdc->setModules(sector,mdcMods);
gHades->getSetup()->addDetector(mdc);

//Set data source
HDataSource *dataSource=0;
if (inFile.Contains(".root")) {         //Root input file
  HRootSource *datos=new HRootSource;
  datos->setDirectory("/d/hades/sanchez");
  datos->addFile(inFile.Data());
  dataSource = datos;
} else if (inFile.Contains(".hld")) {   //List mode data file
  HldFileSource *datos = new HldFileSource;
  datos->setDirectory("/d/hades/sanchez");
  datos->addFile(inFile.Data());
  dataSource = datos;
} else cerr << "Input data suffix not known" << endl;
gHades->setDataSource(datos);

//Set split level
gHades->setSplitLevel(splitLevel);

//Set runtime database
HRuntimeDb* rtdb=gHades->getRuntimeDb();
if (parFile1=="oracle") {
  HParOraIo *ora = new HParOraIo;
  ora->open();
  if (!ora->check()) {
    printf("No connection to Oracle available\n");
    return -1;
  }
  rtdb->setFirstInput(ora);
} else if (parFile1.Contains(".root")) {
  HParRootFileIo *inroot = new HParRootFileIo;
  inroot->open(parFile1.Data());
  rtdb->setFirstInput(inroot);
} else {
  HParAsciiFileIo *input1 = new HParAsciiFileIo;
  input1->open(parFile1.Data(),"in");
  rtdb->setFirstInput(input1);
}

if (parFile2!="") {
  HParAsciiFileIo *input2=new HParAsciiFileIo;
  input2->open(parFile2.Data(),"in");
  rtdb->setSecondInput(input2);
}

//Set tasks
HTaskSet *simulTasks=gHades->getTaskSet("simulation");
HMdcDigitizer *digitizer=0; Text_t digiCad[]="mdc.digi";
HMdcHitFSim *finder=0;      Text_t findCad[]="mdc.hitf";
HMdcSegmentF *segFinder=0;  Text_t segFindCad[]="mdc.segf";
HMdcHitComp *comp=0;
HMdcCalibrater2Sim *calibrater=0; Text_t calibCad[]="mdc.calib";
Text_t buffer[255];
HReconstructor *antTask=0;

//Add digitizer
digitizer=new HMdcDigitizer(digiCad,"Mdc digitizer");
simulTasks->connect(digitizer);
antTask=digitizer;

//Add calibrater2
calibrater = new HMdcCalibrater2Sim(calibCad,"Mdc calibrater");
simulTasks->connect(calibrater,antTask);
antTask=calibrater;

for (Int_t sector = 0; sector<6; sector++) { //For each sector
  if (sectors[sector] == 1) { //Sector is activated
    aLoc[0]=sector;

    for (Int_t i=0;i<4;i++) { //Add Finders
      if (mdcMods[i] != 0) {
	sprintf(buffer,"%s%i.%i",findCad,sector,i);
	finder=new HMdcHitFSim(buffer,"Mdc Hit finder");
	aLoc[1]=i;
	finder->setLoc(aLoc);
	simulTasks->connect(finder,antTask);
	antTask=finder;

// 	sprintf(buffer,"mdc.hitcomp%i.%i",sector,i);
// 	comp=new HMdcHitComp(buffer,"Mdc Hit Comp");
// 	aLoc[1]=i;
// 	comp->setLoc(aLoc);
// 	simulTasks->connect(comp,antTask);
// 	antTask=comp;
      }
    }
    sprintf(buffer,"%s%i",segFindCad,sector);
    segFinder=new HMdcSegmentF(buffer,"Mdc Segment finder");
    aLoc[1]=0;
    segFinder->setLoc(aLoc);
    simulTasks->connect(segFinder,antTask);
    antTask=segFinder;
  }
}

//Initialize the system
if (!gHades->init()) {
  printf("Error during initialization\n");
  rtdb->closeFirstInput();
  rtdb->closeSecondInput();
  return 1;
}

//Configure output
gHades->setOutputFile(outFile.Data(),"RECREATE","Test",1);
gHades->makeTree();

}


















