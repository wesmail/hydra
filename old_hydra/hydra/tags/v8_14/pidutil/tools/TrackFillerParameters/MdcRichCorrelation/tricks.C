
//This function demonstrates that the background from hadrons is reasonably flat when the proper geometrical
//selection is done. It uses geant information and can only be applied to urqmd/pluto data.
TH1F* hadronbackgroundsim(Float_t threshold,Float_t lowerp, Float_t upperp, Float_t pmlower, Float_t pmupper)
{
  TH1F* dAngle_bg = new TH1F("dAngle_bg","dAngle_bg",200,-10,10);
  Char_t draw[512];
  //sprintf(draw,"%s%f%s%f"," f_track_theta>40 &&!(i_geant_corrflag==68 || i_geant_corrflag==70 ||i_geant_corrflag>=76 ||i_geant_corrflag==12 || i_geant_corrflag==14)&& b_ring==1 && i_geant_pid>3 && f_rich_centroid>0 && f_kick_mom_exp>",lowerp,"&&f_kick_mom_exp<",upperp);
  sprintf(draw,"%s%f%s%f%s%f%s%f","f_track_theta>40 && (f_track_phi-(n_sector-1)*60>140 && f_track_phi-(n_sector-1)*60<160)&& f_track_theta<75 && b_ring==1 && i_geant_pid>3 && f_rich_centroid>0 && b_rktrack &&n_rich_patmatrix>",pmlower,"&&n_rich_patmatrix<",pmupper,"&& f_rk_mom_exp>",lowerp,"&&f_rk_mom_exp<",upperp);
  TString* Draw= new TString(draw);

  N->Draw("f_deltaphi>>dAngle_bg",Draw->Data());
  Float_t center=dAngle_bg->GetMean();
  Float_t lower_threshold=center-threshold;
  Float_t upper_threshold=center+threshold;
  
  TString* Background= new TString("[0]+[1]*x+[2]*x*x");
  TString* SS= new TString(Background->Data());
  

  TF1 *SUM = new TF1("SUM",SS->Data(),lower_threshold, upper_threshold);

  SUM->SetParameter(0,100.0);
  
  
  SUM->SetParName(0,"Flat_Background");
  SUM->SetParName(1,"Lin_Background");
  SUM->SetParName(2,"Quad_Background");
  SUM->FixParameter(1,0.0);
  SUM->FixParameter(2,0.0);

  //sigfunc->SetLineColor(2);

  SUM->SetRange(lower_threshold, upper_threshold);      
  dAngle_bg->Fit(SUM,"r");
  SUM->SetLineColor(3);
  SUM->Draw("same");
  Float_t nbins=((2*threshold)/20)*200 - 3;
  cout << "Chi2/NDF: "<<SUM->GetChisquare()/nbins << endl;
  return dAngle_bg;
}


//This function demonstrates that the background from hadrons is reasonably flat when the proper geometrical
//selection is done. It uses only experimental information and can be applied to experimental as well as to urqmd/pluto data.
TH1F* hadronbackgroundexp(Float_t threshold,Float_t lowerp, Float_t upperp, Float_t pmlower, Float_t pmupper)
{
  TH1F* dAngle_bg = new TH1F("dAngle_bg","dAngle_bg",200,-10,10);
  Char_t draw[512];
  sprintf(draw,"%s%f%s%f%s%f%s%f","f_track_theta>40 && (f_track_phi-(n_sector-1)*60>140 && f_track_phi-(n_sector-1)*60<160)&& f_track_theta<75 && b_ring==1 && f_rk_beta<0.7 && f_rich_centroid>0 && b_rktrack &&n_rich_patmatrix>",pmlower,"&&n_rich_patmatrix<",pmupper,"&& f_rk_mom_exp>",lowerp,"&&f_rk_mom_exp<",upperp);
  TString* Draw= new TString(draw);
  cout << Draw->Data() << endl;
  N->Draw("f_deltaphi>>dAngle_bg",Draw->Data());
  Float_t center=dAngle_bg->GetMean();
  Float_t lower_threshold=center-threshold;
  Float_t upper_threshold=center+threshold;
  
  TString* Background= new TString("[0]+[1]*x+[2]*x*x");
  TString* SS= new TString(Background->Data());
  

  TF1 *SUM = new TF1("SUM",SS->Data(),lower_threshold, upper_threshold);

  SUM->SetParameter(0,100.0);
  
  
  SUM->SetParName(0,"Flat_Background");
  SUM->SetParName(1,"Lin_Background");
  SUM->SetParName(2,"Quad_Background");
  SUM->FixParameter(1,0.0);
  SUM->FixParameter(2,0.0);

  //sigfunc->SetLineColor(2);

  SUM->SetRange(lower_threshold, upper_threshold);      
  dAngle_bg->Fit(SUM,"r");
  SUM->SetLineColor(3);
  SUM->Draw("same");
  Float_t nbins=((2*threshold)/20)*200 - 3;
  cout << "Chi2/NDF: "<<SUM->GetChisquare()/nbins << endl;
  return dAngle_bg;
}


