computeWindowsPerMomentum(Char_t* trackingmethod, Char_t* angle, Int_t sector, Char_t* filelist, Char_t* beamtime, Bool_t useBackground=kTRUE, Float_t threshold=3.0)
{

  TString FileNameMean("RICH-MDC-CorrelationWindows-mean-");
  TString FileNameSigma("RICH-MDC-CorrelationWindows-sigma-");
  TString FileNameUpper("RICH-MDC-CorrelationWindows-upper-");
  TString FileNameLower("RICH-MDC-CorrelationWindows-lower-");
  FileNameMean.Append(beamtime);
  FileNameSigma.Append(beamtime);
  FileNameLower.Append(beamtime);
  FileNameUpper.Append(beamtime);

  if(useBackground)
    {
      FileNameMean.Append("WithBG.");
      FileNameSigma.Append("WithBG.");
      FileNameUpper.Append("WithBG.");
      FileNameLower.Append("WithBG.");
    }
  else
    {
      FileNameMean.Append("NoBG.");
      FileNameSigma.Append("NoBG.");
      FileNameUpper.Append("NoBG.");
      FileNameLower.Append("NoBG.");
    }

  FileNameMean.Append(angle);  
  FileNameSigma.Append(angle);
  FILE* mFile = fopen(FileNameMean.Data(),"a");
  FILE* sFile = fopen(FileNameSigma.Data(),"a");


  FileNameUpper.Append(angle);  
  FileNameLower.Append(angle);
  FILE* uFile = fopen(FileNameUpper.Data(),"a");
  FILE* lFile = fopen(FileNameLower.Data(),"a");

  TH1F* histogramarray[7];
  Float_t means[7];
  Float_t sigmas[7];
  //for(Int_t sector=5; sector<6;sector++)
  //{
  TString GifName("MomentumSpecificWindows_");
  GifName.Append(angle);
  GifName.Append("_sector_");
  GifName+=sector;
  GifName.Append(".gif");
  //fprintf(mFile,"%s%d%s%s%s","sector:",sector,"-",angle,"\n");
  for(Int_t mom=0;mom<7;mom++)
    {
      cout << mom << endl;
      if(useBackground)
	{
	  histogramarray[mom] = fitCorrelationExp(mom, trackingmethod, angle, means[mom],sigmas[mom], sector, filelist, threshold);
	}
      else
	{
	  histogramarray[mom] = fitCorrelationSim(mom, trackingmethod, angle, means[mom],sigmas[mom], sector, filelist, threshold);
	}
      gStyle->SetPaperSize(18.,27.);
      TCanvas* pWindows = new TCanvas("MomentumSpecificWindows","Momentum-specific windows");
      pWindows->SetWindowSize(1000,700);
      pWindows->Divide(2,4);
      pWindows->Draw();
    } 
  for(Int_t mom=0;mom<7;mom++)
    {
      pWindows->cd(mom+1);
      histogramarray[mom]->Draw();
    }
  
  
  printf("Mean:{%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f},\n",means[0],means[1],means[2],means[3],means[4],means[5],means[6]);
  printf("Sigma:{%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f},\n",sigmas[0],sigmas[1],sigmas[2],sigmas[3],sigmas[4],sigmas[5],sigmas[6]);
  fprintf(mFile,"{%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f},\n",means[0],means[1],means[2],means[3],means[4],means[5],means[6]);
  fprintf(sFile,"{%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f},\n",sigmas[0],sigmas[1],sigmas[2],sigmas[3],sigmas[4],sigmas[5],sigmas[6]);

  fprintf(uFile,"{%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f}\n",
	  means[0]+2.0*TMath::Abs(sigmas[0]),
	  //means[0]+3.0, //hard threshold due to the fact that signal and background cannot be separated sufficiently
	  means[1]+2.0*TMath::Abs(sigmas[1]),
	  means[2]+2.0*TMath::Abs(sigmas[2]),
	  means[3]+2.0*TMath::Abs(sigmas[3]),
	  means[4]+2.0*TMath::Abs(sigmas[4]),
	  means[5]+2.0*TMath::Abs(sigmas[5]),
	  means[6]+2.0*TMath::Abs(sigmas[6]));
  fprintf(lFile,"{%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f}\n",
	  means[0]-2.0*TMath::Abs(sigmas[0]),
	  //means[0]-3.0, //hard threshold due to the fact that signal and background cannot be separated sufficiently
	  means[1]-2.0*TMath::Abs(sigmas[1]),
	  means[2]-2.0*TMath::Abs(sigmas[2]),
	  means[3]-2.0*TMath::Abs(sigmas[3]),
	  means[4]-2.0*TMath::Abs(sigmas[4]),
	  means[5]-2.0*TMath::Abs(sigmas[5]),
	  means[6]-2.0*TMath::Abs(sigmas[6]));

  pWindows->SaveAs(GifName.Data());
  delete pWindows;
  for (int j=0;j<7;j++)
    {
      delete histogramarray[j];
    }
  fclose(mFile);      
  fclose(sFile);      
  fclose(uFile);      
  fclose(lFile);      
}


