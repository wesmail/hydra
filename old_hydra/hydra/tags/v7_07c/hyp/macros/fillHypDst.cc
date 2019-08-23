/***************************************************************************
 *   Author :  Tiago Perez <tiago.perez@exp2.physik.uni-giessen.de>        *
 *             II Physikalisches Institut                                  *
 *             Universitaet Giessen                                        *
 *            								   *
 *   Project:  HYDRA Event Hypothesys  (HADES)                             *
 *   Description:   						           *
 *            								   *
 *   Modified :                                                            *
 *             2005.02.11   T. Perez  Created                              *
 *             8.8.2004     Ingo (command line interface)                  *
 *            								   *
 ***************************************************************************/
#include "fillHypDst.h"
#include "hhypreconstructor.h"
#include "hparasciifileio.h"
#include "hpidparcont.h"
#include "hpidtofrec.h"
#include <getopt.h>

#define MAXARGS 200
#define MAXLINELEN 200


using namespace std;

char binary_file[MAXLINELEN];
FILE *schedule_fp;
static const int MAX_FILES = 10000;

TString paramFile1;
TString paramFile2;
TString inFile;
TString inDir;
TString outDir; 
TString outDirNtup; 
TString cutFile; 
TString hypNtFile;

Int_t   nEvents;
Int_t   startEvt;
Int_t   hypLevel;
Bool_t  simflag;
Bool_t    momvsbetaflag;
Bool_t    hardcutflag;
Bool_t    splineflag;
TFile * hfile;
Int_t nRunId;
Int_t qaLevel;

void add_hyp(HHypReconstructor * hyp) {
  // event hypothesis tasks
  hfile = new TFile(hypNtFile,"RECREATE","HYP ntuple");
  
  if (hypLevel < 100) { //>100 maybe for differt beamtimes
    TLorentzVector * beam = new TLorentzVector(0,0,2994.73,4076.54);
    hyp->SetBeam(beam);
  }

  TString cut_file_option = "cutfile=" + cutFile;

  if (hypLevel == 0) {
    ////////////////////////////
    //PP analysis, 1st step
    ////////////////////////////

    //FILL
    hyp->AddAlgorithm("empty",new HHypPPPipPimXAlg("inclusive pion filler",""),"PPPipPim");
    hyp->AddAlgorithm("empty",new HHypPPEpEmXAlg("leptons",""),"PPEpEm");
    //hyp->AddAlgorithm("empty",new HHypPPXAlg("2-body something","skipEvenIfExist"),"PP");
    //hyp->AddAlgorithm("PP",new HHypHardCutsAlg("2-body pp",cut_file_option.Data()),"PP_hard");

    //NO-CUT-Projectors
    hyp->AddAlgorithm("PPPipPim",new HHypPPPipPimProjector("exclusive pion projector"),"ntuple", hfile);
    hyp->AddAlgorithm("PPEpEm",new HHypPPEpEmProjector("exclusive lepton projector"),"ntuple", hfile);

  }

  else if (hypLevel == 1) {
    ////////////////////////////
    //PP eta analysis, 2nd step
    ////////////////////////////

    //DTOF
    hyp->AddAlgorithm("PPPipPim",new HHypDeltaTofAlg("dtof for pions"),"PPPipPim_dtof",hfile);
    hyp->AddAlgorithm("PPPipPim_dtof",new HHypPPPipPimPi0Alg("exclusive pion selector"),"PPPipPim_dtof_Pi0");
    
    //HARD
    hyp->AddAlgorithm("PPPipPim",new HHypHardCutsAlg("hard cuts",cut_file_option.Data()),"PPPipPim_hard");
    hyp->AddAlgorithm("PPPipPim_hard",new HHypPPPipPimPi0Alg("exclusive pion selector"),"PPPipPim_hard_Pi0");
    hyp->AddAlgorithm("PPEpEm",new HHypHardCutsAlg("hard cuts",cut_file_option.Data()),"PPEpEm_hard");
    hyp->AddAlgorithm("PPEpEm_hard",new HHypPPEpEmGammaAlg("exclusive lepton selector"),"PPEpEm_hard_Gamma");

    //PROJECT
    hyp->AddAlgorithm("PPPipPim_dtof",new HHypPPPipPimProjector("inclusive pion projector"),"ntuple", hfile);
    hyp->AddAlgorithm("PPPipPim_dtof_Pi0",new HHypPPPipPimProjector("exclusive pion projector"),"ntuple", hfile);
    hyp->AddAlgorithm("PPPipPim_hard_Pi0",new HHypPPPipPimProjector("inclusive pion projector"),"ntuple", hfile);
    hyp->AddAlgorithm("PPEpEm_hard_Gamma",new HHypPPEpEmProjector("exclusive lepton projector"),"ntuple", hfile);
    
    //Elastic
    // hyp->AddAlgorithm("empty",new HHypPPXAlg("pp filler"),"PP");
    // hyp->AddAlgorithm("PP",new HHypPPElasticAlg("pp elastic selector"),"PP_elastic");
    // hyp->AddAlgorithm("PP_elastic",new HHypPPProjector("pp elastic projector"),"ntuple", hfile);
    // hyp->AddAlgorithm("PP",new HHypPPProjector("pp projector"),"ntuple", hfile);

  }
  else if (hypLevel == 2) {
    ////////////////////////////
    //PP eta analysis, step: Lambda search
    ////////////////////////////

    //Hi Marco, add your filler, selectors and projectors here....
  }
  
  if (qaLevel == 2)
    hyp->AddQA(hfile,"trqa");
  else if (qaLevel == 1)
    hyp->AddQA(hfile);

}