TH1F* signal(Float_t threshold,Float_t lowerp, Float_t upperp, Float_t& smallsig, Float_t& largesig, Float_t& chisquare, Float_t fixedwidth=0.0)
{
  TChain* N = maekChain("N");
  //TFile *input = TFile::Open("../input/Nov02/sim/gen3/pdf-data_sum_sim_nov02_gen3.root");
  TH1F* dAngle_sig = new TH1F("dAngle_sig","dAngle_sig",200,-10,10);

  Char_t draw[256];

  sprintf(draw,"%s%f%s%f","f_track_theta>30 && (f_track_phi%60>20 && f_track_phi%60<40) &&(i_geant_corrflag>=76 ||i_geant_corrflag==12 || i_geant_corrflag==14)&& b_ring==1 && f_rk_mom_exp>",lowerp,"&&f_rk_mom_exp<",upperp);
  TString* Draw= new TString(draw);
  TNtuple* N = (TNtuple*)gDirectory->FindObjectAny("N");
  N->Draw("f_deltaphi>>dAngle_sig",Draw->Data(),"");
  Float_t center=dAngle_sig->GetMean();
  cout << "Mean: " << center;
  Float_t lower_threshold=center-threshold;
  Float_t upper_threshold=center+threshold;

  TString* Signal= new TString("[0]*TMath::Exp(-( (x-[1])*(x-[1])) / (2*[2]*[2]) )");
  TString* Shadow= new TString("+[3]*TMath::Exp(-( (x-[4])*(x-[4])) / (2*[5]*[5]) )");

  TString* SS= new TString(Signal->Data());
  SS->Append(Shadow->Data());

  TF1 *SUM = new TF1("SUM",SS->Data(),lower_threshold, upper_threshold);
  
  SUM->SetParameter(0,100.0);
  SUM->SetParameter(1,0.0);
  SUM->SetParameter(2,1.0);
  
  SUM->SetParameter(3,1.0);
  SUM->SetParameter(4,0.0);
  SUM->SetParameter(5,2.0);


  SUM->SetParLimits(0,0.0,1.0e+8);  
  SUM->SetParLimits(1,-3.0,3.0);  
  SUM->SetParLimits(2,0.4,3.0);  

  SUM->SetParLimits(3,0.0,1.0e+8);
  SUM->SetParLimits(4,-3.0,3.0);
  SUM->SetParLimits(5,1.0,5.0);


  SUM->SetParName(0,"Signal_Constant");
  SUM->SetParName(1,"Signal_Mean");
  SUM->SetParName(2,"Signal_Sigma");  
  SUM->SetParName(3,"Shadow_Constant");
  SUM->SetParName(4,"Shadow_Mean");
  SUM->SetParName(5,"Shadow_Width");  

  if(fixedwidth>0.0)
    {
      SUM->FixParameter(5,fixedwidth);
    }
  
  SUM->SetRange(lower_threshold, upper_threshold);      

  dAngle_sig->Fit(SUM,"r");
  SUM->SetLineColor(4);
  SUM->Draw("same");
  Float_t nbins=((2*threshold)/20)*200 - 5;
  cout << "Chi2/NDF: "<<SUM->GetChisquare()/nbins << endl;

  if(SUM->GetParameter(2)<SUM->GetParameter(5))
    {
      smallsig=SUM->GetParameter(2);
      largesig=SUM->GetParameter(5);
    }
  else()
    {
      smallsig=SUM->GetParameter(5);
      largesig=SUM->GetParameter(2);
    }

  chisquare=SUM->GetChisquare()/nbins;
  dAngle_sig->SetDirectory(0);
  
  return dAngle_sig;
}

