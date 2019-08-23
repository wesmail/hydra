{
  printf("Init mdc\n");
//Create detectors and their setup
  HMdcDetector *mdc=new HMdcDetector;
  Int_t mods[4]={0,1,0,0}; 
  mdc->setModules(2,mods);   
//mdc->setModules(5,mods);
  gHades->getSetup()->addDetector(mdc);

//Set the runtime database;
//  gHades->getRuntimeDb()->addEventFile("/s/hades/ht99/ht99_38.hld");

  HParAsciiFileIo *inp2=new HParAsciiFileIo;
  inp2->open("/u/halo/anal/test/macros/Mdc/mdc_lookup_symcorr.txt","in");
  gHades->getRuntimeDb()->setFirstInput(inp2);

  HParAsciiFileIo *inp1=new HParAsciiFileIo;
  inp1->open("/u/zumbruch/anal/beamtime/macros/Mdc/tmp.txt","in");
  gHades->getRuntimeDb()->setSecondInput(inp1);

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

//Set the data source (here from Lmd file)

//Remote access to DAQ-Standard
//Standalone DAQ FOR Mdc:
  HldRemoteSource *source=new HldRemoteSource("hades21");
// source->setDump(); 
  source->addUnpacker(new HMdcUnpacker(513,kTRUE));
  gHades->setDataSource(source);

// Monitoring

  HTask* end=0;
  Text_t monitor[20]="mdc.mon";
  gHades->getTaskSet("real")->connect(new HMdcMonitor(monitor,monitor));
  gHades->getTaskSet("real")->connect(end,monitor);
 
  
//  gHades->getRuntimeDb()->setCurrentEventFile(-1);
  printf("gHades->init()\n");
  if (!gHades->init()) printf("Error\n");

  gHades->getRuntimeDb()->print();

//Set output file
  Int_t compLevel=2;       //compression level of the output file
  gHades->setOutputFile("et99_0004.root","RECREATE","Test",compLevel);

//Build output tree
  gHades->makeTree();

}








