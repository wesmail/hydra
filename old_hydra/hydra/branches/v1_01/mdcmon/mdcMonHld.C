{
Int_t Id = 4; // 1 for real data 4 - for cal
  printf("Init mdc\n");
//Create detectors and their setup
  HMdcDetector *mdc=new HMdcDetector;
  Int_t mods[4]={0,1,0,0}; 
  mdc->setModules(2,mods);   
//mdc->setModules(5,mods);
  gHades->getSetup()->addDetector(mdc);

//Set the runtime database;
  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  HRun* run=rtdb->addRun(944451190); 

  HParAsciiFileIo *inp2=new HParAsciiFileIo;
  inp2->open("/u/halo/anal/test/macros/Mdc/mdc_lookup.txt","in");
  gHades->getRuntimeDb()->setFirstInput(inp2);

//Temporal solution
   HMdcRawStruct *parraw = new HMdcRawStruct;
   gHades->getRuntimeDb()->addContainer(parraw);
   if (! ((HParSet*) parraw)->init()) {
     rtdb->closeFirstInput();
     rtdb->closeOutput();
     return 1;
   }
//Temporal solution

//Set the split level
  Int_t splitLevel=2;      //split level of the output tree
  gHades->setSplitLevel(splitLevel);


// Reading from hld file
  HldFileSource *source=new HldFileSource;
  source->addUnpacker(new HMdcUnpacker(513,FALSE));
  gHades->setDataSource(source);
  source->setDirectory("/d/hades/nov99"); 
  source->addFile("n99_037.hld");

// Monitoring
  HTaskSet* realtask = new HTaskSet("real task set","real task set");
//  HMdcMonitor * monReal = new HMdcMonitor("monitor","monitor",Id);
  HMdcMonitor * monReal = new HMdcMonitor("monitor","monitor");
  realtask->connect(monReal);
  realtask->connect(NULL,"monReal");

//  HTaskSet* caltask = new HTaskSet("cal task set","cal task set");
//  HMdcMonitor * monCal  = new HMdcMonitor("monitor","monitor",Id);
//  caltask->connect(monCal);
//  caltask->connect(NULL,"monCal");

 
//  HTask* end=0;
//  Text_t monitor[20]="mdc.mon";
//  gHades->getTask()->connect(new HMdcMonitor(monitor,monitor,Id));
//  gHades->getTask()->connect(end,monitor);
//  gHades->getTaskSet("calibration")->connect(new HMdcMonitor(monitor,monitor,Id));
   gHades->getTaskSet("real")->connect(realtask); 
//   gHades->getTaskSet("calibration")->connect(caltask);

  
  printf("gHades->init()\n");
  if (!gHades->init()) printf("Error\n");

  gHades->getRuntimeDb()->print();

//Set output file
  Int_t compLevel=2;       //compression level of the output file
  gHades->setOutputFile("n99_037.root","RECREATE","Test",compLevel);

//Build output tree
  gHades->makeTree();

}








