{
gROOT->Reset();

  Hades* myHades = new Hades;
HRichDetector rich; 
Int_t mods[1]={1}; // in RICH there is only 1 module 
rich.setModules(0,mods);   // 0 sector
rich.setModules(1,mods);   // 1 sector
rich.setModules(2,mods);   // 2 sector
rich.setModules(3,mods);   // 3 sector
rich.setModules(4,mods);   // 4 sector
rich.setModules(5,mods);   // 5 sector
gHades->getSetup()->addDetector(&rich);
HRuntimeDb* rtdb = gHades->getRuntimeDb();


HParOraIo input;
//HParHadAsciiFileIo input("RAW");
//input.open("map.txt","READ");
input.open();
rtdb->setFirstInput(&input);

//HParHadAsciiFileIo output;
//output.open("map1.txt", "RECREATE");
//rtdb->setOutput(&output);

//HParRootFileIo *output=new HParRootFileIo;
//output->open("input/richmappingparnew.root", "RECREATE");
//output->print();
//rtdb->setOutput(output);

HParHadAsciiFileIo output("RAW");
output.open("mapOra.txt", "RECREATE");
rtdb->setOutput(&output);


HRichMappingPar* pMapPar = new HRichMappingPar;
rtdb->addContainer(pMapPar);

HOraInfo* oraInfo=input.getOraInfo();
  Int_t runId=oraInfo->getRunId("xx00323045047.hld");
  rtdb->addRun(runId);

rtdb->initContainers(runId);
gHades->getRuntimeDb()->print();
pMapPar->setChanged();
rtdb->saveOutput();
rtdb->closeFirstInput();
rtdb->closeOutput();

}
