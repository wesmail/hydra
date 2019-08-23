makePreliminaryPidTrkFillerPar()
{

  Hades myHades;
  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  
  HParRootFileIo *in = new HParRootFileIo;
  in->open("../output/Aug04/EmptyTrackFillerPar.root");
  rtdb->setFirstInput(in);

  HPidTrackFillerPar *pTrackFillerPar
    = (HPidTrackFillerPar *) rtdb->getContainer("PidTrackFillerPar");
  
  HParRootFileIo *out = new HParRootFileIo;

  out->open("../output/Aug04/PreliminaryPidTrkFillerParAug04.root", "RECREATE");
  rtdb->setOutput(out);
  
  rtdb->initContainers(1);//Dummy
  //rtdb->initContainers(1274446693);  //Aug04Exp


  //--------------------
  //Assemble the histograms
  TString Lower("Lower");
  TString Upper("Upper");
  TString ThetaBoundaries("ThetaBoundaries");
  TString PhiBoundaries("PhiBoundaries");
  TString Sector("Sector");
  
  Float_t LowerThetaBoundaries[12]={-7.0,-7.0,-7.0,-7.0,-7.0,-7.0,-7.0,-7.0,-7.0,-7.0,-7.0,-7.0};
  Float_t UpperThetaBoundaries[12]={ 7.0, 7.0, 7.0, 7.0, 7.0, 7.0, 7.0, 7.0, 7.0, 7.0, 7.0, 7.0};
  Float_t LowerPhiBoundaries[12]=  {-7.0,-7.0,-7.0,-7.0,-7.0,-7.0,-7.0,-7.0,-7.0,-7.0,-7.0,-7.0};
  Float_t UpperPhiBoundaries[12]=  { 7.0, 7.0, 7.0, 7.0, 7.0, 7.0, 7.0, 7.0, 7.0, 7.0, 7.0, 7.0};

  
  TString* histogramName=NULL;
  
  TH1F* HistArray[24];
  Int_t histCounter=0;
  Int_t sector=0;
  Int_t j=0;
  Int_t offset=0;
  Int_t pid=0; //All Species use the same histogram!
  
  const Int_t nTargetBins=3; //There are three targets in this experiment
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

  const Int_t nMomentumBins=12;
  Float_t MomentumBins[nMomentumBins+1]={0.0,100.0,150.0,200.0,250.0,300.0,350.0,400.0,450.0,500.0,550.0,600.0,3000.0};
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
	  tmp->SetBinContent(tmp->GetBin(j),LowerPhiBoundaries[sector]);
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
	  tmp->SetBinContent(tmp->GetBin(j),UpperPhiBoundaries[sector]);
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
	  tmp->SetBinContent(tmp->GetBin(j),LowerThetaBoundaries[sector]);
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
	  tmp->SetBinContent(tmp->GetBin(j),UpperThetaBoundaries[sector]);
	}
      offset=3;
      pid=0;
      pTrackFillerPar->addHistogram(pid,offset,sector,HistArray[histCounter]);
      histCounter++;
      delete histogramName;

      offset=4;
      pTrackFillerPar->addHistogram(pid,offset,sector,pC);
    }



  pTrackFillerPar->setChanged();
  rtdb->saveOutput();
}
