{  
//Standard macro to analyze real events
//************************* Configuration area ********************** 
Int_t mdcMods[4] = {0,1,0,0};             //Bitmap of active modules
Int_t sectors[6] = {0,0,1,0,0,0};         //Bitmap of active sectors  
Bool_t doSegmentMatching = kFALSE;        //Add segment matcher (MDC 1,2 needed) 
TString inputDir = "/d/hades/nov99";      //Input Directory 
TString inFile   = "n99_086.hld";         //Input file
TString outFile  = "test.root";           //Output file
//A maximum of two parameter files is accepted. The string "oracle" can be used 
//instead of a parameter file name.
TString parFile1 = "oracle"; 
TString parFile2 = "../anal/dev/mdctrackS/mdcSPar.par";  //Tracking parameters
//****************** Advanced configuration ************************** 
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
  datos->setDirectory(inputDir.Data()); 
  datos->addFile(inFile.Data()); 
  dataSource = datos; 
} else if (inFile.Contains(".hld")) {   //List mode data file 
  HldFileSource *source = new HldFileSource; 
  source->addUnpacker(new HMdcUnpacker(513,kFALSE)); 
  gHades->setDataSource(source);  
  source->setDirectory(inputDir.Data()); 
  source->addFile(inFile.Data()); 
  dataSource = source; 
} else cerr << "Input data suffix not known" << endl; 
gHades->setDataSource(source); 
 
//Set split level 
gHades->setSplitLevel(splitLevel); 
 
//Set runtime database: FIXME 
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
HTaskSet *realTasks=gHades->getTaskSet("real"); 
HMdcCalibrater1 *calibrater1=0; Text_t calib1Cad[]="mdc.calib1"; 
HMdcCalibrater2 *calibrater2=0; Text_t calib2Cad[]="mdc.calib2"; 
HMdcHitFSim *finder=0;      Text_t findCad[]="mdc.hitf"; 
HMdcSegmentF *segFinder=0;  Text_t segFindCad[]="mdc.segf"; 
Text_t buffer[255]; 
HReconstructor *antTask=0; 
 
//Add calibrater1 
calibrater1 = new HMdcCalibrater1(calib1Cad,"Mdc calibrater"); 
realTasks->connect(calibrater1,antTask); 
antTask=calibrater1; 
 
//Add calibrater2 
calibrater2 = new HMdcCalibrater2(calib2Cad,"Mdc calibrater"); 
realTasks->connect(calibrater2,antTask); 
antTask=calibrater2; 
 
for (Int_t sector = 0; sector<6; sector++) { //For each sector 
  if (sectors[sector] == 1) { //Sector is activated 
    aLoc[0]=sector; 
 
    for (Int_t i=0;i<2;i++) { //Add Hit Finders 
      if (mdcMods[i] != 0) { 
	sprintf(buffer,"%s%i.%i",findCad,sector,i); 
	finder=new HMdcHitF(buffer,"Mdc Hit finder"); 
	aLoc[1]=i; 
	finder->setLoc(aLoc); 
	realTasks->connect(finder,antTask); 
	antTask=finder; 
      } 
    } 
    if (doSegmentMatching) { //Add segment finders
      sprintf(buffer,"%s%i",segFindCad,sector); 
      segFinder=new HMdcSegmentF(buffer,"Mdc Segment finder"); 
      aLoc[1]=0; 
      segFinder->setLoc(aLoc); 
      realTasks->connect(segFinder,antTask); 
      antTask=segFinder;
    } 
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
