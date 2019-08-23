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

#define USEORA    1
#define WRITE_ORA_TO_FILE   0

#include "fillHypDstApr07.h"
#include "hhypreconstructor.h"
#include "hparasciifileio.h"
#include <getopt.h>

#include "hpidtrackcleaner.h"

// here you have to include all algorythms you want to use
#include "hhypAllFillerAlg.h"
#include "hhypAllProjector.h"
#include "hhypXKine.h"
#include "hhypRemoveAllButBest.h"

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
TString hypNtFile;
TString standardHypOption;

Int_t   nEvents;
Int_t   startEvt;
Int_t   hypLevel;
Bool_t  simflag;
Bool_t    substitutehypflag;
TFile * hfile;
Int_t nRunId;
Int_t qaLevel;

void EmptyFunction(HHypList *mylist,TLorentzVector *beam)
{
}

void add_hyp(HHypReconstructor * hyp) {
  // event hypothesis tasks
  hfile = new TFile(hypNtFile,"RECREATE","HYP ntuple");
	cout << "======== HypLevel: " <<hypLevel<< " =============="<<endl;
  if (hypLevel < 1000) { //>1000 maybe for differt beamtimes
    Int_t beam_pid=14, target_pid=14;// PIDs for Beam and Target
    TVector3 mymom( 0, 0, 2994.73);// ideal beam in MeV
//    TVector3 mymom(-5.011, 6.559, 2994.717);// real(?) beam in MeV
//    TLorentzVector * beam = new TLorentzVector(0,0,1976.91,3126.54);   // beam + target !!!!!!!!!!
    Double_t ta, pr;// Masses
    ta=HPidPhysicsConstants::mass(target_pid);
    pr=HPidPhysicsConstants::mass(beam_pid);
    TLorentzVector * beam = new TLorentzVector(mymom,ta+TMath::Sqrt(mymom.Mag2()+pr*pr));
    hyp->SetBeam(beam);
    hyp->SetBeamPID(beam_pid);
    hyp->SetTargetPID(target_pid);
    HypMomErr.setJan04();
    cout << "Set Beam and Target for Jan04 pp @ 2.2GeV"<<endl;
    cout << "Vect("<<mymom.X()<<","<< mymom.Y()<<","<< mymom.Z()<<")"<<endl;
  }else if (hypLevel < 2000) { //>2000 maybe for differt beamtimes
    Int_t beam_pid=14, target_pid=14;// PIDs for Beam and Target
    TVector3 mymom( 0, 0, 4337.96);// ideal beam in MeV
    Double_t ta, pr;// Masses
    ta=HPidPhysicsConstants::mass(target_pid);
    pr=HPidPhysicsConstants::mass(beam_pid);
    TLorentzVector * beam = new TLorentzVector(mymom,ta+TMath::Sqrt(mymom.Mag2()+pr*pr));
    hyp->SetBeam(beam);
    hyp->SetBeamPID(beam_pid);
    hyp->SetTargetPID(target_pid);
    HypMomErr.setApr07();
    cout << "Set Beam and Target for Apr07 pp @ 3.5GeV"<<endl;
    cout << "Vect("<<mymom.X()<<","<< mymom.Y()<<","<< mymom.Z()<<")"<<endl;
  }else if (hypLevel < 3000) { //>3000 maybe for differt beamtimes
    Int_t beam_pid=14, target_pid=14;// PIDs for Beam and Target
    TVector3 mymom( 0, 0, 1976.912);// ideal beam in MeV
    Double_t ta, pr;// Masses
    ta=HPidPhysicsConstants::mass(target_pid);
    pr=HPidPhysicsConstants::mass(beam_pid);
    TLorentzVector * beam = new TLorentzVector(mymom,ta+TMath::Sqrt(mymom.Mag2()+pr*pr));
    hyp->SetBeam(beam);
    hyp->SetBeamPID(beam_pid);
    hyp->SetTargetPID(target_pid);
    HypMomErr.setApr06();
    cout << "Set Beam and Target for Apr06 pp @ 1.25GeV"<<endl;
    cout << "Vect("<<mymom.X()<<","<< mymom.Y()<<","<< mymom.Z()<<")"<<endl;
  }else{
	cerr<< "No Beam/Target for this hyplevel ("<< hypLevel <<") defined! exit."<<endl;
	exit(0);
  }

  if (hypLevel == 0) {
			////////////////////////////
			//PP analysis 2.2GeV, 1st step
			////////////////////////////
    }  else if (hypLevel == 999) {
			////////////////////////////
			//PP analysis 2.2GeV, last level
			////////////////////////////
    }  else if (hypLevel == 1000) {
			////////////////////////////
			//PP analysis 3.5GeV, 1st step
			////////////////////////////
    }  else if (hypLevel == 1999) {
			////////////////////////////
			//PP analysis 3.5GeV, last level
			////////////////////////////
    }  else if (hypLevel == 2000) {
			////////////////////////////
			//PP analysis 1.25GeV, 1st step
			////////////////////////////
    }  else if (hypLevel == 2999) {
			////////////////////////////
			//PP analysis 1.25GeV, last level
			////////////////////////////
    }  else if (hypLevel == 9999) {
			// some easy, but not very useful example
			// Select all events containing p pi+ pi-, hypothesis of a missing p
			// use "standard" track selection (cleaning has to be run before!)
    	hyp->AddAlgorithm("empty",new HHypAllFillerAlg("all Filler",standardHypOption+
        ",cleaner=offical,Reactions="
        "(p pi+ pi- ; p )"
        ),"channelname");
			// Project ntuple
			hyp->AddAlgorithm("channelname",new HHypAllProjector("all projector",
				"TRIGGER,LORENTZ,DTOF_REFIT"), "ntuple", hfile);
			// do a kinematic fitting on that reaction
			hyp->AddAlgorithm("channelname",new HHypXKine("kine refit"), "channelname_KineX",hfile);
			// Project ntuple
			hyp->AddAlgorithm("channelname_KineX",new HHypAllProjector("all projector",
				"TRIGGER,LORENTZ,DTOF_REFIT,MDC_VERTEX"), "ntuple", hfile);
			// select only best combination
			hyp->AddAlgorithm("channelname_KineX",new HHypRemoveAllButBest("kine refit"),
				"channelname_KineX_best");
			// Project ntuple
			hyp->AddAlgorithm("channelname_KineX_best",new HHypAllProjector("all projector",
				"TRIGGER,LORENTZ,DTOF_REFIT,MDC_VERTEX"), "ntuple", hfile);
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

void GetBaseName(TString & BaseName, TString & FilePath)
{
//////////////////////////////////////////////////////////////////////////////
	TString fp = FilePath;
	Int_t n1, n2;

	n1 = 0;
	while (1) {
		if ((n2 = fp.Index("/", n1)) == -1) break;
		n1 = n2 + 1;
	}
	BaseName = FilePath;
	BaseName = BaseName(n1, BaseName.Length() - n1);
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

  TString inputFile;//=inFile+".root";
  TString outFile;//=outDir+inFile+"_hyp.root";

  cout << "Store HYP-DSTs in: " << outDir << endl;

  TString pidNtFile;//=outDir+inFile+"_pid_ntup.root";
  TString pdfNtFile;//=outDir+inFile+"_pdf_ntup.root";
  // hypNtFile=outDirNtup+inFile+"_hyp_ntup.root";

  {// create all the filenames
    TString tmp;

	cout <<"----- check for "<<inFile <<" -------"<< endl;
    if (inFile.EndsWith(".schedule")) {// schedule file
      tmp=gSystem->BaseName(inFile.Data());
      tmp.Resize(tmp.Length() - 9);
	}else if (inFile.EndsWith(".root")) {// end with .root
   	   tmp=inFile;
			tmp.Resize(tmp.Length() - 5);
      	//tmp=BaseName(inFile.Data());
      	inputFile=tmp+".root";
    }else{
		// then its a .root file without a ".root"
		// or a .schedule file without a ".schedule"
			FileStat_t a;
			// now we check if files exist
         if( gSystem->GetPathInfo(inFile+".schedule",a)==0){
				cout <<"----- Schedule file -------"  << endl;
		      tmp=gSystem->BaseName(inFile.Data());
				inFile+=".schedule";
				cout <<inFile << endl;
			}else if(gSystem->GetPathInfo(inFile+".root",a)==0){
				cout <<"----- Root file -------" << endl;
	   	   tmp=inFile+".root";
				tmp.Resize(tmp.Length() - 5);
	      	inputFile=tmp+".root";
				cout <<inFile << endl;
			}else{
				cout <<"----- File not found -------" <<endl;
	   	   tmp=inFile+".root";
				tmp.Resize(tmp.Length() - 5);
	      	inputFile=tmp+".root";
				cout <<"Guess its a root file: " <<inputFile << endl;
			}
    }
    outFile=outDir+tmp+"_hyp.root";
    pidNtFile=outDir+tmp+"_pid_ntup.root";
    pdfNtFile=outDir+tmp+"_pdf_ntup.root";
    hypNtFile=outDirNtup+tmp+"_hyp_ntup.root";
  }

  if (outDir.EndsWith("null")){
    outFile="/dev/null";
    pdfNtFile="/dev/null";
  }

  TStopwatch timer;
  Int_t evN=0;

  // set gHades
  if(gHades == NULL) new Hades;
  gHades->setTreeBufferSize(8000);

  cout<< " Setting configuration..." << endl;

  HSpectrometer* spec = gHades->getSetup();

  {
    HPidTrackCleaner* cleaner = new HPidTrackCleaner();
    HPidTrackSorter::setIgnoreRICH();
    HPidTrackSorter::setIgnoreInnerMDC();
    gHades->getTaskSet(context)->add(cleaner);
  }


  // now comes the (main) hyp tasks
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
  }else{// root file
    source->addFile((Text_t*)inputFile.Data());
  }

  gHades->setDataSource(source);
  if (nRunId) {
    source->setGlobalRefId(nRunId);
    source->setRefId(nRunId,nRunId);
  }

   //------------ UPDATE these categories to the Tree -------------

  if(substitutehypflag){
		Cat_t SubstituteHypCat[] =
		{
			catHypList, catHypComb, catHypKine
		};
		for(UInt_t j=0;j<sizeof(SubstituteHypCat)/sizeof(Cat_t);j++)
			cout << source->disableCategory(SubstituteHypCat[j])<< endl;
  }

  //parameter files
  HRuntimeDb* rtdb=gHades->getRuntimeDb();

  if (paramFile1.EndsWith("txt")) {
    cout << "First Input is TXT "<< (Text_t*)paramFile1.Data() << endl;
    HParAsciiFileIo *input1=new HParAsciiFileIo;
    input1->open((Text_t*)paramFile1.Data(),"in");
    rtdb->setFirstInput(input1);
  }
#ifdef USEORA
#warning "compiled WITH ora"
  else if (paramFile1.EndsWith("ora")) {
    HParOraIo *ora=new HParOraIo() ;
    ora->open();
    ora->setHistoryDate("now");
//    ora->setHistoryDate("APR07_pp_gen1");
    if (!ora->check())
    {
       Error("ora->check","no connection to Oracle \n exiting...!");
       exit(EXIT_FAILURE);
    }
    rtdb->setFirstInput(ora);

#if WRITE_ORA_TO_FILE
//     rtdb->setOutput(ora);// was soll denn der hier????? Dicker Fehler???
     Text_t fname[]="/u/bspruck/hydra/hypmacros/hyppar_apr07.root";
     cout <<"Set Output " << fname<<endl;
//      {
//        if (!rtdb->makeParamFile(fname.Data(),"apr07","07-MAY-2007 22:02:00","07-MAY-2007 22:10:00")) {
//          delete gHades;
//          exit(EXIT_FAILURE);
//        }
//      }
     HParRootFileIo* output=new HParRootFileIo;
     output->open(fname,"recreate");
     rtdb->setOutput(output);
#endif

  }
#else
#warning "compiled withOUT ora"
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
    if (!ora->check())
    {
       Error("ora->check","no connection to Oracle \n exiting...!");
       exit(EXIT_FAILURE);
    }
    rtdb->setSecondInput(ora);
  }
#endif
  else {
    HParRootFileIo *input2=new HParRootFileIo;
    input2->open((Text_t*)paramFile2.Data(),"READ");
    rtdb->setSecondInput(input2);
  }


  //------------------------ Add THE HYP Task -----------------------------


  gHades->getTaskSet(context)->add(hyp);

  gHades->getTaskSet(context)->print();

  //------------------------ Initialization ----------------------------
  cout<<"gHades->init()\n";

  gHades->makeCounter(1000);
  if(!gHades->init())
    cerr<<"Error gHades->init() returns false\n";


#if WRITE_ORA_TO_FILE
  cout <<"***************************************************"<<endl;
  cout <<"***************************************************"<<endl;
  cout <<"******************     write     ******************"<<endl;
//   rtdb->writeContainers();
//   rtdb->setContainersStatic();
  cout <<"***************************************************"<<endl;
  cout <<"***************************************************"<<endl;
#endif


  //Set output

  if (! (outDir.EndsWith("null") || outDir.EndsWith("none") || outDir=="")) {
    gHades->setOutputFile((Text_t*)outFile.Data(),"RECREATE","Test",2);
    gHades->makeTree();
  }

  //--------------------------------------------------------------------
	gHades->printDefinedTaskSets();
 	gHades->setQuietMode(0);
  cout<<"Processing events...\n";
  timer.Reset();
  timer.Start();
  if ((nEvents<1) && (startEvt == 0) ) {
    evN=gHades->eventLoop();
  } else {
    evN=gHades->eventLoop(nEvents,startEvt);
  }

  gHades->getTaskSet(context)->printTimer();
  hfile->Close();
  printf("rtdb deleted\n");

#if WRITE_ORA_TO_FILE
  cout <<"***************************************************"<<endl;
  cout <<"***************************************************"<<endl;
  cout <<"******************     write     ******************"<<endl;
//   rtdb->saveOutput();
//   rtdb->writeContainers();
//   rtdb->setContainersStatic();
  cout <<"***************************************************"<<endl;
  cout <<"***************************************************"<<endl;
#endif
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

//   mein_qa->Write();
//   mein_file->Write();
//   mein_file->Close();
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
  cout << "-j,                     Discard all existing hyp categories" << endl;
  cout << "-d, --inputdir=<dir>    input directory" << endl;
  cout << "-o, --outputdir=<dir>   output HypDST directory" << endl;
  cout << "-n, --ntupledir=<dir>   output ntuple directory" << endl;
  cout << "-#, --numofevents=<int> number of events" << endl;
  cout << "-$, --startevent=<int>  start event" << endl;
  cout << "-?, --help              display this text" << endl;
}