TH1F*  fitCorrelationExp(Int_t mom, Char_t* trackingmethod, Char_t* angle, Float_t& mean, Float_t& sigma, Int_t sector, Char_t* filelist,Float_t threshold=3.0)
  {
    Int_t LowerMomentumThreshold=(Float_t)mom*100.0;
    if(mom<6)
      Int_t UpperMomentumThreshold=(Float_t)(mom+1)*100.0;
    else
      Int_t UpperMomentumThreshold=1000.0;

    TFile* sumfiles[100]; 
    TNtuple* sumntuples[100];
    Char_t* names[100];

    for(Int_t i=0;i<100;i++)
    {
      sumfiles[i]=NULL;
      sumntuples[i]=NULL;
      names[i]=new Char_t[265];
    }

    ifstream nTupleList(filelist);
    Char_t name[256];
    Int_t wait;
    Int_t k=0;
    Int_t nFiles=0;

    while(nTupleList.getline(name,265) &&nFiles<10)
    { 
      cout << "opening file: " << name<< endl;
      sumfiles[k] = TFile::Open(name);
      sumntuples[k]=(TNtuple*)sumfiles[k]->FindObjectAny("N");
      cout << sumntuples[k] << endl;
      sprintf(names[k],"%s",name);
      k++;
      nFiles++;
    }

    
    Char_t MomCutString[256];
    sprintf(MomCutString,"%s%s%s%f%s%s%s%f","f_",trackingmethod,"_mom_exp>=",LowerMomentumThreshold,"&&f_",trackingmethod,"_mom_exp<",UpperMomentumThreshold);
    cout << "MomCut: " << MomCutString << endl;
    TCut MomCut(MomCutString);
    
    
    TCut StdCut("");
    TString CutString;

    TString TrackString(trackingmethod);
    if(TrackString.Contains("kick"))
      CutString="b_kicktrack==1&&b_corrRingKick==1 &&n_sector==";
    if(TrackString.Contains("spline"))
      CutString="b_splinetrack==1&&b_corrRingSpline==1 &&n_sector==";
    CutString+=sector;
    StdCut=CutString.Data();

    cout << "StdCut: " << StdCut.GetTitle() << endl;


    Char_t DrawString[128];
    sprintf(DrawString,"%s%s%s","f_delta",angle,">>+dAngle");
    cout << DrawString << endl;
    TH1F* dAngle = new TH1F("dAngle","dAngle",120,-4,4);
    for(Int_t f=0;sumntuples[f]!=NULL;f++)
      {
	cout << sumntuples[f] << endl;
	sumntuples[f]->Draw(DrawString,StdCut && MomCut,"");
      }

    Float_t center=dAngle->GetMean();
    //Float_t lower_threshold=-3.0;
    //Float_t upper_threshold=+3.0;

    Float_t lower_threshold=center-3.0;
    Float_t upper_threshold=center+3.0;

    TString* Signal= new TString("[0]*TMath::Exp(-( (x-[1])*(x-[1])) / (2*[2]*[2]) )");
    TString* Shadow= new TString("[4]*TMath::Exp(-( (x-[5])*(x-[5])) / (2*[6]*[6]) )");

    //TString* Background= new TString("+[3]+[4]*x+[5]*(x*x)+[6]*x*x*x");
    //TString* BackgroundAux= new TString("[0]+[1]*x+[2]*(x*x)+[3]*x*x*x");

    TString* Background= new TString("+[3]");
    TString* BackgroundAux= new TString("[0]");

    TString* ShadowAux= new TString("[0]*TMath::Exp(-( (x-[1])*(x-[1])) / (2*[2]*[2]) )");

    TString* SB= new TString(Signal->Data());
    SB->Append(Background->Data());
    //SB->Append("+");
    //SB->Append(Shadow->Data());

    TF1 *SUM = new TF1("SUM",SB->Data(),lower_threshold, upper_threshold);
    
    TF1 *sigfunc= new TF1("signal",Signal->Data(),lower_threshold, upper_threshold);
    TF1 *bgfunc= new TF1("background",BackgroundAux->Data(),lower_threshold, upper_threshold);
    //TF1 *shadowfunc= new TF1("shadow",Shadow->Data(),lower_threshold, upper_threshold);
    
    SUM->SetParameter(0,1000.0);
    SUM->SetParameter(1,0.0);
    SUM->SetParameter(2,1.0);
    
    SUM->SetParameter(3,1.0);
    
    /*
      SUM->SetParameter(4,1.0);
      SUM->SetParameter(5,1.0);
      SUM->SetParameter(6,1.0);
    */

    SUM->SetParName(0,"Signal_Constant");
    SUM->SetParName(1,"Signal_Mean");
    SUM->SetParName(2,"Signal_Sigma");  
    SUM->SetParName(3,"Offset");    
    /*
    SUM->SetParName(4,"Shadow_Constant"); 
    SUM->SetParName(5,"Shadow_Mean");
    SUM->SetParName(5,"Shadow_Sigma");  
    */
  
  
    sigfunc->SetLineColor(2);
    bgfunc->SetLineColor(3);
    //shadowfunc->SetLineColor(4);

    SUM->SetRange(lower_threshold, upper_threshold);      
    dAngle->Fit(SUM,"r");
    mean=SUM->GetParameter(1);
    sigma=SUM->GetParameter(2);

    return dAngle;
  }


