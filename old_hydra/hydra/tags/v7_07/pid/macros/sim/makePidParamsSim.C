int makePidParamsSim()
{
  TFile *f = new TFile("/net/disk14/otwinow/hydra_nov02/params/pid/pidParams_RichTofShowerTofino_Nov02_Sim.root");
  f->cd();
  HPidHistogramsCont *pRichPar = (HPidHistogramsCont*)f->Get("richpar");
  HPidHistogramsCont *pBetaPar = (HPidHistogramsCont*)f->Get("betasim");
  HPidHistogramsCont *pShowerPar = (HPidHistogramsCont*)f->Get("showsim");
  f->Close();

  // input and output data files
  TString outParFile = "/net/disk14/otwinow/hydra_dev/params/pidParams_HardCuts_Nov02_Sim.root"; 
  
  TStopwatch timer;
  Int_t evN=0;

  // set gHades
  if(gHades == NULL) new Hades;
  gHades->setTreeBufferSize(8000);

  //pid tasks
  Int_t nRunId=9;

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
	  // no difference in response between e+ and e-
  	  TH1 *rich = pRichPar->getHistogram(2,0);
      TH1 *shower1 = pShowerPar->getHistogram(2,2);
      TH1 *shower2 = pShowerPar->getHistogram(2,3);

	  for(Int_t part = 0; part < nPart+1; part++) 
	  {
  		  TH1 *betaLsys0 = pBetaPar->getHistogram(nParticles[part],0);
  		  TH1 *betaUsys0 = pBetaPar->getHistogram(nParticles[part],1);
  		  TH1 *betaLsys1 = pBetaPar->getHistogram(nParticles[part],2);
  		  TH1 *betaUsys1 = pBetaPar->getHistogram(nParticles[part],3);

		  par->addHistogram(nParticles[part],0,sec,rich);
		  par->addHistogram(nParticles[part],1,sec,shower1);
		  par->addHistogram(nParticles[part],2,sec,shower2);
		  par->addHistogram(nParticles[part],3,sec,betaLsys0);
		  par->addHistogram(nParticles[part],4,sec,betaUsys0);
		  par->addHistogram(nParticles[part],5,sec,betaLsys1);
		  par->addHistogram(nParticles[part],6,sec,betaUsys1);
	  }
  }
   rtdb->initContainers(nRunId);
   par->setChanged();
   rtdb->setContainersStatic();
   rtdb->print();

   rtdb->saveOutput();
   rtdb->closeOutput();

  delete gHades;

  return 0;
}// END int fill(TString, Int_t , Int_t)


#ifndef __CINT__
int main(int argc, char **argv)
{
  TROOT AnalysisDST("HADES Hyp","Analysis of 4 track  Events");
  
  switch(argc)
  {
    case 2:
      return makePidParams(TString(argv[1]));
      break;
    case 3:
      return makePidParams(TString(argv[1]),atoi(argv[2]));
      break;
    case 4:
      return makePidParams(TString(argv[1]),atoi(argv[2]),atoi(argv[3]));
      break;
    default:
    break;
   }
  return 0;
}
#endif
