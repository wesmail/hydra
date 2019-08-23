/***************************************************************************
 *   Author :  Tiago Perez <tiago.perez@exp2.physik.uni-giessen.de>        *
 *             II Physikalisches Institut                                  *
 *             Universitaet Giessen                                        *
 *            								   *
 *   Title  :  fillHyContExp.cc                                            *
 *   Project:  HYDRA Event Hypothesys  (HADES)                             *
 *   Description:   						           *
 *            								   *
 *   Modified :                                                            *
 *             2005.02.11   T. Perez  Created                              *
 *                                                                         *
 *            								   *
 ***************************************************************************/
#include "fillPidHypContSim.h"
#include "hhypreconstructor.h"
#include "hparasciifileio.h"
#include "hpidparcont.h"
#include "hpidtofrec.h"

using namespace std;

char binary_file[1000];
FILE *schedule_fp;
static const int MAX_FILES = 10000;


int get_next_file(void) {
  if (fscanf(schedule_fp, "%s\n", binary_file)==EOF) return 0;
  while (*binary_file =='#')  //This is for comments
    {
      if (fscanf(schedule_fp, "%s\n", binary_file)==EOF) return 0;
    }
  return 1;
}


int fillPidHypContSim(TString inDir, TString inFile, Int_t nEvents=0, Int_t startEvt=0)
{
  // real or simulation
  char *context="simulation"; 

  //TString paramFile1="/home/mdestefa/Hydra/dev2005.07.14_r4.04.02d/pidparameters_tiago/pp_pidparams_jan04_tiago01_SPLINE.root";

  TString paramFile1="/home/ingo/hades/ana/hydra/pid_par_jan04_c.root";
  TString paramFile2="/home/jacek/hydra/dev/params/allparams.root";
  TString paramFile3="/home/ingo/hades/ana/hydra/hyp/testParams.txt";

  
  TString outDir = "/d/c0/hypdata/ingo/"; 
  TString outDirNtup = "/home/ingo/hades/ana/hydra/"; 

  TString inputFile=inFile+".root";

  TString outFile=outDir+inFile+"_hyp.root";
  TString pidNtFile=outDir+inFile+"_pid_ntup.root";
  TString hypNtFile=outDirNtup+inFile+"_hyp_ntup.root";

  if (inFile.EndsWith(".schedule")) {
    TString tmp = inFile;
    tmp.Resize(tmp.Length() - 9);

    outFile=outDir+tmp+"_hyp.root";
    pidNtFile=outDir+tmp+"_pid_ntup.root";
    hypNtFile=outDirNtup+tmp+"_hyp_ntup.root";
  }

  TFile * hfile = new TFile(hypNtFile,"RECREATE","Demo ROOT file with hyp ntupla");

  TStopwatch timer;
  Int_t evN=0;

  // set gHades
  if(gHades == NULL) new Hades;
  gHades->setTreeBufferSize(8000);

  cout<< " Setting configuration..." << endl; 
  //Create hades
  if (!gHades) new Hades;
  gHades->setTreeBufferSize(8000);

  HSpectrometer* spec = gHades->getSetup();
  spec->addDetector(new HMdcDetector);

  //pid tasks
  //Int_t nRunId=9;
  Int_t nRunId=501;

  // Stefano task
  HPidTofRec *pidtofrec = new
    HPidTofRec("tofreconstructor","tofreconstructor","chiin");
  pidtofrec->setQualityRichParams(200.,4.,2.8,5);
  HTaskSet * tofrectask = new HTaskSet("","");
  tofrectask->connect(pidtofrec);

  HTask  *pTrackFiller = new HPidTrackFiller("makeSimCategory,nochi2seg1,SKIPNOTOFJAN04");
  HPidReconstructor *pPidRec = new HPidReconstructor("pdf,CL,merge,ALG_RUNGEKUTTA");
  Short_t nParticles[7]={-1,-2,2,3,8,9,14};
  pPidRec->setParticleIds(nParticles,sizeof(nParticles)/sizeof(Short_t));
  //pPidRec->addAlgorithm(new HPidAlgMomVsBeta());
  pPidRec->addAlgorithm(new HPidAlgHardCuts("test_pid_frame.root"));

  HPidParticleFiller *pPartFiller = new HPidParticleFiller("RUNGEKUTTA,MAKESIMCATEGORY");
  pPartFiller->setAlgorithm(9);
  pPartFiller->print();
  
  
  // event hypothesis tasks
  TLorentzVector * beam = new TLorentzVector(0,0,2994.73,4076.54);

  
  HHypReconstructor * hyp = new HHypReconstructor("name","title","skipempty");
  //HHypReconstructor * hyp = new HHypReconstructor("name","title","");

  hyp->SetBeam(beam);

  //FILL
  hyp->AddAlgorithm("empty",new HHypPPPipPimXAlg("inclusive pion filler",""),"PPPipPim");
  hyp->AddAlgorithm("empty",new HHypPPEpEmXAlg("leptons",""),"PPEpEm");

  //NO-CUT-Projectors
  hyp->AddAlgorithm("PPPipPim",new HHypPPPipPimProjector("exclusive pion projector"),"ntuple", hfile);
  hyp->AddAlgorithm("PPEpEm",new HHypPPEpEmProjector("exclusive lepton projector"),"ntuple", hfile);

#if 1
  //SELECT
  hyp->AddAlgorithm("PPPipPim",new HHypDeltaTofAlg("dtof for pions"),"PPPipPim_dtof",hfile);
  hyp->AddAlgorithm("PPPipPim_dtof",new HHypPPPipPimPi0Alg("exclusive pion selector"),"PPPipPim_dtof_Pi0");

  //DEBUG....The other way around... Result MUST be the same
  hyp->AddAlgorithm("PPPipPim",new HHypPPPipPimPi0Alg("exclusive pion selector"),"PPPipPim_Pi0");
  hyp->AddAlgorithm("PPPipPim_Pi0",new HHypDeltaTofAlg("dtof for pions"),"PPPipPim_Pi0_dtof",hfile);

  //HARD
  hyp->AddAlgorithm("PPPipPim",new HHypHardCutsAlg("hard cuts","cutfile=test_pid_frame.root"),"PPPipPim_hard");
  hyp->AddAlgorithm("PPPipPim_hard",new HHypPPPipPimPi0Alg("exclusive pion selector"),"PPPipPim_hard_Pi0");

  //PROJECT
  hyp->AddAlgorithm("PPPipPim_dtof",new HHypPPPipPimProjector("inclusive pion projector"),"ntuple", hfile);
  hyp->AddAlgorithm("PPPipPim_dtof_Pi0",new HHypPPPipPimProjector("exclusive pion projector"),"ntuple", hfile);
  hyp->AddAlgorithm("PPPipPim_hard_Pi0",new HHypPPPipPimProjector("inclusive pion projector"),"ntuple", hfile);

  //Elastic
  // hyp->AddAlgorithm("empty",new HHypPPXAlg("pp filler"),"PP");
  // hyp->AddAlgorithm("PP",new HHypPPElasticAlg("pp elastic selector"),"PP_elastic");
  // hyp->AddAlgorithm("PP_elastic",new HHypPPProjector("pp elastic projector"),"ntuple", hfile);
  // hyp->AddAlgorithm("PP",new HHypPPProjector("pp projector"),"ntuple", hfile);

  //FILL



  //SELECT
  //hyp->AddAlgorithm("PPEpEm",new HHypDeltaTofAlg("dtof for leptons"),"PPEpEm_dtof",hfile);

  //PROJECT
  //hyp->AddAlgorithm("PPEpEm_hard",new HHypPPEpEmProjector("inclusive pion projector"),"ntuple", hfile);
#endif  


  hyp->AddQA(hfile,"trqa");


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
  source->setGlobalRefId(nRunId);

  //parameter files
  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  HParRootFileIo *input1=new HParRootFileIo;
  input1->open((Text_t*)paramFile1.Data(),"READ");
  rtdb->setFirstInput(input1);


  HParRootFileIo *input2=new HParRootFileIo;
  input2->open((Text_t*)paramFile2.Data(),"READ");
  rtdb->setSecondInput(input2);

  HParAsciiFileIo *input3=new HParAsciiFileIo;
  input3->open((Text_t*)paramFile3.Data(),"in");
  rtdb->setMoreInputs(input3);

  HPidParCont *pPar1 = (HPidParCont*)rtdb->getContainer("PidAlgStandCutsPar");
  pPar1->setParamContext("HPidAlgStandCutsContextSPLINE");



  //------------------------ Add PID Tasks -----------------------------

  gHades->getTaskSet(context)->add(tofrectask);
  gHades->getTaskSet(context)->add(pTrackFiller);
  gHades->getTaskSet(context)->add(pPidRec);
  gHades->getTaskSet(context)->add(pPartFiller);
  gHades->getTaskSet(context)->add(hyp);
  //gHades->getTaskSet(context)->add(pidNtFiller);

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


#ifndef __CINT__
int main(int argc, char **argv)
{
  //TROOT AnalysisDST("HADES Hyp","Analysis of 4 track  Events");
  
  switch(argc)
  {
    case 3:
      return fillPidHypContSim(TString(argv[1]),TString(argv[2]));
      break;
    case 4:
      return fillPidHypContSim(TString(argv[1]),TString(argv[2]),atoi(argv[3]));
      break;
    case 5:
      return fillPidHypContSim(TString(argv[1]),TString(argv[2]),atoi(argv[3]),atoi(argv[4]));
      break;
    default:
    break;
   }
  return 0;
}
#endif