TH1F* ghosts(Float_t threshold,Float_t lowerp, Float_t upperp, Float_t& smallsig,Float_t& largesig,Float_t& chisquare, Float_t fixedwidth=0.0)
{
  TH1F* dAngle_crazy = new TH1F("dAngle_crazy","dAngle_crazy",200,-10,10);
  Char_t draw[256];
  sprintf(draw,"%s%f%s%f","f_track_theta>25 &&(i_geant_corrflag==68 || i_geant_corrflag==70)&& b_corrRingKick==1 && f_kick_mom_exp>",lowerp,"&&f_kick_mom_exp<",upperp);
  TString* Draw= new TString(draw);
  TNtuple* N = (TNtuple*)gDirectory->FindObjectAny("N");
  N->Draw("f_deltaphi>>dAngle_crazy",Draw->Data());
  Float_t center=dAngle_crazy->GetMean();
  cout << "Mean: " << center;
  Float_t lower_threshold=center-threshold;
  Float_t upper_threshold=center+threshold;
  
  TString* Signal= new TString("[0]*TMath::Exp(-( (x-[1])*(x-[1])) / (2*[2]*[2]) )");
  TString* Shadow= new TString("+[3]*TMath::Exp(-( (x-[4])*(x-[4])) / (2*[5]*[5]) )");

  TString* SS= new TString(Signal->Data());
  SS->Append(Shadow->Data());

  TF1 *SUM = new TF1("SUM",SS->Data(),lower_threshold, upper_threshold);
  

  SUM->SetParameter(0,100.0);
  SUM->SetParameter(1,0.0);
  SUM->SetParameter(2,1.0);
  
  SUM->SetParameter(3,1.0);
  SUM->SetParameter(4,0.0);
  SUM->SetParameter(5,2.0);

  SUM->SetParLimits(0,0.0,1.0e+8);    
  SUM->SetParLimits(1,-3.0,3.0);  
  SUM->SetParLimits(2,0.4,3.0);
  SUM->SetParLimits(3,0.00,1.0e+8);
  SUM->SetParLimits(4,-3.0,3.0);
  SUM->SetParLimits(5,1.0,5.0);
  if(fixedwidth>0.0)
    {
      SUM->FixParameter(5,fixedwidth);
    }
  
  SUM->SetParName(0,"Signal_Constant");
  SUM->SetParName(1,"Signal_Mean");
  SUM->SetParName(2,"Signal_Sigma");  
  SUM->SetParName(3,"Shadow_Constant");
  SUM->SetParName(4,"Shadow_Mean");
  SUM->SetParName(5,"Shadow_Sigma");  



  SUM->SetRange(lower_threshold, upper_threshold);      
  dAngle_crazy->Fit(SUM,"r");
  Float_t nbins=((2*threshold)/20)*200 - 3;
  cout << "Chi2/NDF: "<<SUM->GetChisquare()/nbins << endl;
  SUM->SetLineColor(2);
  SUM->Draw("same");
  if(SUM->GetParameter(2)<SUM->GetParameter(5))
    {
      smallsig=SUM->GetParameter(2);
      largesig=SUM->GetParameter(5);
    }
  else()
    {
      smallsig=SUM->GetParameter(5);
      largesig=SUM->GetParameter(2);
    } 
  chisquare=SUM->GetChisquare()/nbins;
  return dAngle_crazy;

}


//This function demonstrates that the background from hadrons is reasonably flat when the proper geometrical
//selection is done. It uses geant information and can only be applied to urqmd/pluto data.
TH1F* hadronbackground(Float_t threshold,Float_t lowerp, Float_t upperp, Float_t pmlower, Float_t pmupper)
{
  TH1F* dAngle_bg = new TH1F("dAngle_bg","dAngle_bg",200,-10,10);
  Char_t draw[512];
  //sprintf(draw,"%s%f%s%f"," f_track_theta>40 &&!(i_geant_corrflag==68 || i_geant_corrflag==70 ||i_geant_corrflag>=76 ||i_geant_corrflag==12 || i_geant_corrflag==14)&& b_ring==1 && i_geant_pid>3 && f_rich_centroid>0 && f_kick_mom_exp>",lowerp,"&&f_kick_mom_exp<",upperp);
  sprintf(draw,"%s%f%s%f%s%f%s%f","f_track_theta>40 && (f_track_phi-(n_sector-1)*60>140 && f_track_phi-(n_sector-1)*60<160)&& f_track_theta<75 && b_ring==1 && i_geant_pid>3 && f_rich_centroid>0 && b_rktrack &&n_rich_patmatrix>",pmlower,"&&n_rich_patmatrix<",pmupper,"&& f_rk_mom_exp>",lowerp,"&&f_rk_mom_exp<",upperp);
  TString* Draw= new TString(draw);

  N->Draw("f_deltaphi>>dAngle_bg",Draw->Data());
  Float_t center=dAngle_bg->GetMean();
  Float_t lower_threshold=center-threshold;
  Float_t upper_threshold=center+threshold;
  
  TString* Background= new TString("[0]+[1]*x+[2]*x*x");
  TString* SS= new TString(Background->Data());
  

  TF1 *SUM = new TF1("SUM",SS->Data(),lower_threshold, upper_threshold);

  SUM->SetParameter(0,100.0);
  
  
  SUM->SetParName(0,"Flat_Background");
  SUM->SetParName(1,"Lin_Background");
  SUM->SetParName(2,"Quad_Background");
  SUM->FixParameter(1,0.0);
  SUM->FixParameter(2,0.0);

  //sigfunc->SetLineColor(2);

  SUM->SetRange(lower_threshold, upper_threshold);      
  dAngle_bg->Fit(SUM,"r");
  SUM->SetLineColor(3);
  SUM->Draw("same");
  Float_t nbins=((2*threshold)/20)*200 - 3;
  cout << "Chi2/NDF: "<<SUM->GetChisquare()/nbins << endl;
  return dAngle_bg;
}



