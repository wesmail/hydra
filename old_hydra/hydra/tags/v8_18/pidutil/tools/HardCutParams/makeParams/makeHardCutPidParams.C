// Macro to make PidAlgStandCutsPar paramter container. As the input the Rich, Beta and Shower 
// standard cut PID parameters should be created. Before runing the macro user must set analysis 
// context, input and output files and runId. 
//
// To run this macro the ROOT and HYDRA enviroment must be set. 
// To start this macro use:
// 
// root -l makeHardCutPidParams()
// or
// root> .x makeHardCutPidParams()

int makeHardCutPidParams()
{

// -- start user configuration

  // context  
  TString context = "sim";
  //TString context = "exp";
   
  // input files with Rich/Beta/Shower PID parameters  
  TString inRichParFile   = "/disk2/otwinow/hydra/hydra-dev/params/aug04/hardcuts/richparams_PM180_RC3.0_NP6_AC_6.root";
  TString inShowerParFile =	"/disk2/otwinow/hydra/hydra-dev/params/aug04/hardcuts/showerparams_F10_0.80_F20_0.80_maxF_0.80_sum00.95_sumdiff_0.80_sim_runge_26032007.root";
  TString inBetaParFile	  =	"/disk2/otwinow/hydra/hydra-dev/params/aug04/hardcuts/betaparams_2.0sigma_sim_runge.root";
  
  // output file
  TString outParFile = "test_hard_cuts_aug04.root"; 
  
  //ref nRunId
  Int_t nRunId=7003;
  //Int_t nRunId=621306858;

// -- end user settings
  
  TStopwatch timer;
  Int_t evN=0;

  // set gHades
  if(gHades == NULL) new Hades;
  gHades->setTreeBufferSize(8000);

  //Set batch (needed for TCanvas's)

  gROOT->SetBatch();

  //parameter files
  HRuntimeDb* rtdb=gHades->getRuntimeDb();

  HParRootFileIo *output=new HParRootFileIo;
  output->open((Text_t*)outParFile.Data(),"RECREATE");
  rtdb->setOutput(output);

  // rich parameters
  TFile *f0 = new TFile(inRichParFile.Data());
  f0->cd();
  HPidHistogramsCont *pRichPar = (HPidHistogramsCont*)f0->Get(context.Data());
  f0->Close();

  // beta vs momentum
  TFile *f1 = new TFile(inBetaParFile.Data());
  f1->cd();
  HPidHistogramsCont *pBetaPar = (HPidHistogramsCont*)f1->Get(context.Data());
  f1->Close();
  
  // shower params
  TFile *f2 = new TFile(inShowerParFile.Data());
  f2->cd();
  HPidHistogramsCont *pShowerPar = (HPidHistogramsCont*)f2->Get(context.Data());
  f2->Close();

  //--------------------------------------------------------------------
  Short_t nParticles[]={2,3};
  Short_t nPart = sizeof(nParticles)/sizeof(int);

  //HPidAlgStandCutsPar *par = new HPidAlgStandCutsPar("PidAlgStandCutsPar","Parameters for HPidAlgStandCutsPar","PidAlgStandCutsParContext");
  HPidAlgStandCutsPar *par = rtdb->getContainer("PidAlgStandCutsPar");

  for(Int_t sec = 0; sec < 6; sec++) 
  {
	  for(Int_t part = 0; part < nPart+1; part++) 
	  {
  	      TH1 *rich = pRichPar->getHistogram(nParticles[part],0,sec);
          TH1 *shower1 = pShowerPar->getHistogram(nParticles[part],2,sec);
          TH1 *shower2 = pShowerPar->getHistogram(nParticles[part],3,sec);
          TH1 *shower3 = pShowerPar->getHistogram(nParticles[part],4,sec);
	  
  		  TH1 *betaLsys0 = pBetaPar->getHistogram(nParticles[part],0,sec);
  		  TH1 *betaUsys0 = pBetaPar->getHistogram(nParticles[part],1,sec);
  		  TH1 *betaLsys1 = pBetaPar->getHistogram(nParticles[part],2,sec);
  		  TH1 *betaUsys1 = pBetaPar->getHistogram(nParticles[part],3,sec);

		  par->addHistogram(nParticles[part],0,sec,rich);
		  par->addHistogram(nParticles[part],1,sec,shower1);
		  par->addHistogram(nParticles[part],2,sec,shower2);
		  par->addHistogram(nParticles[part],3,sec,betaLsys0);
		  par->addHistogram(nParticles[part],4,sec,betaUsys0);
		  par->addHistogram(nParticles[part],5,sec,betaLsys1);
		  par->addHistogram(nParticles[part],6,sec,betaUsys1);

		  par->addHistogram(nParticles[part],7,sec,shower3);
	  }
  }
   rtdb->addRun(nRunId);
   par->setChanged();

   rtdb->setContainersStatic();
   rtdb->saveOutput();
   rtdb->print();
   rtdb->closeOutput();

  delete gHades;

  return 0;
} 
