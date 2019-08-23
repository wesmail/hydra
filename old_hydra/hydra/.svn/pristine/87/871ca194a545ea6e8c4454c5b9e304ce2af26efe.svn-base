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

using namespace std;

int fillPidHypContSim(TString inFile, Int_t nEvents=0, Int_t startEvt=0)
{
  // real or simulation
  char *context="simulation"; 

  // parameter files
 TString paramFile1="/home/jacek/hydra/dev/params/pidparams_sim.root";
 TString paramFile2="/home/jacek/hydra/dev/params/allparams.root";

  // input and output data files
  TString inDir   = "/bigtmp/hadeshyp/dst/jan04/sim/";
  TString outDir = "/d/c2/hadeshyp/output/sim/"; 
  
  TString inputFile=inFile+".root";
  TString outFile=outDir+inFile+"_hyp.root";

  TString pidNtFile=outDir+inFile+"_pid_ntup.root";
  TString hypNtFile=outDir+inFile+"_hyp_ntup.root";

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

 /*
  Int_t mdcMods[6][4] =  { {1,1,1,1},
                           {1,1,1,1},
                           {1,1,1,0},
                           {1,1,1,1},
                           {1,1,1,1},
                           {1,1,1,0} };
			   */

  HSpectrometer* spec = gHades->getSetup();
  spec->addDetector(new HMdcDetector);

  //for (Int_t is=0; is<6; is++) spec->getDetector("Mdc")   ->setModules(is,mdcMods[is]);

  //pid tasks
  Int_t nRunId=9;

  // Stefano task
  HTofRec *tofrec = new HTofRec("tofreconstructor","tofreconstructor","simulation","");
  tofrec->setRichParams(1.56,2.18,0.,1.84,5.);
  tofrec->setQualityRichParams(200.,4.,2.8,5);
  HTaskSet * tofrectask = new HTaskSet("","");
  tofrectask->connect(tofrec);

  HTask  *pTrackFiller = new HPidTrackFiller("makeSimCategory,nochi2seg1,SKIPNOTOFJAN04");
  HPidReconstructor *pPidRec = new HPidReconstructor("pdf,CL,merge,ALG_SPLINE");
  Short_t nParticles[7]={-1,-2,2,3,8,9,14};
  pPidRec->setParticleIds(nParticles,sizeof(nParticles)/sizeof(Short_t));
  pPidRec->addAlgorithm(new HPidAlgMomVsBeta());

  HPidParticleFiller *pPartFiller = new HPidParticleFiller("SPLINE,MAKESIMCATEGORY");
  pPartFiller->print();
       
  //HPidPerformaceTestSim *pidNtFiller = new  HPidPerformaceTestSim(pidNtFile.Data());

  // event hypothesis tasks
  TLorentzVector * beam = new TLorentzVector(0,0,2994.73,4076.54);

  HHypReconstructor * hyp = new HHypReconstructor();
  hyp->SetBeam(beam);
  HHypChannel       hchannel;

  hyp->AddAlgorithm(
		    hchannel.Get("empty"),
		    new HHypPPPipPimXAlg("inclusive pion selector"),
		    hchannel.Get("pp_pip_pim_X")
		    );

   hyp->AddAlgorithm(
 		    hchannel.Get("pp_pip_pim_X"),
 		    new HHypPPPipPimPi0Alg("exclusive pion selector"),
 		    hchannel.Get("pp_pip_pim_pi0")
 		    );

   hyp->AddAlgorithm(
 		    hchannel.Get("pp_pip_pim_pi0"),
 		    new HHypPPPipPimProjector("inclusive pion projector"),
 		    hchannel.Get("empty") , hfile
 		    );

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
  HParRootFileIo *input2=new HParRootFileIo;
  input2->open((Text_t*)paramFile2.Data(),"READ");
  rtdb->setSecondInput(input2);

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
  TROOT AnalysisDST("HADES Hyp","Analysis of 4 track  Events");
  
  switch(argc)
  {
    case 2:
      return fillPidHypContSim(TString(argv[1]));
      break;
    case 3:
      return fillPidHypContSim(TString(argv[1]),atoi(argv[2]));
      break;
    case 4:
      return fillPidHypContSim(TString(argv[1]),atoi(argv[2]),atoi(argv[3]));
      break;
    default:
    break;
   }
  return 0;
}
#endif