TH1F* total(Float_t threshold,Float_t lowerp, Float_t upperp, Float_t& smallsig, Float_t& largesig,Float_t& chisquare, Float_t fixedwidth=0.0,Int_t sector=-1)
{
  TH1F* dAngle_total = new TH1F("dAngle_total","dAngle_total",200,-10,10);
  TNtuple* N = (TNtuple*)gDirectory->FindObjectAny("N");
  Char_t draw[256];
  sprintf(draw,"%s%f%s%f"," f_track_theta>30 && (f_track_phi%60>20 && f_track_phi%60<40)&& f_track_theta<75 && b_ring==1 && n_rich_patmatrix>400 && f_rk_mom_exp>",lowerp,"&&f_rk_mom_exp<",upperp);
  TString* Draw= new TString(draw);
  
  if(sector>-1)
    {
      Char_t dd[16];
      sprintf(dd,"%s%d","&& n_sector==",sector);
      Draw->Append(dd);
    }
  
  N->Draw("f_deltaphi>>dAngle_total",Draw->Data());

  Float_t center=dAngle_total->GetMean();
  Float_t lower_threshold=center-threshold;
  Float_t upper_threshold=center+threshold;
  
  TString* Signal= new TString("[0]*TMath::Exp(-( (x-[1])*(x-[1])) / (2*[2]*[2]) )");
  TString* Shadow= new TString("+[3]*TMath::Exp(-( (x-[4])*(x-[4])) / (2*[5]*[5]) )");
  TString* Background= new TString("+[6]+x*[7]+x*x*[8]");


  TString* Total= new TString(Signal->Data());
  Total->Append(Shadow->Data());
  Total->Append(Background->Data());

  TF1 *SUM = new TF1("SUM",Total->Data(),lower_threshold, upper_threshold);


  SUM->SetParameter(0,100.0);
  SUM->SetParameter(1,0.0);
  SUM->SetParameter(2,1.0);
  
  SUM->SetParameter(3,1.0);
  SUM->SetParameter(4,0.0);
  SUM->SetParameter(5,2.0);

  SUM->SetParameter(6,1.0);
  SUM->SetParameter(7,0.0);
  SUM->SetParameter(8,0.0);
  

  SUM->SetParLimits(0,0.00,1.0e+8);
  SUM->SetParLimits(1,-3.0,3.0);
  SUM->SetParLimits(2,0.4,3.0);

  SUM->SetParLimits(3,0.00,1.0e+8);
  SUM->SetParLimits(4,-3.0,3.0);

  if(fixedwidth>0.0)
    {
      SUM->FixParameter(5,fixedwidth);
    }

  SUM->SetParLimits(6,0.00,1.0e+8);
  SUM->FixParameter(7,0.0);
  SUM->FixParameter(8,0.0);

  
  SUM->SetParName(0,"Signal_Constant");
  SUM->SetParName(1,"Signal_Mean");
  SUM->SetParName(2,"Signal_Sigma");  
  SUM->SetParName(3,"Shadow_Constant");
  SUM->SetParName(4,"Shadow_Mean");
  SUM->SetParName(5,"Shadow_Sigma");  

  SUM->SetParName(6,"Background_Constant");
  SUM->SetParName(7,"Background_Linear");
  SUM->SetParName(8,"Background_Quad");  


  SUM->SetRange(lower_threshold, upper_threshold);      
  dAngle_total->Fit(SUM,"r");
  SUM->SetLineColor(1);
  SUM->Draw("same");
  Float_t nbins=((2*threshold)/20)*200 - 9 - 3;
  cout << "Chi2/NDF: "<<SUM->GetChisquare()/nbins << endl;
   if(SUM->GetParameter(2)<SUM->GetParameter(5))
    {
      smallsig=SUM->GetParameter(2);
      largesig=SUM->GetParameter(5);
    }
  else()
    {
      smallsig=SUM->GetParameter(5);
      largesig=SUM->GetParameter(2);
    }
   chisquare=SUM->GetChisquare()/nbins;
   return dAngle_total;
}

