#include "pidLepExp.h"

using namespace std;

Int_t pidLepExp(TString inFile, Int_t nEvents=0, Int_t startEvt=0)
{
  // real or simulation
  Char_t *context="real"; 

  // parameter files
  TString paramFile1="/d/hades/jacek/hydra_dev_sarge/params/pidParams_HardCuts_aug04_exp_kick.root";
  TString paramFile2="";

  Bool_t ora_input = kTRUE;

  // input and output data files
  TString inDir   = "/d/hades10/data/jacek/aug04/dst/newShower/";
  TString outDir  = "/d/hades10/data/jacek/aug04/pid/";
  
  TString inputFile=inFile+".root";
  TString outFile=outDir+inFile+"_pid.root";
  TString pidNtFile=outDir+inFile+"_pid_ntup.root";

  TStopwatch timer;
  Int_t evN=0;

  // set gHades
  if(gHades == NULL) new Hades;
  gHades->setTreeBufferSize(8000);

  //pid tasks
  Int_t nRunId=621306858;

  // pidtrack filler 
  HTask  *pTrackFiller = new HPidTrackFiller("nochi2seg1,SKIPNOLEPTON");

  // Spline or kick momentum reconstruction
  HPidReconstructor *pPidRec = new HPidReconstructor("pdf,CL,merge,ALG_RUNGEKUTTA");
  //HPidReconstructor *pPidRec = new HPidReconstructor("pdf,CL,merge,ALG_SPLINE");
  //HPidReconstructor *pPidRec = new HPidReconstructor("pdf,CL,merge,ALG_KICK");

  Short_t nParticles[2]={2,3};
  pPidRec->setParticleIds(nParticles,sizeof(nParticles)/sizeof(Short_t));

  // HPidAlgStandCuts algorithm with and without control ntuple
  // standard cuts algorithms
  // RICHRINGCUTS,TOFPVSBETA,TOFINOPVSBETA,SHOWERMAXFVSP,SHOWERSUM0VSP,MDCDEDX,SHOWERSUMDIFFVSP
  // 
  // default setting
  // RICHRINGCUTS,TOFPVSBETA,TOFINOPVSBETA,SHOWERMAXFVSP,SHOWERSUM0VSP
  //
  // seting for aug04exp
  // RICHRINGCUTS,MDCDEDX,TOFPVSBETA,TOFINOPVSBETA,SHOWERSUMDIFFVSP
  
  //HPidAlgStandCuts *pPidAlgStandCuts = new HPidAlgStandCuts();
  //HPidAlgStandCuts *pPidAlgStandCuts = new HPidAlgStandCuts(pidNtFile);
  HPidAlgStandCuts *pPidAlgStandCuts = new HPidAlgStandCuts(pidNtFile,"RICHRINGCUTS,MDCDEDX,TOFPVSBETA,TOFINOPVSBETA,SHOWERSUMDIFFVSP");
  pPidRec->addAlgorithm(pPidAlgStandCuts);

  // HPidParticleFiller for experiment
  HPidParticleFiller *pPartFiller = new HPidParticleFiller("RUNGEKUTTA");
  //HPidParticleFiller *pPartFiller = new HPidParticleFiller("SPLINE");
  //HPidParticleFiller *pPartFiller = new HPidParticleFiller("KICK");
  
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

  if (!paramFile1.IsNull())
  {
    HParRootFileIo *input1=new HParRootFileIo;
    input1->open((Text_t*)paramFile1.Data(),"READ");
    rtdb->setFirstInput(input1);
  }

  if (!paramFile2.IsNull())
  {
    HParRootFileIo *input2=new HParRootFileIo;
    input2->open((Text_t*)paramFile2.Data(),"READ");
    rtdb->setSecondInput(input2);
  }

  if(ora_input)
  {
    HParOraIo *ora=new HParOraIo() ;
    ora->open();
    ora->setHistoryDate("now");

    if (!ora->check())
    {
      Error("ora->check","no connection to Oracle \n exiting...!");
      exit(EXIT_FAILURE);
    }

    if (paramFile1.IsNull() && paramFile2.IsNull())
    {
      rtdb->setFirstInput(ora);
    }
    else
    {
      rtdb->setSecondInput(ora);
    }
  }

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
}// END Int_t fill(TString, Int_t , Int_t)


#ifndef __CINT__
Int_t main(Int_t argc, Char_t **argv)
{
  TROOT AnalysisDST("HADES Hyp","Analysis of 4 track  Events");
  
  switch(argc)
  {
    case 2:
      return pidLepExp(TString(argv[1]));
      break;
    case 3:
      return pidLepExp(TString(argv[1]),atoi(argv[2]));
      break;
    case 4:
      return pidLepExp(TString(argv[1]),atoi(argv[2]),atoi(argv[3]));
      break;
    default:
    break;
   }
  return 0;
}
#endif