//////////////////////////////////////////////////////////////////////////
// no need to change something below
//////////////////////////////////////////////////////////////////////////

int get_next_file(void) {
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  
  Bool_t loop = kTRUE;
  while (loop) {
    if ((read = getline(&line, &len, schedule_fp) == -1)) // EOF
      return 0;
    if (strlen(line)>2) {
      if (*line != '#') {
	loop = kFALSE;
	if (strlen(line) > MAXLINELEN) {
	  cout << "error: line too long: " << endl << line << endl;
	  return 0;
	}
	strcpy(binary_file,line);
	if (binary_file[strlen(binary_file)-1] == '\n')
	  binary_file[strlen(binary_file)-1] = 0;
      }
    }
  }
  return 1;
}


int fillHypDst(void)
{
  // real or simulation
  char *context;
  if (simflag)
    context="simulation"; 
  else
    context="real"; 

  cout << "Context is " << context << endl;

  TString inputFile=inFile+".root";
  TString outFile=outDir+inFile+"_hyp.root";  

  if (outDir.EndsWith("null")) outFile="/dev/null";

  TString pidNtFile=outDir+inFile+"_pid_ntup.root";
  hypNtFile=outDirNtup+inFile+"_hyp_ntup.root";

  if (inFile.EndsWith(".schedule")) {
    TString tmp = inFile;
    tmp.Resize(tmp.Length() - 9);

    outFile=outDir+tmp+"_hyp.root";
    pidNtFile=outDir+tmp+"_pid_ntup.root";
    hypNtFile=outDirNtup+tmp+"_hyp_ntup.root";
  }


  TStopwatch timer;
  Int_t evN=0;

  // set gHades
  if(gHades == NULL) new Hades;
  gHades->setTreeBufferSize(8000);

  cout<< " Setting configuration..." << endl; 

  HSpectrometer* spec = gHades->getSetup();
  spec->addDetector(new HMdcDetector);

  if (momvsbetaflag || hardcutflag) {

    // Stefano task
    HPidTofRec *pidtofrec = new
      HPidTofRec("tofreconstructor","tofreconstructor","chiin");
    pidtofrec->setQualityRichParams(200.,4.,2.8,5);
    HTaskSet * tofrectask = new HTaskSet("","");
    tofrectask->connect(pidtofrec);

    
    HTask  *pTrackFiller;

    if (simflag)
      pTrackFiller = new HPidTrackFiller("makeSimCategory,nochi2seg1,SKIPNOTOFJAN04");
    else 
      pTrackFiller = new HPidTrackFiller("nochi2seg1,SKIPNOTOFJAN04");

    HPidReconstructor *pPidRec;
    if (splineflag)
      pPidRec = new HPidReconstructor("pdf,CL,merge,ALG_SPLINE");
    else
      pPidRec = new HPidReconstructor("pdf,CL,merge,ALG_RUNGEKUTTA");

    if (hardcutflag) {
      Short_t nParticles[7]={-1,-2,2,3,8,9,14};
      pPidRec->setParticleIds(nParticles,sizeof(nParticles)/sizeof(Short_t));
    }
    else {
      Short_t nParticles[5]={-1,-2,8,9,14};
      pPidRec->setParticleIds(nParticles,sizeof(nParticles)/sizeof(Short_t));
    }

    if (momvsbetaflag) 
      pPidRec->addAlgorithm(new HPidAlgMomVsBeta());    
    if (hardcutflag)
      pPidRec->addAlgorithm(new HPidAlgHardCuts(cutFile.Data()));
    
    
    HPidParticleFiller *pPartFiller;
    if (simflag)
      if (splineflag)
	pPartFiller = new HPidParticleFiller("RUNGEKUTTA,MAKESIMCATEGORY");
      else
	pPartFiller = new HPidParticleFiller("SPLINE,MAKESIMCATEGORY");
    else
      if (splineflag)
	pPartFiller = new HPidParticleFiller("RUNGEKUTTA");
      else
	pPartFiller = new HPidParticleFiller("SPLINE");

    if (hardcutflag)
      pPartFiller->setAlgorithm(9);
    pPartFiller->print();

    gHades->getTaskSet(context)->add(tofrectask);
    gHades->getTaskSet(context)->add(pTrackFiller);
    gHades->getTaskSet(context)->add(pPidRec);
    gHades->getTaskSet(context)->add(pPartFiller);

  }

  HHypReconstructor * hyp = new HHypReconstructor("name","title","skipempty");  
  add_hyp(hyp);



  //Set batch (needed for TCanvas's)

  gROOT->SetBatch();

  //Add input files
  HRootSource *source=new HRootSource;
  source->setDirectory((Text_t*)inDir.Data());

  if (inFile.EndsWith(".schedule")) {
    //reads each file from schedule list
    
    if ((schedule_fp = fopen((char*)inFile.Data(), "r"))==NULL)
      {
	cout << "Binfile::Binfile -> Cannot open " << inFile.Data() << endl;
	return 1;
      }
    
    while (get_next_file()) {
      cout << "attaching file " << binary_file<<endl; 
      source->addFile(binary_file);
    }
  }
  else
    source->addFile((Text_t*)inputFile.Data());
  

  gHades->setDataSource(source);
  if (nRunId) source->setGlobalRefId(nRunId);

  //parameter files
  HRuntimeDb* rtdb=gHades->getRuntimeDb();

  if (momvsbetaflag) {
    HPidAlgMomVsBetaPar * momvsbeta = (HPidAlgMomVsBetaPar *) rtdb->getContainer("PidAlgMomVsBetaPar");
    if (splineflag)
      momvsbeta->setParamContext("HPidAlgMomVsBetaContextSPLINE");
    else
      momvsbeta->setParamContext("HPidAlgMomVsBetaContextRUNGEKUTTA");
  }

  if (paramFile1.EndsWith("txt")) {
    cout << "First Input is TXT "<< (Text_t*)paramFile1.Data() << endl;
    HParAsciiFileIo *input1=new HParAsciiFileIo;
    input1->open((Text_t*)paramFile1.Data(),"in");
    rtdb->setFirstInput(input1);
  } 
#ifdef USEORA
  else if (paramFile1.EndsWith("ora")) {
    HParOraIo *ora=new HParOraIo() ;
    ora->open();
    ora->setHistoryDate("now");
    rtdb->setFirstInput(ora);
  } 
#endif
  else {
    HParRootFileIo *input1=new HParRootFileIo;
    input1->open((Text_t*)paramFile1.Data(),"READ");
    rtdb->setFirstInput(input1);
  }
 
  if (paramFile2.EndsWith("txt")) {
    cout << "Second Input is TXT" << endl;
    HParAsciiFileIo *input2=new HParAsciiFileIo;
    input2->open((Text_t*)paramFile2.Data(),"in");
    rtdb->setSecondInput(input2);
  } 
#ifdef USEORA
  else if (paramFile2.EndsWith("ora")) {
    HParOraIo *ora=new HParOraIo() ;
    ora->open();
    ora->setHistoryDate("now");
    rtdb->setSecondInput(ora);
  } 
#endif
  else {
    HParRootFileIo *input2=new HParRootFileIo;
    input2->open((Text_t*)paramFile2.Data(),"READ");
    rtdb->setSecondInput(input2);
  }


#if 0
  HParAsciiFileIo *input3=new HParAsciiFileIo;
  input3->open((Text_t*)paramFile3.Data(),"in");
  rtdb->setMoreInputs(input3);
#endif

  HPidParCont *pPar1 = (HPidParCont*)rtdb->getContainer("PidAlgStandCutsPar");
  pPar1->setParamContext("HPidAlgStandCutsContextSPLINE");



  //------------------------ Add PID Tasks -----------------------------


  gHades->getTaskSet(context)->add(hyp);


  gHades->getTaskSet(context)->print();

  //------------------------ Initialization ----------------------------
  cout<<"gHades->init()\n";

  gHades->makeCounter(1000);
  if(!gHades->init())
    cerr<<"Error\n";


  //Set output
  gHades->setOutputFile((Text_t*)outFile.Data(),"RECREATE","Test",2);
  gHades->makeTree();

  //--------------------------------------------------------------------

  cout<<"Processing events...\n";
  timer.Reset();
  timer.Start();
  if (nEvents<1) {
    evN=gHades->eventLoop();
  } else {
    evN=gHades->eventLoop(nEvents,startEvt);
  }

  gHades->getTaskSet(context)->printTimer();

  hfile->Close();
  printf("rtdb deleted\n");
  /*
  gHades->getRuntimeDb()->removeContainer("PidTrackFillerPar");
  gHades->getRuntimeDb()->removeContainer("PidReconstructorPar");
  gHades->getRuntimeDb()->removeContainer("PidAlgMomVsBetaPar");
  */
  delete gHades;

  timer.Stop();

  cout<<"------------------------------------------------------\n";
  cout<<"Events processed: "<<evN<<endl;
  cout<<"Real time: "<<timer.RealTime()<<endl;;
  cout<<"Cpu time: "<<timer.CpuTime()<<endl;
  if (evN) cout<<"Performance: "<<timer.CpuTime()/evN<<endl;;

  return 0;
}// END int fill(TString, Int_t , Int_t)

