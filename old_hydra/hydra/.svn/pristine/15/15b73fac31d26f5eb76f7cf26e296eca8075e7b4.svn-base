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
#include "fillProjSim.h"
#include "hhypreconstructor.h"
#include "hparasciifileio.h"
#include "hpidparcont.h"

using namespace std;

int fillProjSim(TString inDir, TString inFile, Int_t nEvents=0, Int_t startEvt=0)
{
  // real or simulation
  char *context="simulation"; 

  // parameter files
  TString paramFile1="/home/ingo/hades/ana/hydra/pid_par_jan04_c.root";
  TString paramFile2="/home/jacek/hydra/dev/params/allparams.root";
  TString paramFile3="/home/ingo/hades/ana/hydra/hyp/testParams.txt";
  

  
  // input and output data files
  
  TString outDir = "/d/c0/hypdata/ingo/"; 
  TString outDirNtup = "/home/ingo/hades/ana/hydra/"; 
  
  TString inputFile=inFile+".root";
  TString outFile=outDir+inFile+"_proj.root";
  
  TString pidNtFile=outDir+inFile+"_pid_ntup.root";
  TString hypNtFile=outDirNtup+inFile+"_hyp_ntup.root";

  TFile * hfile = new TFile(hypNtFile,"RECREATE","Demo ROOT file with hyp ntupla");

  TStopwatch timer;
  Int_t evN=0;
  Int_t nRunId=501;

  // set gHades
  if(gHades == NULL) new Hades;
  gHades->setTreeBufferSize(8000);

  cout<< " Setting configuration..." << endl; 
  //Create hades
  if (!gHades) new Hades;
  gHades->setTreeBufferSize(8000);

  HSpectrometer* spec = gHades->getSetup();
  spec->addDetector(new HMdcDetector);

  // event hypothesis tasks
  TLorentzVector * beam = new TLorentzVector(0,0,2994.73,4076.54);

  HHypReconstructor * hyp = new HHypReconstructor();
  hyp->SetBeam(beam);

  //HARD
  hyp->AddAlgorithm("PPPipPim",new HHypHardCutsAlg("hard cuts","cutfile=test_pid_frame.root"),"PPPipPim_hard");
  hyp->AddAlgorithm("PPEpEm",new HHypHardCutsAlg("hard cuts","cutfile=test_pid_frame.root"),"PPEpEm_hard");

  //MISSING MASS
  hyp->AddAlgorithm("PPPipPim_hard",new HHypPPPipPimPi0Alg("exclusive pion selector"),"PPPipPim_hard_Pi0");
  hyp->AddAlgorithm("PPEpEm_hard",new HHypPPEpEmGammaAlg("exclusive lepton selector"),"PPEpEm_hard_Gamma");
    
  //PROJECT
  hyp->AddAlgorithm("PPPipPim_hard",new HHypPPPipPimProjector("exclusive pion projector"),"ntuple", hfile);
  hyp->AddAlgorithm("PPEpEm_hard",new HHypPPEpEmProjector("exclusive lepton projector"),"ntuple", hfile);

  hyp->AddAlgorithm("PPPipPim_hard_Pi0",new HHypPPPipPimProjector("exclusive pion projector"),"ntuple", hfile);
  hyp->AddAlgorithm("PPEpEm_hard_Gamma",new HHypPPEpEmProjector("exclusive lepton projector"),"ntuple", hfile);
  

  //hyp->AddQA(hfile,"trqa");


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

  HParAsciiFileIo *input3=new HParAsciiFileIo;
  input3->open((Text_t*)paramFile3.Data(),"in");
  rtdb->setMoreInputs(input3);

  HPidParCont *pPar1 = (HPidParCont*)rtdb->getContainer("PidAlgStandCutsPar");
  pPar1->setParamContext("HPidAlgStandCutsContextSPLINE");


  //------------------------ Add PID Tasks -----------------------------

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
      return fillProjSim(TString(argv[1]),TString(argv[2]));
      break;
    case 4:
      return fillProjSim(TString(argv[1]),TString(argv[2]),atoi(argv[3]));
      break;
    case 5:
      return fillProjSim(TString(argv[1]),TString(argv[2]),atoi(argv[3]),atoi(argv[4]));
      break;
    default:
    break;
   }
  return 0;
}

#endif