#ifndef __CINT__
int main(int argc, char **argv)
{

  int myargc=1;
  char *myargv[MAXARGS];
  myargv[0] = argv[0]; //name itself

  cout << "Arguments given were:" <<endl;
  for(int i=0; i<argc; i++){
    cout << argv[i]<<" ";
  }
  cout << endl << ".........." << endl;

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
    {"standardHypOption", 1, 0 , 'h'},
    {"substitutehyp", 0, 0 , 'j'},
    {"refID", 1, 0 , 'r'},
    {"hyplevel", 1, 0 , 'l'},
    {"qalevel", 1, 0 , 'q'},
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
  substitutehypflag = kFALSE;
  inFile = "nofile";
  inDir  = "road_to_nowhere";
  outDir = "road_to_nowhere";
  outDirNtup = "road_to_nowhere";
  hypLevel=0;
  nEvents=0;
  startEvt=0;
  nRunId=0;
  qaLevel=1; //use qa by default
  standardHypOption="";

  while ((c = getopt_long (myargc, myargv,
		":sj"
		"1:2:h:d:o:n:l:q:#:$:r:",
		long_options, &option_index)) != -1)
	{
    if (c == Int_t(':')) {cerr << "Missing argument to \""<<optarg<<"\"!"<<endl; return 1; }
    if (c == Int_t('?')) {usage();return 1; }

	 // Parameters without optpar
	 if (c == Int_t('j')) substitutehypflag=kTRUE;
    if (c == Int_t('s')) simflag = kTRUE;

    // Parameters requiring optpar
    if (c == Int_t('1')) paramFile1 = optarg;
    if (c == Int_t('2')) paramFile2 = optarg;
    if (c == Int_t('h')) standardHypOption = optarg;
    if (c == Int_t('d')) inDir = optarg;
    if (c == Int_t('o')) outDir = optarg;
    if (c == Int_t('n')) outDirNtup = optarg;
    if (c == Int_t('l')) sscanf(optarg,"%u",&hypLevel);
    if (c == Int_t('q')) sscanf(optarg,"%u",&qaLevel);
    if (c == Int_t('#')) sscanf(optarg,"%u",&nEvents);
    if (c == Int_t('$')) sscanf(optarg,"%u",&startEvt);
    if (c == Int_t('r')) sscanf(optarg,"%u",&nRunId);
  }
  if (optind == (myargc-1)) inFile = myargv[optind];
  else {cout << "no filename given!" << endl;usage();return 1; }

  cout << "start analyzing " << inFile << endl;

  return fillHypDst();

}
#endif