void usage(void) {
  cout << "Usage: fillHypDst [CONFIGFILE] [OPTION]... [FILE]" << endl;
  cout << "Compresses a bunch of root-DSTs into one big HYP-DST" << endl;
  cout << "Making projections of HYP-DSTs into Ntuple" << endl;
  cout << endl;
  cout << "CONFIGFILE: A file containing the following options" << endl;
  cout << "            commandline option overwrite config" << endl;
  cout << endl;
  cout << "FILE:       A root-file (DST) or a schedule file (.schedule) " << endl;
  cout << endl;
  cout << "-s, --simulation        use simulation context" << endl;
  cout << "                        default is real" << endl;
  cout << "-1, --paramfile1=<file> paramfile 1 (default is 'ora')" << endl;
  cout << "-2, --paramfile2=<file> paramfile 2 (default is 'ora')" << endl;
  cout << "-r, --refID=<int>       Gobal ref ID" << endl;
  cout << "-l, --hyplevel=<int>    Level: 0 means produce HYP-DST, 1 projection step" << endl;
  cout << "-q, --qalevel=<int>     Level: 0: no QA, 1: QA, 2: TRQA" << endl;
  cout << "-M, --useMomVSBeta      PID->MomVSBeta" << endl;
  cout << "-H, --useHardCuts       PID->HardCuts" << endl;
  cout << "-c, --cutfile=<file>    Cutfile (for PID & HYP)" << endl;
  cout << "-S, --Spline            Spline (default is RungeKutta)" << endl;
  cout << "-d, --inputdir=<dir>    input directory" << endl;
  cout << "-o, --outputdir=<dir>   output HypDST directory" << endl;
  cout << "-n, --ntupledir=<dir>   output ntuple directory" << endl;
  cout << "-#, --numofevents=<int> number of events" << endl;
  cout << "-%, --startevent=<int>  start event" << endl;
  cout << "-?, --help              display this text" << endl;
}

