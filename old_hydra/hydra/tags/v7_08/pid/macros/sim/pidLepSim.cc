#include "pidLepSim.h"

using namespace std;

int pidLepSim(TString inFile, Int_t nEvents=0, Int_t startEvt=0)
{
  // real or simulation
  char *context="simulation"; 

  // parameter files
  TString paramFile1="/d/hades/jacek/hydra_dev/params/pidParams_HardCuts_Nov02_KICK_Sim.root";
  TString paramFile2="/d/hades/jacek/hydra_dev/params/pidParams_HardCuts_Nov02_KICK_Sim.root";

  // input and output data files
  TString inDir   = "/d/hades06/data/nov02/sim/dst/gen3/c2c/";
  TString outDir = "/s/jacek/"; 
  
  TString inputFile=inFile+".root";
  TString outFile=outDir+inFile+"_pid.root";
  TString pidNtFile=outDir+inFile+"_pid_ntup.root";

  TStopwatch timer;
  Int_t evN=0;

  // set gHades
  if(gHades == NULL) new Hades;
  gHades->setTreeBufferSize(8000);

  //pid tasks
  Int_t nRunId=501;

  HTask  *pTrackFiller = new HPidTrackFiller("nochi2seg1,MAKESIMCATEGORY,SKIPNOLEPTON");

  // Spline or kick momentum reconstruction
  //HPidReconstructor *pPidRec = new HPidReconstructor("pdf,CL,merge,ALG_SPLINE");
  HPidReconstructor *pPidRec = new HPidReconstructor("pdf,CL,merge,ALG_KICK");

  Short_t nParticles[2]={2,3};
  pPidRec->setParticleIds(nParticles,sizeof(nParticles)/sizeof(Short_t));

  // HPidAlgStandCuts algorithm with and without control ntuple
  HPidAlgStandCuts *pPidAlgStandCuts = new HPidAlgStandCuts(pidNtFile);
  //HPidAlgStandCuts *pPidAlgStandCuts = new HPidAlgStandCuts();
  pPidRec->addAlgorithm(pPidAlgStandCuts);

  // HPidParticleFiller for simulation
  //HPidParticleFiller *pPartFiller = new HPidParticleFiller("SPLINE,MAKESIMCATEGORY");
  HPidParticleFiller *pPartFiller = new HPidParticleFiller("KICK,MAKESIMCATEGORY");
  
  pPartFiller->setAlgorithm(7); // only hard cuts
  pPartFiller->print();
       

  //Set batch (needed for TCanvas's)

  gROOT->SetBatch();

  //Add input files
  HRootSource *source=new HRootSource;
  source->setDirectory((Text_t*)inDir.Data());
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

  //------------------------ Add PID Tasks -----------------------------

  gHades->getTaskSet(context)->add(pTrackFiller);
  gHades->getTaskSet(context)->add(pPidRec);
  gHades->getTaskSet(context)->add(pPartFiller);

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
  TROOT AnalysisDST("HADES Hyp","Analysis of 4 track  Events");
  
  switch(argc)
  {
    case 2:
      return pidLepSim(TString(argv[1]));
      break;
    case 3:
      return pidLepSim(TString(argv[1]),atoi(argv[2]));
      break;
    case 4:
      return pidLepSim(TString(argv[1]),atoi(argv[2]),atoi(argv[3]));
      break;
    default:
    break;
   }
  return 0;
}
#endif
