{
  //Takes the geant output and distilles the relevant information into an
  //ntuple. The generated ntuple is the basis for the further parameterization

Text_t parFile[]="allgeom.021200.root"; //Old TOF geometry
//Text_t parFile2[]="/home/manuel/hades/params/mdc_kick_sim.apr01.txt";
Text_t parFile2[]="/home/manuel/hades/params/kick2.m3pos.meta.thin.sims.0702.txt";
gHades->setOutputFile("out.pos.sims.notargnorich.meta.lowfield.300902.root","RECREATE","utils",2);
Bool_t useTofResolution=kFALSE;
Bool_t useMdcResolution=kFALSE;
TString mode="meta";  //"meta", "mdc2_slope", "mdc2_kickpos", "mdc2_tofmdc"

Text_t *inFiles[] = {
  "/mnt/scratch/data/reft0_pos.low1.root",
  "/mnt/scratch/data/reft0_pos_uni.low1.root",
  NULL};

//***********************************************
 HMdcDetector *mdc=new HMdcDetector;
 Int_t mdcMods[4]={1,1,1,1};
 for (int i=0;i<6;i++) mdc->setModules(i,mdcMods);
 gHades->getSetup()->addDetector(mdc);

 HTofDetector *tof=new HTofDetector;
 Int_t tofMods[22]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
 for (int i=0;i<6;i++) tof->setModules(i,tofMods);
 gHades->getSetup()->addDetector(tof);

 HRootSource *datos=new HRootSource;
Text_t **filename=inFiles;
datos->setRefId(1,0);
for (filename=inFiles; *filename != NULL; filename++) {
  cout << "Adding file: " << *filename << endl;
  datos->addFile(*filename);
}

 gHades->setDataSource(datos);

 HParRootFileIo *input=new HParRootFileIo;
 input->open(parFile,"READ");
 gHades->getRuntimeDb()->setSecondInput(input);

 HParAsciiFileIo *input2=new HParAsciiFileIo;
 input2->open(parFile2,"in");
 gHades->getRuntimeDb()->setFirstInput(input2);
 
 HTaskSet *ts=gHades->getTaskSet("simulation");
 HKickResolution *resol=0;

if (mode.Contains("mdc2")) {
  HKickResolutionMdc2 *rmdc2 = new HKickResolutionMdc2("kick.resol",
						       "kick.resol");
  if (mode.Contains("kickpos")) rmdc2->useKickPosition();
  if (mode.Contains("mdctof")) rmdc2->useMdcTof();

  resol = rmdc2;
} else {
  HKickResolutionTof *rtof = new HKickResolutionTof("kick.resol",
						    "kick.resol");
  resol = rtof;  
}
 resol->setUsingTofResolution(useTofResolution);
 resol->setUsingMdcResolution(useMdcResolution);

 ts->connect(resol);

 if (!gHades->init()) {
	cout << "Error durante inicializacion\n";
	return 1;
 }

 HEvent *ev = gHades->getCurrentEvent();
gHades->makeCounter(1000);


 //gHades->setSplitLevel(2);
 //gHades->setOutputFile(outFile,"RECREATE","utils",2);
 //gHades->makeTree();
}
