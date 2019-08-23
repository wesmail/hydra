#include "projectPidNtuple.h"

//-----------------------------------------------------------------

Bool_t anaProjectPidNtuple(TString inFileName,TString outFileName, Int_t nEvents = 0,Int_t startEvt = 0)
{
TString context = "simulation" ; 
//TString context = "real" ; 

/*
TString inDir   = "/d/hades06/data/nov02/sim/dst/gen3/c2c/";

char *aFiles[] = 
{
	"c2c.v1.3.f14.real.141_dst.root"
};
*/

//uncomment if you want the output with the tree 
//TString outFile = "./results.root"; 

//-----------------------------------------------------------------

  //gROOT->Reset();
  TStopwatch timer;
  Int_t evN=0;

  if(gHades == NULL)
      new Hades;

  printf("Setting configuration...\n");

//---------------  Set HPidParticleFiller (connected later to taks list)

HReconstructor *pRec = new HPidHistograms(outFileName.Data());

//---------------  Set batch (needed for TCanvas's) ------------------

  gROOT->SetBatch();

// ------------ Set input data file: ------------------ --------------

  HRootSource *source = new HRootSource(kFALSE);
  //source->setDirectory((Text_t*)inDir.Data());
  //for(Int_t i = 0; i < sizeof(aFiles) / sizeof(char *); i++) source->addFile(aFiles[i]);
  source->addFile((Text_t*)inFileName.Data());
  gHades->setDataSource(source);

// ----------Add detectors to the setup: NO NEED TO CHANGE -----------

// no detectors needed on this level

//------------------------ Add PID Tasks -----------------------------

  gHades->getTaskSet((Text_t*)context.Data())->add(pRec);

//------------------------ Initialization ----------------------------

  printf("gHades->init()\n");

  gHades->makeCounter(1000);
  if(!gHades->init())
     printf("Error\n");

//-------------------------- Set output ------------------------------

  //gHades->setOutputFile(outFile.Data(),"RECREATE","Test",2);
  //gHades->makeTree();

//--------------------------------------------------------------------

  printf("Processing events...\n");
  timer.Reset();
  timer.Start();
  if (nEvents<1) {
    evN=gHades->eventLoop();
  } else {
    evN=gHades->eventLoop(nEvents,startEvt);
  }

  gHades->getTaskSet("real")->printTimer();

  delete gHades;
  timer.Stop();

  printf("------------------------------------------------------\n");
  printf("Events processed: %i\n",evN);
  printf("Real time: %f\n",timer.RealTime());
  printf("Cpu time: %f\n",timer.CpuTime());
  if (evN) printf("Performance: %f s/ev\n",timer.CpuTime()/evN);

  return kTRUE;
}

#ifndef __CINT__
int main(int argc, char **argv)
{
  TROOT AnalysisDST("HADES","Analysis of Events");

  switch(argc)
  {
    case 3:
      return anaProjectPidNtuple(TString(argv[1]),TString(argv[2]));
      break;
    case 4:
      return anaProjectPidNtuple(TString(argv[1]),TString(argv[2]),atoi(argv[3]));
      break;
    case 5:
      return anaProjectPidNtuple(TString(argv[1]),TString(argv[2]),atoi(argv[3]),atoi(argv[4]));
      break;
    default:
    break;
   }
  return 0;
}
#endif