TH1F*  fitCorrelationSim(Int_t mom, Char_t* trackingmethod, Char_t* angle, Float_t& mean, Float_t& sigma, Int_t sector, Char_t* filelist, Float_t threshold=3.0)
  {
    Int_t LowerMomentumThreshold=(Float_t)mom*100.0;
    if(mom<6)
      Int_t UpperMomentumThreshold=(Float_t)(mom+1)*100.0;
    else
      Int_t UpperMomentumThreshold=1000.0;

    TFile* sumfiles[100]; 
    TNtuple* sumntuples[100];
    Char_t* names[100];

    for(Int_t i=0;i<100;i++)
    {
      sumfiles[i]=NULL;
      sumntuples[i]=NULL;
      names[i]=new Char_t[265];
    }

    ifstream nTupleList(filelist);
    Char_t name[256];
    Int_t wait;
    Int_t k=0;
    Int_t nFiles=0;

    while(nTupleList.getline(name,265) &&nFiles<10)
    { 
      cout << "opening file: " << name<< endl;
      sumfiles[k] = TFile::Open(name);
      sumntuples[k]=(TNtuple*)sumfiles[k]->FindObjectAny("N");
      cout << sumntuples[k] << endl;
      //ntup[k]->SetDirectory(0);
      sprintf(names[k],"%s",name);
      k++;
      nFiles++;
    }
    
    Char_t MomCutString[256];
    sprintf(MomCutString,"%s%s%s%f%s%s%s%f","f_",trackingmethod,"_mom_exp>=",LowerMomentumThreshold,"&&f_",trackingmethod,"_mom_exp<",UpperMomentumThreshold);
    cout << "MomCut: " << MomCutString << endl;
    TCut MomCut(MomCutString);
    
    
    TCut StdCut("");
    TString CutString;

    TString TrackString(trackingmethod);
    if(TrackString.Contains("kick"))
      CutString="i_geant_corrflag>=76 && (i_geant_corrflag>=76 ||i_geant_corrflag==12 || i_geant_corrflag==14)&& b_kicktrack==1&&b_corrRingKick==1 &&n_sector==";
    if(TrackString.Contains("spline"))
      CutString="i_geant_corrflag>=76 && (i_geant_corrflag>=76 ||i_geant_corrflag==12 || i_geant_corrflag==14)&& b_splinetrack==1&&b_corrRingSpline==1 &&n_sector==";
    CutString+=sector;
    StdCut=CutString.Data();

    cout << "StdCut: " << StdCut.GetTitle() << endl;


    Char_t DrawString[128];
    sprintf(DrawString,"%s%s%s","f_delta",angle,">>+dAngle");
    cout << DrawString << endl;
    TH1F* dAngle = new TH1F("dAngle","dAngle",120,-4,4);
    for(Int_t f=0;sumntuples[f]!=NULL;f++)
      {
	cout << sumntuples[f] << endl;
	sumntuples[f]->Draw(DrawString,StdCut && MomCut,"");
      }

    Float_t center=dAngle->GetMean();
    //Float_t lower_threshold=center-2.5;
    //Float_t upper_threshold=center+2.5;

    Float_t lower_threshold=center-3.0;
    Float_t upper_threshold=center+3.0;

    TString* Signal= new TString("[0]*TMath::Exp(-( (x-[1])*(x-[1])) / (2*[2]*[2]) )");
    //TString* Signal= new TString("gaus");
  
    //TString* Background= new TString("+[3]+[4]*x+[5]*(x*x)+[6]*x*x*x");
    //TString* BackgroundAux= new TString("[0]+[1]*x+[2]*(x*x)+[3]*x*x*x");
    
    
    TString* SB= new TString(Signal->Data());
    //SB->Append(Background->Data());
    
    TF1 *SUM = new TF1("SUM",SB->Data(),lower_threshold, upper_threshold);
    
    //TF1 *sigfunc= new TF1("signal",Signal->Data(),lower_threshold, upper_threshold);
    //TF1 *bgfunc= new TF1("background",BackgroundAux->Data(),lower_threshold, upper_threshold);
    
        
    SUM->SetParameter(0,1000.0);
    SUM->SetParameter(1,0.0);
    SUM->SetParameter(2,1.0);

    //SUM->SetParameter(3,1.0);
    //SUM->SetParameter(4,1.0);
    //SUM->SetParameter(5,1.0);
    //SUM->SetParameter(6,1.0);
    
    SUM->SetParName(0,"Signal_Constant");
    SUM->SetParName(1,"Signal_Mean");
    SUM->SetParName(2,"Signal_Sigma");  
    /*
    SUM->SetParName(3,"BG_Const");
    SUM->SetParName(4,"BG_Linear");
    SUM->SetParName(5,"BG_Quadratic");  
    SUM->SetParName(6,"BG_Cubic");  
    */
    /*
    sigfunc->SetLineColor(2);
    bgfunc->SetLineColor(3);
    */
    SUM->SetRange(lower_threshold, upper_threshold);      
    dAngle->Fit(SUM,"r");
    mean=SUM->GetParameter(1);
    sigma=SUM->GetParameter(2);

    return dAngle;
  }
