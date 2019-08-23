int makeHardCutPidParams()
{
  // context  
  TString context = "sim";
  //TString context = "exp";
  
  // input files
  // rich parameters
  TFile *f0 = new TFile("/misc/pachmay/aug04sim/pluto/cuts/params/rich/richparams_PM180_RC3.0_NP6_AC_6.root");
  //TFile *f0 = new TFile("/misc/pachmay/aug04real/cuts/params/rich/richparams_PM180_RC3.0_NP6_AC_6.root");
  f0->cd();
  HPidHistogramsCont *pRichPar = (HPidHistogramsCont*)f0->Get("richpar");
  f0->Close();

  // beta vs momentum
  //TFile *f1 = new TFile("/misc/pachmay/aug04sim/pluto/cuts/params/beta/betaparams_2.0sigma_sim_kick.root");
  TFile *f1 = new TFile("/misc/pachmay/aug04sim/pluto/cuts/params/beta/betaparams_2.0sigma_sim_runge.root");
  //TFile *f1 = new TFile("/misc/pachmay/aug04real/cuts/params/beta/betaparams_2.0sigma_exp_kick.root");
  //TFile *f1 = new TFile("/misc/pachmay/aug04real/cuts/params/beta/betaparams_2.0sigma_exp_runge.root");
  f1->cd();
  HPidHistogramsCont *pBetaPar = (HPidHistogramsCont*)f1->Get(context.Data());
  f1->Close();
  
  // shower params
  TFile *f2 = new TFile("/d/hades/jacek/hydra-dev/params/hardcuts/showerparams_F10_0.80_F20_0.80_maxF_0.80_sum00.95_sumdiff_0.80_sim.root");
  //TFile *f2 = new TFile("/d/hades/jacek/hydra-dev/params/hardcuts/showerparams_F10_0.80_F20_0.80_maxF_0.80_sum00.95_sumdiff_0.80_exp.root");
  f2->cd();
  HPidHistogramsCont *pShowerPar = (HPidHistogramsCont*)f2->Get(context.Data());
  f2->Close();

  // output file
  //TString outParFile = "/d/hades/jacek/hydra-dev/params/hardcuts/pidParams_HardCuts_aug04_sim_kick.root"; 
  TString outParFile = "/d/hades/jacek/hydra-dev/params/hardcuts/pidParams_HardCuts_aug04_sim_runge.root"; 
  //TString outParFile = "/d/hades/jacek/hydra-dev/params/hardcuts/pidParams_HardCuts_aug04_exp_kick.root"; 
  //TString outParFile = "/d/hades/jacek/hydra-dev/params/hardcuts/pidParams_HardCuts_aug04_exp_runge.root"; 
  
  TStopwatch timer;
  Int_t evN=0;

  // set gHades
  if(gHades == NULL) new Hades;
  gHades->setTreeBufferSize(8000);

  //ref nRunId
  Int_t nRunId=7003;
  //Int_t nRunId=621306858;

  //Set batch (needed for TCanvas's)

  gROOT->SetBatch();

  //parameter files
  HRuntimeDb* rtdb=gHades->getRuntimeDb();

  HParRootFileIo *output=new HParRootFileIo;
  output->open((Text_t*)outParFile.Data(),"RECREATE");
  rtdb->setOutput(output);

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
   rtdb->initContainers(nRunId);
   par->setChanged();
   rtdb->setContainersStatic();

   rtdb->saveOutput();
   rtdb->closeOutput();
   rtdb->print();

  delete gHades;

  return 0;
} 
