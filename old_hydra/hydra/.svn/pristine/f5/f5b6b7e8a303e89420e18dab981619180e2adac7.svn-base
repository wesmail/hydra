fillTrackFillerParExp()
{

  Hades myHades;
  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  
  HParRootFileIo *in = new HParRootFileIo;
  in->open("../output/Aug04/EmptyTrackFillerPar.root");
  rtdb->setFirstInput(in);

  HPidTrackFillerPar *pTrackFillerPar
    = (HPidTrackFillerPar *) rtdb->getContainer("PidTrackFillerPar");
  
  HParRootFileIo *out = new HParRootFileIo;
  
  out->open("../output/Aug04/PidTrkFillerPar_Aug04ExpGen2.root", "RECREATE");
  rtdb->setOutput(out);
  
  rtdb->initContainers(1);


  //--------------------
  //Assemble the histograms
  TString Lower("Lower");
  TString Upper("Upper");
  TString ThetaBoundaries("ThetaBoundaries");
  TString PhiBoundaries("PhiBoundaries");
  TString Sector("Sector");
  
  // Aug04 Exp Gen0
  
  Float_t ThetaSigma[][12]={{0.98,0.86,0.77,0.70,0.64,0.60,0.60,0.60,0.60,0.60,0.60,0.60},
			    {1.13,0.93,0.78,0.72,0.61,0.59,0.53,0.53,0.53,0.53,0.53,0.53},
			    {1.18,1.04,0.93,0.79,0.79,0.70,0.70,0.70,0.70,0.70,0.70,0.70},
			    {1.12,0.96,0.81,0.74,0.63,0.63,0.63,0.55,0.55,0.55,0.55,0.55},
			    {0.99,0.88,0.72,0.72,0.72,0.72,0.72,0.72,0.72,0.72,0.72,0.72},
			    {1.20,1.09,0.97,0.90,0.84,0.76,0.73,0.64,0.64,0.64,0.64,0.64}};
  
  Float_t ThetaMean[][12]={{-1.12,-1.16,-1.12,-1.09,-1.09,-1.03,-1.03,-1.03,-1.03,-1.03,-1.03,-1.03},
			   {-0.38,-0.35,-0.34,-0.28,-0.29,-0.28,-0.28,-0.28,-0.28,-0.28,-0.28,-0.28},
			   {0.19,0.06,0.13,0.14,0.21,0.28,0.28,0.28,0.28,0.28,0.28,0.28},
			   {0.29,0.29,0.32,0.33,0.36,0.34,0.34,0.37,0.37,0.37,0.37,0.37},
			   {-0.71,-0.72,-0.66,-0.66,-0.66,-0.66,-0.66,-0.66,-0.66,-0.66,-0.66,-0.66}
			   {-0.50,-0.61,-0.61,-0.52,-0.51,-0.43,-0.39,-0.37,-0.37,-0.37,-0.37,-0.37}};
  
  Float_t PhiSigma[][12]={{0.99,0.85,0.75,0.70,0.69,0.64,0.63,0.63,0.61,0.61,0.61,0.61},
			  {0.99,0.82,0.70,0.67,0.58,0.57,0.55,0.52,0.52,0.52,0.52,0.52},
			  {1.10,0.96,0.82,0.75,0.70,0.68,0.63,0.63,0.63,0.63,0.63,0.63},
			  {1.00,0.81,0.69,0.61,0.56,0.51,0.51,0.48,0.48,0.48,0.48,0.48},
			  {0.98,0.86,0.75,0.70,0.60,0.60,0.60,0.60,0.60,0.60,0.60,0.60},
			  {1.11,0.95,0.80,0.74,0.65,0.64,0.60,0.54,0.52,0.52,0.52,0.52}};
  
  Float_t PhiMean[][12]={{0.58,0.63,0.65,0.66,0.67,0.65,0.69,0.69,0.69,0.69,0.69,0.69},
			 {0.56,0.58,0.59,0.59,0.58,0.60,0.56,0.56,0.56,0.56,0.56,0.56},
			 {0.33,0.36,0.36,0.38,0.42,0.42,0.36,0.36,0.36,0.36,0.36,0.36},
			 {-0.34,-0.34,-0.36,-0.33,-0.34,-0.32,-0.32,-0.36,-0.36,-0.36,-0.36,-0.36},
			 {-0.28,-0.21,-0.21,-0.23,-0.23,-0.23,-0.23,-0.23,-0.23,-0.23,-0.23,-0.23},
			 {0.22,0.27,0.26,0.30,0.32,0.31,0.29,0.24,0.25,0.25,0.25,0.25}};

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
