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
#include "fillPidHypContExp.h"
#include "hhypreconstructor.h"

using namespace std;

int fillPidHypContExp(TString inDir, TString inFile, Int_t nEvents=0, Int_t startEvt=0)
{
  // real or simulation
  char *context="real"; 

  // parameter files
  TString paramFile1="/u/hadeshyp/ingo/pidparams_exp.root";
  TString paramFile2="/u/hadeshyp/ingo/hyp/testParams.txt";
  
  // input and output data files
  //TString inDir   = "/d/hades07/data/jan04_dstgen1/root/";
  TString outDir = "/dev/null/"; 
  TString outDirHyp = "/u/hadeshyp/ingo/"; 

  TString inputFile=inFile+".root";
  //TString outFile=outDir+inFile+"_hyp.root";
  TString outFile="/dev/null";

  //TString pidNtFile=outDir+inFile+"_pid_ntup.root";
  TString hypNtFile=outDirHyp+inFile+"_hyp_ntup.root";

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

  //pid tasks
  Int_t nRunId=524548104;

  HTask  *pTrackFiller = new HPidTrackFiller("nochi2seg1,SKIPNOTOFJAN04");
  HPidReconstructor *pPidRec = new HPidReconstructor("pdf,CL,merge,ALG_SPLINE");
  Short_t nParticles[7]={-1,-2,2,3,8,9,14};
  pPidRec->setParticleIds(nParticles,sizeof(nParticles)/sizeof(Short_t));
  pPidRec->addAlgorithm(new HPidAlgMomVsBeta());

  HPidParticleFiller *pPartFiller = new HPidParticleFiller("SPLINE");
  pPartFiller->print();
       
  //HPidPerformaceTestExp *pidNtFiller = new  HPidPerformaceTestExp(pidNtFile.Data());

  // event hypothesis tasks
  TLorentzVector * beam = new TLorentzVector(0,0,2994.73,4076.54);

  HHypReconstructor * hyp = new HHypReconstructor();
  hyp->SetBeam(beam);
  HHypChannel       hchannel;

  hyp->AddAlgorithm("empty",new HHypPPPipPimXAlg("inclusive pion selector"),"PPPipPim");

  hyp->AddAlgorithm("PPPipPim",new HHypPPPipPimPi0Alg("exclusive pion selector"),"PPPipPim_Pi0");
		    
  hyp->AddAlgorithm("PPPipPim",new HHypPPPipPimProjector("inclusive pion projector"),"ntuple", hfile);
  hyp->AddAlgorithm("PPPipPim_Pi0",new HHypPPPipPimProjector("inclusive pion projector"),"ntuple", hfile);

  //Elastic
  hyp->AddAlgorithm("empty",new HHypPPXAlg("pp selector"),"PP");
  hyp->AddAlgorithm("PP",new HHypPPElasticAlg("pp selector"),"PP_elastic");
  hyp->AddAlgorithm("PP_elastic",new HHypPPProjector("pp projector"),"ntuple", hfile);
  hyp->AddAlgorithm("PP",new HHypPPProjector("pp projector"),"ntuple", hfile);

  hyp->AddQA(hfile);

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
  //HParRootFileIo *input1=new HParRootFileIo;
  //input1->open((Text_t*)paramFile1.Data(),"READ");
  HParAsciiFileIo *input2=new HParAsciiFileIo;
  input2->open((Text_t*)paramFile2.Data(),"in");
  //rtdb->setSecondInput(input2);

  HPidParCont *pPar0 = (HPidParCont*)rtdb->getContainer("PidAlgMomVsBetaPar");
  //pPar0->setParamContext("HPidAlgRingPropertiesContextKICK");

  
  HParOraIo *ora=new HParOraIo() ;
  ora->open();
  ora->setHistoryDate("now");
  rtdb->setFirstInput(input2);
  rtdb->setSecondInput(ora);

  //------------------------ Add PID Tasks -----------------------------

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
  TROOT AnalysisDST("HADES Hyp","Analysis of 4 track  Events");
  
  switch(argc)
  {
    case 3:
      return fillPidHypContExp(TString(argv[1]),TString(argv[2]));
      break;
    case 4:
      return fillPidHypContExp(TString(argv[1]),TString(argv[2]),atoi(argv[3]));
      break;
    case 5:
      return fillPidHypContExp(TString(argv[1]),TString(argv[2]),atoi(argv[3]),atoi(argv[4]));
      break;
    default:
    break;
   }
  return 0;
}
#endif
