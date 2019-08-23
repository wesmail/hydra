fillTrackFillerParams()
{

  Hades myHades;
  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  
  HParRootFileIo *in = new HParRootFileIo;
  in->open("../input/EmptyTrackFillerPar.root");
  rtdb->setFirstInput(in);

  HPidTrackFillerPar *pTrackFillerPar
    = (HPidTrackFillerPar *) rtdb->getContainer("PidTrackFillerPar");
  
  HParRootFileIo *out = new HParRootFileIo;
  
  out->open("./youroutputpathandfile", "RECREATE");
  rtdb->setOutput(out);
  
  rtdb->initContainers(501);


  //--------------------
  //Assemble the histograms
  TString Lower("Lower");
  TString Upper("Upper");
  TString ThetaBoundaries("ThetaBoundaries");
  TString PhiBoundaries("PhiBoundaries");
  TString Sector("Sector");
  
  // Nov02 Exp Gen3
  
  Float_t ThetaSigma[][7]={{1.38,1.08,0.84,0.72,0.65,0.61,0.60},
			   {1.22,0.96,0.77,0.72,0.69,0.65,0.65},
			   {1.25,1.03,0.80,0.69,0.67,0.61,0.58},
			   {1.31,1.04,0.80,0.70,0.62,0.59,0.55},
			   {1.15,0.91,0.73,0.67,0.62,0.61,0.61},
			   {1.26,1.03,0.79,0.67,0.62,0.57,0.57}};
  
  Float_t ThetaMean[][7]={{-0.33,-0.28,-0.18,-0.10,-0.05,0.01,0.00},
			  {0.27,0.28,0.28,0.26,0.21,0.20,0.16},
			  {0.27,0.33,0.40,0.43,0.43,0.47,0.43},
			  {0.25,0.33,0.42,0.49,0.53,0.57,0.55},
			  {-0.34,-0.34,-0.32,-0.35,-0.35,-0.38,-0.38},
			  {-0.05,-0.00,0.09,0.16,0.21,0.23,0.26}};
  
  Float_t PhiSigma[][7]={{1.11,0.90,0.74,0.67,0.64,0.64,0.64},
			 {1.01,0.80,0.67,0.64,0.60,0.60,0.60},
			 {1.12,0.92,0.74,0.69,0.67,0.67,0.67},
			 {1.11,0.88,0.70,0.63,0.60,0.60,0.60},
			 {1.24,0.88,0.69,0.66,0.64,0.64,0.64},
			 {1.25,1.21,0.93,0.70,0.70,0.66,0.66}};
  
  
  Float_t PhiMean[][7]={{0.40,0.43,0.46,0.47,0.49,0.48,0.45},
			{0.32,0.34,0.35,0.35,0.36,0.32,0.32},
			{0.04,0.04,0.06,0.06,0.07,0.06,0.07},
			{-0.29,-0.29,-0.29,-0.29,-0.31,-0.31,-0.32},
			{-0.13,-0.11,-0.10,-0.10,-0.12,-0.09,-0.07},
			{0.27,0.27,0.29,0.33,0.31,0.30,0.29}};

  TString* histogramName=NULL;
  
  TH1F* HistArray[24];
  Int_t histCounter=0;
  Int_t sector=0;
  Int_t j=0;
  Int_t offset=0;
  Int_t pid=0; //All Species use the same histogram!
  const Int_t nTargetBins=2;
  Float_t targetBins[nTargetBins+1]={-127.0, -27.0, 73.0,};

  const Int_t nCoefficients=8;
  Float_t coefficientBins[nCoefficients+1]={0,1,2,3,4,5,6,7,8};

  pC = new TH2F("polynomialCoefficients","polynomialCoefficients",nTargetBins,targetBins,nCoefficients,coefficientBins);

  //This is for Aug04 - edit it for other beamtimes
  
  FILE *fp=fopen("../input/Nov02/nov02-coefficients.txt","r");

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
  const Int_t nMomentumBins=7;
  Float_t MomentumBins[nMomentumBins+1]={0.0,100.0,200.0,300.0,400.0,500.0,600.0,2000.0};
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