#ifndef __CINT__
int main(int argc, char **argv)
{
  
  int myargc=1;
  char *myargv[MAXARGS];
  myargv[0] = argv[0]; //name itself

  if (argc<3) {
    usage();return 1;
  }

  TROOT AnalysisDST("HADES Hyp","Making HYP DSTs");

  //first open the config file
  TString optfile  = argv[1];

  if ((schedule_fp = fopen((char*)optfile.Data(), "r"))==NULL)
    {
      cout << "Optfile -> Cannot open " << optfile.Data() << endl;
      return 1;
    }
  
  while (get_next_file()) {
    //cout << "'" << binary_file << "'" << endl;
    myargv[myargc]= new char[strlen(binary_file)+1];
    strcpy(myargv[myargc], binary_file);
    myargc++;
    if (myargc == MAXARGS) {
      cout << "Optfile -> Too many lines in " << optfile.Data() << endl;
      return 1;
    }
  }


  //now attach the old argv
  for (int i=2; i<argc; i++) {
    myargv[myargc]= argv[i];
    //cout << "atach " << argv[i] << endl;
    myargc++;
    if (myargc == MAXARGS) {
      cout << "Too many otions in total" << endl;
      return 1;
    }
  }


  static struct option long_options[] = {
    {"simulation", 0, 0 , 's'},
    {"paramfile1", 1, 0 , '1'},
    {"paramfile2", 1, 0 , '2'},
    {"refID", 1, 0 , 'r'},
    {"hyplevel", 1, 0 , 'l'},
    {"qalevel", 1, 0 , 'q'},
    {"useMomVSBeta", 0, 0 , 'M'},
    {"useHardCuts", 0, 0 , 'H'},
    {"cutfile", 1, 0 , 'c'},
    {"Spline", 0, 0 , 'S'},
    {"inputdir", 1, 0 , 'd'},
    {"outputdir", 1, 0 , 'o'},
    {"ntupledir", 1, 0 , 'n'},
    {"numofevents", 1, 0 , '#'},
    {"startevent", 1, 0 , '$'},
    {"help", 0, 0 , '?'},
    {0, 0, 0, 0}
  };
  int option_index = 0;

 
  int c;
  simflag = kFALSE;
  paramFile1 = "ora";
  paramFile2 = "ora";
  momvsbetaflag = kFALSE;
  hardcutflag = kFALSE;
  splineflag = kFALSE;
  inFile = "nofile";
  inDir  = "road_to_nowhere";
  outDir = "road_to_nowhere";
  outDirNtup = "road_to_nowhere";
  hypLevel=0;
  nEvents=0;
  startEvt=0;
  nRunId=0;
  qaLevel=1; //use qa by default

  while ((c = getopt_long (myargc, myargv, "s1:2:l:MHi:d:o:n:#:$:Sc:r:q:", long_options, &option_index)) != -1) {

    if (c == Int_t('s')) simflag = kTRUE;
    if (c == Int_t('1')) paramFile1 = optarg;
    if (c == Int_t('2')) paramFile2 = optarg;
    if (c == Int_t('M')) momvsbetaflag = kTRUE;	
    if (c == Int_t('H')) hardcutflag = kTRUE;	
    if (c == Int_t('S')) splineflag = kTRUE;
    if (c == Int_t('d')) inDir = optarg;
    if (c == Int_t('o')) outDir = optarg;
    if (c == Int_t('n')) outDirNtup = optarg;
    if (c == Int_t('c')) cutFile = optarg;
    if (c == Int_t('l')) sscanf(optarg,"%u",&hypLevel);
    if (c == Int_t('q')) sscanf(optarg,"%u",&qaLevel);
    if (c == Int_t('#')) sscanf(optarg,"%u",&nEvents);
    if (c == Int_t('$')) sscanf(optarg,"%u",&startEvt);
    if (c == Int_t('r')) sscanf(optarg,"%u",&nRunId);
    if (c == Int_t('?')) {usage();return 1; }
  }
  if (optind == (myargc-1)) inFile = myargv[optind];
  else {cout << "no filename given!" << endl;usage();return 1; }

  cout << "start analyzing " << inFile << endl;

  return fillHypDst();

}
#endif
