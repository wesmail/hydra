{

  Hades myHades;
  HRuntimeDb* rtdb=gHades->getRuntimeDb();


  HRichDetector rich; 
  Int_t mods[1]={1}; // in RICH there is only 1 module 
  rich.setModules(0,mods);   // 0 sector
  rich.setModules(1,mods);   // 1 sector
  rich.setModules(2,mods);   // 2 sector
  rich.setModules(3,mods);   // 3 sector
  rich.setModules(4,mods);   // 4 sector
  rich.setModules(5,mods);   // 5 sector
  gHades->getSetup()->addDetector(&rich);

  rtdb->addRun(944433336);

  HParHadAsciiFileIo input1; // 1st ASCII input
  input1.openfilelist("richTest/data/rich_key_filelist.txt");
  rtdb->setFirstInput(&input1);
  
  HParHadAsciiFileIo input2("RAW"); // 2nd ASCII input
//  input2.open("input/ri00152141625cal.txt");
  input2.openfilelist("richTest/data/rich_raw_filelist.txt");
  rtdb->setSecondInput(&input2);

//  HParHadAsciiFileIo output;
//  output.open("calanadigigeoout.txt", "RECREATE");
  HParRootFileIo output;
  output.open("Richcalanadigigeomapout521.root", "RECREATE");
  rtdb->setOutput(&output);

  HRichMappingPar mappar;
  rtdb->addContainer(&mappar);

  HRichAnalysisPar anapar;
  rtdb->addContainer(&anapar);

  HRichDigitisationPar digipar;
  rtdb->addContainer(&digipar);

  HRichGeometryPar geopar;
  rtdb->addContainer(&geopar);
 
  HRichCalPar calpar;
  rtdb->addContainer(&calpar);

  rtdb->initContainers(944433336); // explicit containers initialisation

 anapar->printParam();
 anapar->setChanged();
 mappar->setChanged();
 digipar->setChanged();
 geopar->setChanged();
 calpar->setChanged();

  rtdb->saveOutput();
  rtdb->print();
  rtdb->closeFirstInput();
  rtdb->closeSecondInput();
  rtdb->closeOutput();
  delete gHades;
}
 