systematics(Float_t width, Float_t lowerp, Float_t upperp)
{
  TH1F* signal;
  TH1F* ghosts;
  TH1F* bg;
  TH1F* all;
  signal=signal(width,lowerp,upperp);
  signal->SetLineColor(4);
  ghosts=ghosts(width,lowerp,upperp);
  ghosts->SetLineColor(2);
  bg=background(width,lowerp,upperp);
  bg->SetLineColor(3);
  all=total(width,lowerp,upperp);
  
  all->Draw();
  signal->Draw("same");
  bg->Draw("same");
  ghosts->Draw("same");
}

loop(Float_t fixedwidth=0.0, Int_t mode=0,Int_t sector=-1)
{
  for(Int_t i=0;i<700;i+=100)
    {
      investigateSignalStability(i, fixedwidth, mode);
    }
}

investigateSignalStability(Int_t lowerp, Float_t fixedwidth=0.0,Int_t mode,Int_t sector=-1)
{
  if(mode!=0 && mode!=1 && mode!=2)
    {
      cout << "invalid mode - Aborting!"<< endl;
      exit(-1);
    }

  Int_t upperp=lowerp+100;
  if(lowerp==600)
    {
      upperp=1000;
    }
  TString filename_smallsigma("SmallWidthVsRange_");
  if(mode==0)
    {
      filename_smallsigma.Append("Signal_");
    }
  if(mode==1)
    {
      filename_smallsigma.Append("S+B_");
    }
  if(mode==2)
    {
      filename_smallsigma.Append("Ghosts_");
    }
  filename_smallsigma+=lowerp;
  filename_smallsigma.Append("_to_");
  filename_smallsigma+=upperp;
  filename_smallsigma.Append(".txt");
  FILE* smallsigFile = fopen(filename_smallsigma.Data(),"a");

  TString filename_largesigma("LargeWidthVsRange_");
  if(mode==0)
    {
      filename_largesigma.Append("Signal_");
    }
  if(mode==1)
    {
      filename_largesigma.Append("S+B_");
    }

  if(mode==2)
    {
      filename_largesigma.Append("Ghosts_");
    }

  filename_largesigma+=lowerp;
  filename_largesigma.Append("_to_");
  filename_largesigma+=upperp;
  filename_largesigma.Append(".txt");
  FILE* largesigFile = fopen(filename_largesigma.Data(),"a");

  TString filename_chisquare("Chi2VsRange_");
  if(mode==0)
    {
      filename_chisquare.Append("Signal_");
    }
  if(mode==1)
    {
      filename_chisquare.Append("S+B_");
    }
  if(mode==2)
    {
      filename_chisquare.Append("Ghosts_");
    }

  filename_chisquare+=lowerp;
  filename_chisquare.Append("_to_");
  filename_chisquare+=upperp;
  filename_chisquare.Append(".txt");
  FILE* chisquareFile = fopen(filename_chisquare.Data(),"a");



  TH1F* ptr;

  Float_t chisquare[6];
  Float_t sigmasmall[6];
  Float_t sigmalarge[6];

  for(Int_t i = 3;i<9;i++ )
    {
      if(mode==0)
	{
	  ptr = signal((Float_t)i,lowerp, upperp,sigmasmall[i-3],sigmalarge[i-3],chisquare[i-3],fixedwidth);
	}
      if(mode==1)
	{
	  ptr =  total((Float_t)i,lowerp, upperp,sigmasmall[i-3],sigmalarge[i-3],chisquare[i-3],fixedwidth,sector);
	}
      if(mode==2)
	{
	  ptr =  ghosts((Float_t)i,lowerp, upperp,sigmasmall[i-3],sigmalarge[i-3],chisquare[i-3],fixedwidth);
	}

      fprintf(chisquareFile,"%d\t%.2f\n",i,chisquare[i-3]);
      fprintf(smallsigFile,"%d\t%.2f\n",i,sigmasmall[i-3]);
      fprintf(largesigFile,"%d\t%.2f\n",i,sigmalarge[i-3]);
    }
}
