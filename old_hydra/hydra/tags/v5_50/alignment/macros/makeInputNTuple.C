/////////////////////////////////////////////////////////////////////////
//
// This macro produce a file with a NTuple which is used by HMetaAligner
// As an input root file (inFile) should be used a file from DST without MF
// Use with analyze("makeInputNTuple.C", nEvents);
//
// M.Jaskula Thu Nov 29 21:47:36 CET 2001
//
/////////////////////////////////////////////////////////////////////////
// Input and Output files and directory

Float_t fShowerDX  = 0.0f;
Float_t fShowerDY  = 0.0f;
Float_t fShowerDZ  = 0.0f;
Float_t fShowerWindow = 300.0f;

TString inFile = "noField.root";
TString outDir = "aligment.root";
// Parameter input files for real data
TString RootParFile  = "currentParams.root";
TString AsciiParFile = "tracks.par";

//************************************************************************//
//*****                   Input preprocessing                          ***//
//************************************************************************//

TString parFile1,parFile2;

parFile1 = RootParFile;
parFile2 = AsciiParFile;

//*************************************************************************//
//*                      Hardcoded parameters                             *//
//*************************************************************************//

/////////////////////////////////////////////
// run number used to initialize rtdb
// Int_t refrunID = 1202;

//**************************************************************************//
//****                         Macro Code                                ***//
//**************************************************************************//

// set input data file
HRootSource *rsource =  new HRootSource;
rsource->addFile(inFile.Data());
//rsource->setRefId(rsource->getCurrentRunId(), 0);
//rsource->setRefId(987897909);//, 1202);
/////////////////////////////////////////////////////////////////////////
// Disable unwanted categories
rsource->disableCategory(catMdcCal1);
rsource->disableCategory(catMdcCal2);
rsource->disableCategory(catMdcHit);
rsource->disableCategory(catMdcRaw);
rsource->disableCategory(catRichCal);
rsource->disableCategory(catShowerCal);
rsource->disableCategory(catShowerRaw);
rsource->disableCategory(catStartCal);
rsource->disableCategory(catTofinoCal);

gHades->setDataSource(rsource);

Int_t showerMods[3] = {1,2,3};
Int_t tofMods[22]   = {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
Int_t tofinoMods[][]  = { {1},{1},{1},{1},{1},{1} };

Int_t mdcMods[6][4]=
  { {0,1,0,0},   // moduledef for sector 1
    {0,1,0,0},   // moduledef for sector 2
    {0,1,0,0},   // ...
    {0,1,0,0},
    {0,1,0,0},
    {0,1,0,0} };

/////////////////////////////////////////////////////////////////////////
// Detectors setup configuration

HSpectrometer* spec = gHades->getSetup();
HRuntimeDb* rtdb=gHades->getRuntimeDb();
//rtdb->addRun(refrunID);

HShowerDetector *shower=new HShowerDetector;
spec->addDetector(shower);

HTofDetector *tof=new HTofDetector;
spec->addDetector(tof);

HTofinoDetector *tofino = new HTofinoDetector;
spec->addDetector(tofino);

HMdcDetector *mdc=new HMdcDetector;
 for (Int_t i=0;i<6;i++) {mdc->setModules(i,mdcMods[i]);}
spec->addDetector(mdc);

for (Int_t is=0; is<6; is++) {
  shower->setModules(is,showerMods);
  tof->setModules(is,tofMods);
  tofino->setModules(is,tofinoMods[is]);
}

////////////////////////////////////////////////////////////////////////
// RuntimeDb inputs
 HParAsciiFileIo *asInput = new HParAsciiFileIo;
 asInput->open(parFile2.Data(),"in");
 rtdb->setFirstInput(asInput);

 HParRootFileIo *input=new HParRootFileIo;
 input->open(parFile1.Data(),"READ");
 rtdb->setSecondInput(input);

/////////////////////////////////////////////////////////////////////////
// Task list
HMetaAlignerTask2 *pAligner = new HMetaAlignerTask2(fShowerDX, fShowerDY,
                                        fShowerDZ, fShowerWindow);

gHades->getTaskSet("real")->connect(pAligner);

HSUProgressRec *pProgress = new HSUProgressRec("Progress", "Progress Bar");
    pProgress->SetMaxValue(nEvents);
    pProgress->SetSpinEvents(100);
    //pProgress->SetSpinChars(1);
    pProgress->SetSpinChars("* META aligner ");

    gHades->getTaskSet("real")->connect(pProgress, pAligner);

/////////////////////////////////////////////////////////////////////////
// Initialization
  if(!gHades->init()) {
     printf("\nError in Hades::init() \n");
     rtdb->closeFirstInput();
     rtdb->closeSecondInput();
     exit(2);
  }

/////////////////////////////////////////////////////////////////////////
// build output tree
  gHades->setOutputFile(outDir.Data(),"RECREATE","Test",2);
}
