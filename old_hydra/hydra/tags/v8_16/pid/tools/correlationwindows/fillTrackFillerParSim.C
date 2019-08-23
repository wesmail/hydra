fillTrackFillerParSim()
{

  Hades myHades;
  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  
  HParRootFileIo *in = new HParRootFileIo;
  in->open("../output/Aug04/EmptyTrackFillerPar.root");
  rtdb->setFirstInput(in);

  HPidTrackFillerPar *pTrackFillerPar
    = (HPidTrackFillerPar *) rtdb->getContainer("PidTrackFillerPar");
  
  HParRootFileIo *out = new HParRootFileIo;
  
  out->open("../output/Aug04/PidTrkFillerPar_Aug04SimGen2.root", "RECREATE");
  rtdb->setOutput(out);
  
  rtdb->initContainers(1);


  //--------------------
  //Assemble the histograms
  TString Lower("Lower");
  TString Upper("Upper");
  TString ThetaBoundaries("ThetaBoundaries");
  TString PhiBoundaries("PhiBoundaries");
  TString Sector("Sector");
  
  // Aug04 Sim Gen1
  
  Float_t ThetaSigma[][12]={{1.11,0.95,0.76,0.72,0.62,0.60,0.52,0.48,0.48,0.48,0.48,0.48},
			    {1.17,0.93,0.79,0.67,0.60,0.55,0.54,0.51,0.51,0.51,0.51,0.51},
			    {1.16,0.96,0.77,0.68,0.63,0.55,0.51,0.51,0.51,0.51,0.51,0.51},
			    {1.14,0.95,0.77,0.66,0.60,0.52,0.52,0.49,0.49,0.49,0.49,0.49},
			    {1.17,0.91,0.78,0.67,0.59,0.54,0.54,0.54,0.54,0.54,0.54,0.54},
			    {1.21,0.95,0.77,0.66,0.59,0.56,0.47,0.47,0.47,0.47,0.47,0.47}};

  Float_t ThetaMean[][12]={{0.04,-0.04,-0.02,-0.01,-0.01,0.02,0.00,0.04,0.04,0.04,0.04,0.04},
			   {-0.00,-0.09,-0.03,-0.02,0.02,-0.02,-0.04,-0.06,-0.06,-0.06,-0.06,-0.06},
			   {0.02,-0.09,-0.04,-0.02,0.01,-0.00,0.04,0.04,0.04,0.04,0.04,0.04},
			   {0.03,-0.04,-0.01,-0.00,-0.03,-0.02,-0.02,0.03,0.03,0.03,0.03,0.03},
			   {0.02,-0.06,-0.03,-0.02,-0.00,0.02,0.02,0.02,0.02,0.02,0.02,0.02},
			   {0.10,-0.06,-0.05,0.02,-0.01,-0.07,-0.01,-0.01,-0.01,-0.01,-0.01,-0.01}};
  
  Float_t PhiSigma[][12]={{1.03,0.85,0.70,0.61,0.61,0.47,0.47,0.47,0.47,0.47,0.47,0.47},
			  {1.04,0.84,0.72,0.63,0.55,0.51,0.51,0.49,0.49,0.49,0.49,0.49},
			  {1.09,0.83,0.70,0.60,0.57,0.55,0.48,0.44,0.44,0.44,0.44,0.44},
			  {0.97,0.82,0.68,0.58,0.55,0.49,0.44,0.43,0.43,0.43,0.43,0.43},
			  {1.06,0.81,0.68,0.65,0.55,0.55,0.49,0.49,0.49,0.49,0.49,0.49},
			  {1.06,0.84,0.69,0.60,0.49,0.49,0.47,0.47,0.47,0.47,0.47,0.47}};
  
  Float_t PhiMean[][12]={{0.03,0.02,0.02,-0.01,-0.03,-0.04,-0.04,-0.04,-0.04,-0.04,-0.04,-0.04},
			 {-0.01,0.00,0.00,0.01,0.02,0.00,-0.01,-0.00,-0.08,-0.08,-0.19,-0.84},
			 {0.02,-0.01,0.01,0.01,0.01,0.07,0.01,-0.03,-0.03,-0.03,-0.03,-0.03},
			 {0.00,0.01,-0.00,0.03,-0.05,-0.02,0.01,-0.08,-0.08,-0.08,-0.08,-0.08},
			 {-0.02,-0.02,0.00,-0.00,-0.00,-0.02,-0.01,-0.01,-0.01,-0.01,-0.01,-0.01},
			 {-0.01,0.01,-0.01,-0.01,0.01,0.01,0.07,0.07,0.07,0.07,0.07,0.07}};

  TString* histogramName=NULL;
  
  TH1F* HistArray[24];
  Int_t histCounter=0;
  Int_t sector=0;
  Int_t j=0;
  Int_t offset=0;
  Int_t pid=0; //All Species use the same histogram!

  const Int_t nTargetBins=3;
  Float_t targetBins[nTargetBins+1]={-62.5, -47.5, -32.5, -17.5};

  const Int_t nCoefficients=8;
  Float_t coefficientBins[nCoefficients+1]={0,1,2,3,4,5,6,7,8};

  pC = new TH2F("polynomialCoefficients","polynomialCoefficients",nTargetBins,targetBins,nCoefficients,coefficientBins);

  //This is for Aug04 - edit it for other beamtimes
  
  FILE *fp=fopen("../input/Aug04/aug04-coefficients.txt","r");

  Float_t value;

  for(Int_t targetSection=0;targetSection<nTargetBins;targetSection++)
    {
      for(Int_t order=0;order<nCoefficients;order++)
	{
	  fscanf(fp,"%f",&value);
	  pC->SetBinContent(pC->GetBin(targetSection+1,order+1),value); 
	  cout <<"Value: " << value << endl;
	}
    }
  Float_t nSigma=2.0;
  const Int_t nMomentumBins=12;
  Float_t MomentumBins[nMomentumBins+1]={0.0,100.0,150.0,200.0,250.0,300.0,350.0,400.0,450.0,500.0,550.0,600.0,2000.0};
  const Float_t* ptr = MomentumBins;
  //Lower Phi Boundaries
  for(sector=0;sector<6;sector++)
    {
      histogramName = new TString(Lower);
      histogramName->Append(PhiBoundaries);
      histogramName->Append(Sector);
      (*histogramName)+=sector;
      cout << histogramName->Data() << endl;
      HistArray[histCounter] = new TH1F(histogramName->Data(),histogramName->Data(),nMomentumBins,ptr);
      TH1F* tmp = HistArray[histCounter];
      for(j=1;j<=tmp->GetNbinsX();j++)
	{
	  tmp->SetBinContent(tmp->GetBin(j),PhiMean[sector][j-1]-nSigma*TMath::Abs(PhiSigma[sector][j-1]));
	}
      offset=0; //first histogram!
      pid=0; //
      pTrackFillerPar->addHistogram(pid,offset,sector,HistArray[histCounter]);
      histCounter++;
      delete histogramName;
    }

  //Upper Phi Boundaries
  for(sector=0;sector<6;sector++)
    {
      histogramName = new TString(Upper);
      histogramName->Append(PhiBoundaries);
      histogramName->Append(Sector);
      (*histogramName)+=sector;
      cout << histogramName->Data() << endl;
      HistArray[histCounter] = new TH1F(histogramName->Data(),histogramName->Data(),nMomentumBins,ptr);
      TH1F* tmp = HistArray[histCounter];
      for(j=1;j<=tmp->GetNbinsX();j++)
	{
	  tmp->SetBinContent(tmp->GetBin(j),PhiMean[sector][j-1]+nSigma*TMath::Abs(PhiSigma[sector][j-1]));
	}
      offset=1; //first histogram!
      pid=0;
      pTrackFillerPar->addHistogram(pid,offset,sector,HistArray[histCounter]);
      histCounter++;
      delete histogramName;
    }

  //Lower Theta Boundaries
  for(sector=0;sector<6;sector++)
    {
      histogramName = new TString(Lower);
      histogramName->Append(ThetaBoundaries);
      histogramName->Append(Sector);
      (*histogramName)+=sector;
      cout << histogramName->Data() << endl;
      HistArray[histCounter] = new TH1F(histogramName->Data(),histogramName->Data(),nMomentumBins,ptr);
      TH1F* tmp = HistArray[histCounter];
      for(j=1;j<=tmp->GetNbinsX();j++)
	{
	  tmp->SetBinContent(tmp->GetBin(j),ThetaMean[sector][j-1]-nSigma*TMath::Abs(ThetaSigma[sector][j-1]));
	}
      //tmp->Draw();
      offset=2;
      pid=0;
      pTrackFillerPar->addHistogram(pid,offset,sector,HistArray[histCounter]);
      histCounter++;
      delete histogramName;
    }

  //Upper Theta Boundaries
  for(sector=0;sector<6;sector++)
    {
      histogramName = new TString(Upper);
      histogramName->Append(ThetaBoundaries);
      histogramName->Append(Sector);

      (*histogramName)+=sector;
      cout << histogramName->Data() << endl;
      HistArray[histCounter] = new TH1F(histogramName->Data(),histogramName->Data(),nMomentumBins,ptr);
      TH1F* tmp = HistArray[histCounter];
      //tmp->Draw();
      for(j=1;j<=tmp->GetNbinsX();j++)
	{
	  tmp->SetBinContent(tmp->GetBin(j),ThetaMean[sector][j-1]+nSigma*TMath::Abs(ThetaSigma[sector][j-1]));
	}
      offset=3;
      pid=0;
      pTrackFillerPar->addHistogram(pid,offset,sector,HistArray[histCounter]);
      histCounter++;
      delete histogramName;

      offset=4;
      pTrackFillerPar->addHistogram(pid,offset,sector,polynomialCoefficients);
    }



  pTrackFillerPar->setChanged();
  rtdb->saveOutput();
}
