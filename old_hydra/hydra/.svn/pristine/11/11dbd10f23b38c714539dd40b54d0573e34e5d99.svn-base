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
#include "fillHypContExp.h"
#include "hhypreconstructor.h"

using namespace std;

Int_t 
fillHypContExp(TString inputFile, Int_t nEvents, Int_t startEvt)
{
  //-----------------------------------------------------------------
  //   Int_t nEvents = 1000;
  //   Int_t startEvt = 0;
  //
  //cout<<"\n------- Operation with filenames -------------------"<<endl;
  TString inDir   = "/d/c2/jacek/jan04/postdst/";
  TString outDir = "/d/c2/hadeshyp/output/"; 

  TString outFileSuffix = "_hyp.root";
  TString inputFileSuffix = ".root";
  TString inputFileFull=inputFile+inputFileSuffix;
  TString outHistFileSuffix = "_ntup_hyp.root";
  TString outFile = outDir+inputFile+outFileSuffix; 
  TString outHistFile = outDir+inputFile+outHistFileSuffix; 
  TFile * hfile = new TFile(outHistFile,"RECREATE","Demo ROOT file with ntupla");
  //cout<<"\n------- END Operation with filenames ----------------"<<endl;
  
  // real or simulation
  Char_t *context="real"; 

  //-----------------------------------------------------------------
  //gSystem->SetIncludePath(" $HYDRA_INCLUDES ");
  //cout<<"gSystem->SetIncludePath( $HYDRA_INCLUDES )"<<endl;
  //-----------------------------------------------------------------

  TStopwatch timer;
  Int_t evN=0;

  // set gHades
  if(gHades == NULL) new Hades;
  gHades->setTreeBufferSize(8000);

  cout<< " Setting configuration..." << endl; 
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

  //---------------  Set batch (needed for TCanvas's) ------------------

  gROOT->SetBatch();

  // ------------ Set input data file: ------------------ --------------

  //HRootSource *source = new HRootSource(kFALSE);
  HRootSource *source = new HRootSource();
  source->setDirectory((Text_t*)inDir.Data());
  source->addFile((Text_t*)inputFileFull.Data());
  gHades->setDataSource(source);

  //------------------------ Add PID Tasks -----------------------------

  gHades->getTaskSet(context)->add(hyp);

  //------------------------ Initialization ----------------------------
  cout<<"gHades->init()\n";

  gHades->makeCounter(1000);
  if(!gHades->init())
    cerr<<"Error\n";

  //-------------------------- Set output ------------------------------
  // Bool_t 
  // setOutputFile(Text_t* name, Option_t* opt, Text_t* title, Int_t comp)
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

  delete gHades;
  timer.Stop();
  hfile->Close();

  cout<<"------------------------------------------------------\n";
  cout<<"Events processed: "<<evN<<endl;
  cout<<"Real time: "<<timer.RealTime()<<endl;;
  cout<<"Cpu time: "<<timer.CpuTime()<<endl;
  if (evN) cout<<"Performance: "<<timer.CpuTime()/evN<<endl;;

  return 0;
}// END Int_t fill(TString, Int_t , Int_t)


#ifndef __CINT__

//main(Int_t i,Int_t nEvents)
Int_t main()
{
  TROOT AnalysisDST("HADES Hyp","Analysis of 4 track  Events");

  TString inFiles[] = 
    {   
      "be04043142704_dstgen1_1_post",
      "be04043100605_dstgen1_1_post",
      "be04043135918_dstgen1_1_post",
      "be04043141244_dstgen1_1_post",    
      //      "be04043141244_dstgen1_post",
      "be04043103321_dstgen1_post",
      "be04043134551_dstgen1_post",
      "be04043142704_dstgen1_post",
      "be04043135918_dstgen1_post",
      "be04043134551_dstgen1_1_post"     
    };

  cout<<"Enter file number : "<<endl;
  for(ii = 0; ii < sizeof(inFiles) / sizeof(TString); ii++) 
    cout<<"\t ["<<ii<<"] "<<inFiles[ii]<<endl;
  //  cin>>i;
  i = 5;
  if ( (sizeof(inFiles) / sizeof(TString)) < i )
       i = 0;
  //cout<<"Enter number of events : "<<endl;

 
  //  for(i = 0; i < sizeof(inFiles) / sizeof(TString); i++) {
  cout<<"***************************************************************************"<<endl;
  cout<<"***************************************************************************"<<endl;
  cout<<"***************************************************************************"<<endl;
  cout<<"**** Processing File "<<i
      <<"  nEvents = "<<nEvents<<"  :: "<<inFiles[i]<<".root\t***"<<endl;
  cout<<"***************************************************************************"<<endl;
  fill( inFiles[i], nEvents,7000);
  cout<<"****\t     End of File  "<<i<<" :: "<<inFiles[i]<<".root\t***"<<endl;
  cout<<"***************************************************************************\n\n"<<endl;
  //  }
  return 0;
}
#endif
