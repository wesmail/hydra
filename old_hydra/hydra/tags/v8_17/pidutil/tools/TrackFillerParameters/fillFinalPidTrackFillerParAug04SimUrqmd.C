fillTrackFillerParams()
{

  Hades myHades;
  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  
  HParRootFileIo *in = new HParRootFileIo;
  in->open("EmptyTrackFillerPar.root");
  rtdb->setFirstInput(in);

  HPidTrackFillerPar *pTrackFillerPar
    = (HPidTrackFillerPar *) rtdb->getContainer("PidTrackFillerPar");
  
  HParRootFileIo *out = new HParRootFileIo;
  
  out->open("FinalPidTrkFillerParamsAug04SimUrqmdGen2.root", "RECREATE");
  rtdb->setOutput(out);
  
  rtdb->initContainers(7003);


  //--------------------
  //Assemble the histograms
  TString Lower("Lower");
  TString Upper("Upper");
  TString ThetaBoundaries("ThetaBoundaries");
  TString PhiBoundaries("PhiBoundaries");
  TString Sector("Sector");
  
  // Aug04 Sim Gen2
  
  Float_t ThetaSigma[6][9]={{1.13,0.98,0.77,0.70,0.62,0.55,0.55,0.50,0.50},
			   {1.25,0.89,0.75,0.67,0.62,0.60,0.59,0.59,0.59},
			   {1.16,0.88,0.76,0.65,0.60,0.60,0.56,0.56,0.56},
			   {1.18,0.90,0.73,0.67,0.59,0.56,0.53,0.53,0.53},
			   {1.18,0.93,0.79,0.67,0.64,0.61,0.55,0.55,0.55},
			   {1.06,0.90,0.74,0.69,0.60,0.58,0.53,0.53,0.53}};
  
  Float_t ThetaMean[6][9]={{0.03,-0.04,-0.02,-0.01,-0.01,-0.00,-0.00,-0.04,-0.04},
			  {0.06,-0.03,-0.03,-0.02,-0.01,-0.02,0.01,0.01,0.01},
			  {0.19,-0.03,-0.02,-0.03,-0.02,-0.02,0.03,0.03,0.03},
			  {0.01,-0.04,-0.03,-0.02,-0.03,-0.01,-0.00,-0.00,-0.00},
			  {0.02,-0.05,-0.03,-0.01,-0.01,-0.02,-0.00,-0.00,-0.00},
			  {0.08,0.01,-0.01,-0.01,-0.02,-0.00,-0.02,-0.02,-0.02}};
  
  Float_t PhiSigma[6][9]={{1.03,0.82,0.65,0.58,0.52,0.47,0.40,0.40,0.40},
			 {0.97,0.79,0.66,0.56,0.52,0.45,0.44,0.41,0.41},
			 {1.04,0.81,0.63,0.55,0.52,0.47,0.43,0.43,0.43},
			 {0.99,0.79,0.64,0.56,0.50,0.47,0.46,0.40,0.40},
			 {0.94,0.77,0.65,0.56,0.51,0.48,0.46,0.40,0.40},
			 {1.01,0.78,0.62,0.55,0.51,0.49,0.43,0.43,0.43}};
  
  Float_t PhiMean[6][9]={{-0.02,-0.01,-0.01,-0.01,0.01,0.00,-0.01,-0.01,-0.01},
			{-0.02,0.01,-0.01,-0.01,-0.00,-0.01,-0.00,0.00,0.00},
			{0.01,0.00,-0.01,0.01,-0.00,0.00,0.02,0.02,0.02},
			{-0.00,0.01,0.01,-0.00,0.01,0.01,0.01,0.01,0.01},
			{-0.00,0.00,0.01,0.00,-0.00,0.02,0.01,0.02,0.02},
			{0.01,-0.01,-0.00,0.00,-0.00,0.02,0.00,0.00,0.00}};

  Float_t LowerXBoundariesRkMeta[2][6]   =
    {{-50.0,-50.0,-50.0,-50.0,-50.0,-50.0},{-78.0,-78.0,-81.0,-76.0,-76.0,-78.0}};
  
  Float_t UpperXBoundariesRkMeta[2][6]   =
    {{ 50.0, 50.0, 50.0, 50.0, 50.0, 50.0},{ 78.0, 78.0, 81.0, 76.0, 76.0, 78.0}};
  
  Float_t LowerYBoundariesRkMeta[2][6]   =
    {{-30.0,-50.0,-40.0,-30.0,-30.0,-40.0},{-12.0,-11.0,-14.0,-11.0,-11.0,-12.0}};
  
  Float_t UpperYBoundariesRkMeta[2][6]   =
    {{ 30.0, 30.0, 40.0, 30.0, 30.0, 40.0},{ 12.0, 11.0, 14.0, 11.0, 11.0, 12.0}};
  
  
  Float_t LowerXBoundariesMdcMeta[2][6]  =
    {{-50.0,-50.0,-70.0,-50.0,-50.0,-70.0},{-78.0,-78.0,-100.0,-76.0,-76.0,-97.0}};
  
  Float_t UpperXBoundariesMdcMeta[2][6]  =
    {{ 50.0, 50.0, 70.0, 50.0, 50.0, 70.0},{ 78.0, 78.0, 100.0, 76.0, 76.0, 97.0}};
  
  Float_t LowerYBoundariesMdcMeta[2][6]  =
    {{-40.0,-40.0,-50.0,-40.0,-40.0,-50.0},{-29.0,-27.0,-42.0,-27.0,-27.0,-40.0}};
  
  Float_t UpperYBoundariesMdcMeta[2][6]  =
    {{ 40.0, 40.0, 50.0, 40.0, 40.0, 50.0},{ 29.0, 27.0, 42.0, 27.0, 27.0, 40.0}};



  TString* histogramName=NULL;
  
  TH1F* HistArray[24];
  Int_t histCounter=0;
  Int_t sector=0;
  Int_t j=0;
  Int_t offset=0;
  Int_t pid=0; //All Species use the same histogram!

  const Int_t nTargetBins=3;
  Float_t targetBins[nTargetBins+1]={-60.5, -45.5, -30.5, -15.5};

  const Int_t nCoefficients=8;
  Float_t coefficientBins[nCoefficients+1]={0,1,2,3,4,5,6,7,8};

  pC = new TH2F("polynomialCoefficients","polynomialCoefficients",nTargetBins,targetBins,nCoefficients,coefficientBins);

  //This is for Aug04 - edit it for other beamtimes
  
  FILE *fp=fopen("aug04-coefficients.txt","r");

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
  const Int_t nMomentumBins=9;
  Float_t MomentumBins[nMomentumBins+1]={0.0,100.0,150.0,200.0,250.0,300.0,400.0,500.0, 1000.0, 3000.0};
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

  for(Int_t sector = 0;sector<6;sector++)
    {
      pTrackFillerPar->setDeltaMdcMetaXMin(sector,0,LowerXBoundariesMdcMeta[0][sector]);
      pTrackFillerPar->setDeltaMdcMetaYMin(sector,0,LowerYBoundariesMdcMeta[0][sector]);
      pTrackFillerPar->setDeltaMdcMetaXMax(sector,0,UpperXBoundariesMdcMeta[0][sector]);
      pTrackFillerPar->setDeltaMdcMetaYMax(sector,0,UpperYBoundariesMdcMeta[0][sector]);

      pTrackFillerPar->setDeltaMdcMetaXMin(sector,1,LowerXBoundariesMdcMeta[1][sector]);
      pTrackFillerPar->setDeltaMdcMetaYMin(sector,1,LowerYBoundariesMdcMeta[1][sector]);
      pTrackFillerPar->setDeltaMdcMetaXMax(sector,1,UpperXBoundariesMdcMeta[1][sector]);
      pTrackFillerPar->setDeltaMdcMetaYMax(sector,1,UpperYBoundariesMdcMeta[1][sector]);

      pTrackFillerPar->setDeltaRkMetaXMin(sector,0,LowerXBoundariesRkMeta[0][sector]);
      pTrackFillerPar->setDeltaRkMetaYMin(sector,0,LowerYBoundariesRkMeta[0][sector]);
      pTrackFillerPar->setDeltaRkMetaXMax(sector,0,UpperXBoundariesRkMeta[0][sector]);
      pTrackFillerPar->setDeltaRkMetaYMax(sector,0,UpperYBoundariesRkMeta[0][sector]);

      pTrackFillerPar->setDeltaRkMetaXMin(sector,1,LowerXBoundariesRkMeta[1][sector]);
      pTrackFillerPar->setDeltaRkMetaYMin(sector,1,LowerYBoundariesRkMeta[1][sector]);
      pTrackFillerPar->setDeltaRkMetaXMax(sector,1,UpperXBoundariesRkMeta[1][sector]);
      pTrackFillerPar->setDeltaRkMetaYMax(sector,1,UpperYBoundariesRkMeta[1][sector]);


    }

  pTrackFillerPar->setChanged();
  rtdb->saveOutput();
}
