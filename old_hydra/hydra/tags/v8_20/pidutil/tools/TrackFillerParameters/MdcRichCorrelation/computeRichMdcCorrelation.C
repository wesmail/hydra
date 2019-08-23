#define MAXSEC 6
#define MAXMOMBINS 9
computeRichMdcCorrelation(Bool_t useHighResTracking, Float_t fitrange,Float_t fixedwidth_theta,Float_t fixedwidth_phi,Bool_t sim)
{
  TChain* N = makeChain("N");

  //First make one histogram per sector and momentum range
  TH1F* histarray_theta[MAXSEC][MAXMOMBINS];
  TH1F* histarray_phi[MAXSEC][MAXMOMBINS];
  
  Float_t lowerMomentumBoundaries[9] = {0.0,  100.0,150.0,200.0,250.0,300.0,400.0,500.0, 1000.0};
  Float_t upperMomentumBoundaries[9] = {100.0,150.0,200.0,250.0,300.0,400.0,500.0,1000.0,3000.0};
  
  //Variables used internally to determine the momentum-bin which is associated to a certain histogram.
  Float_t LowerMomentumThreshold, UpperMomentumThreshold;
  Float_t pmthreshold[6]={200,200,200,200,400,200};

  for(Int_t sector=0;sector<MAXSEC;sector++)
    {
      for(Int_t mombin=0; mombin<MAXMOMBINS;mombin++)
	{

	  LowerMomentumThreshold=lowerMomentumBoundaries[mombin];
	  UpperMomentumThreshold=upperMomentumBoundaries[mombin];

	  //Build meaningful names and titles for all histograms
	  TString histname_theta("Delta_theta_sector_");
	  TString histname_phi("Delta_phi_sector_");
	  
	  histname_theta+=sector;
	  histname_phi+=sector;
	  
	  histname_theta.Append("_");
	  histname_phi.Append("_");
	  
	  histname_theta+=(Int_t)LowerMomentumThreshold;
	  histname_phi+=(Int_t)LowerMomentumThreshold;
	  
	  histname_theta.Append("_to_");
	  histname_phi.Append("_to_");
	  
	  histname_theta+=(Int_t)UpperMomentumThreshold;
	  histname_phi+=(Int_t)UpperMomentumThreshold;
	  
	  cout << "Build histograms: " << endl;
	  cout << histname_theta.Data() << endl;
	  cout << histname_phi.Data() << endl;
	  histarray_theta[sector][mombin]= new TH1F(histname_theta.Data(),histname_theta.Data(),200,-10,10);
	  histarray_phi[sector][mombin]= new TH1F(histname_phi.Data(),histname_phi.Data(),200,-10,10);
	}
    }
  

  cout << "All files together have: "<< N->GetEntries()<< " entries!" << endl;
  
  //Variables required for filling the histograms with suitable data
  Float_t f_track_theta;
  Float_t f_track_phi;
  
  Float_t f_deltatheta;
  Float_t f_deltaphi;
  
  Float_t f_mdc_chi2;
  
  Float_t b_ring;
  
  Float_t f_shw_tof, f_tof_tof;
  
  Float_t f_rk_quality;
  
  Float_t b_splinetrack, b_kicktrack, b_rktrack;
  
  Float_t n_sector, n_system;
  
  Float_t f_kick_mom_exp, f_rk_mom_exp, f_track_momentum;

  Float_t n_rich_patmatrix;

  Float_t i_geant_corrflag;
  
  N->SetBranchAddress("f_track_theta",&f_track_theta);
  N->SetBranchAddress("f_track_phi",&f_track_phi);
  
  N->SetBranchAddress("f_deltatheta",&f_deltatheta);
  N->SetBranchAddress("f_deltaphi",&f_deltaphi);
  
  N->SetBranchAddress("f_mdc_chi2",&f_mdc_chi2);
  
  N->SetBranchAddress("b_ring",&b_ring);
  
  N->SetBranchAddress("f_tof_tof",&f_tof_tof);
  N->SetBranchAddress("f_shw_tof",&f_shw_tof);
  N->SetBranchAddress("n_system",&n_system);
  
  N->SetBranchAddress("f_rk_quality",&f_rk_quality);
  
  N->SetBranchAddress("b_splinetrack",&b_splinetrack);
  N->SetBranchAddress("b_kicktrack",&b_kicktrack);
  N->SetBranchAddress("b_rktrack",&b_rktrack);
  
  N->SetBranchAddress("n_sector",&n_sector);

  if(sim)
    {
      N->SetBranchAddress("i_geant_corrflag",&i_geant_corrflag);
    }
  
  N->SetBranchAddress("n_rich_patmatrix", &n_rich_patmatrix);
  

  //Depending on the type of tracking chosen by the user we use the corresponding
  //momentum value.
  if(!useHighResTracking)
    {
      N->SetBranchAddress("f_kick_mom_exp",&f_track_momentum);
    }
  else
    {
      N->SetBranchAddress("f_rk_mom_exp",&f_track_momentum);
    }
  
  //for(Int_t i=0;i< 10;i++)
  for(Int_t i=0;i< N->GetEntries();i++)
    {
      N->GetEntry(i);
      if(!(i%100000))
	{
	  cout << "Analyzing Entry: " << i << endl;
	}
      if(sim)
	{
	  if(i_geant_corrflag!=12 && i_geant_corrflag!=14 && i_geant_corrflag!=78 && i_geant_corrflag!=76)
	    {
	      //bad correlation
	      continue;
	    }
	}
      if(b_ring<1) //no ring - this track is not interesting for us
	{
	  continue;
	}
      
      if(n_rich_patmatrix<pmthreshold[(Int_t)n_sector]) //bad ring -> continue
	{
	  continue;
	}
      
      if(f_track_theta<25 || f_track_theta>75 || (Int_t)f_track_phi%60<10 || (Int_t)f_track_phi%60>50) //track is outside trusted range
	{
	  continue;
	}
      
      if(!useHighResTracking) //Kickplane required
	{
	  if(b_kicktrack<=0)  //No kicktrack 
	    {
	      continue;	   
	    }	
	}  
      else
	{
	  if(b_rktrack<=0 || b_splinetrack<=0 || f_rk_quality>1000 )
	    {
	      continue;
	    }
	}
      //cout << "Checking mdc chi-square " << endl;
      if(f_mdc_chi2<0)
	{
	  continue;	      
	}
      
      //cout << "Checking tof " << endl;
      if(n_system<0)
	{
	  continue;
	}
      
      //cout << "Checking mombin " << endl;
      Int_t targetmombin=-1;
      targetmombin=getmombin(f_track_momentum);
      histarray_theta[n_sector][targetmombin]->Fill(f_deltatheta);
      histarray_phi[n_sector][targetmombin]->Fill(f_deltaphi);
      
      
    }
  
  //Now all histograms are filled - we need to fit them!
  //This is what we care for in essence.
  Float_t meanvalues_theta[MAXSEC][MAXMOMBINS];
  Float_t sigmavalues_theta[MAXSEC][MAXMOMBINS];
  
  Float_t meanvalues_phi[MAXSEC][MAXMOMBINS];
  Float_t sigmavalues_phi[MAXSEC][MAXMOMBINS];
  
  
  //Loop over all histograms and fit them
  for(Int_t sector=0;sector<MAXSEC;sector++)
    {

      for(Int_t mombin=0; mombin<MAXMOMBINS;mombin++)
	{
	  //This function has to be executed once for the polar angle and once for the azimuthal angle
	  fitHistogram(histarray_theta[sector][mombin],
		       meanvalues_theta[sector][mombin],
		       sigmavalues_theta[sector][mombin], 
		       fitrange,fixedwidth_theta);
	  
	  fitHistogram(histarray_phi[sector][mombin],
		       meanvalues_phi[sector][mombin],
		       sigmavalues_phi[sector][mombin], 
		       fitrange,fixedwidth_phi);
	  
	}
      
    }
  
  
  for(Int_t sector=0;sector<MAXSEC;sector++)
    {
      TString phiCanvasName("phiCorrelationSector_");
      phiCanvasName+=sector;
      TCanvas* phiCanvas = new TCanvas(phiCanvasName,phiCanvasName);
      phiCanvas->Divide(3,3);
      
      for(Int_t mombin=0; mombin<MAXMOMBINS;mombin++)
	{
	  phiCanvas->cd(mombin+1);
	  gStyle->SetOptStat(0);
	  histarray_phi[sector][mombin]->Draw();
	}
      phiCanvasName.Append(".ps");
      phiCanvas->SaveAs(phiCanvasName.Data());
    }
  
  for(Int_t sector=0;sector<MAXSEC;sector++)
    {
      TString thetaCanvasName("thetaCorrelationSector_");
      thetaCanvasName+=sector;
      TCanvas* thetaCanvas = new TCanvas(thetaCanvasName,thetaCanvasName);
      thetaCanvas->Divide(3,3);
      
      for(Int_t mombin=0; mombin<MAXMOMBINS;mombin++)
	{
	  thetaCanvas->cd(mombin+1);
	  gStyle->SetOptStat(0);
	  histarray_theta[sector][mombin]->Draw();
	}	  
      thetaCanvasName.Append(".ps");
      thetaCanvas->SaveAs(thetaCanvasName.Data());
    }  

  TFile* histogramfile = TFile::Open("CorrelationHistograms.RichMdc.root","RECREATE");    
  
  for(Int_t sector=0;sector<MAXSEC;sector++)
    {
      for(Int_t mombin=0; mombin<MAXMOMBINS;mombin++)
	{
	  histarray_theta[sector][mombin]->Write();
	  histarray_phi[sector][mombin]->Write();
	}
    }
  histogramfile->Close();
  
  FILE* sigmatheta  = fopen("sigmatheta.txt","a");
  FILE* sigmaphi  = fopen("sigmaphi.txt","a");
  FILE* meantheta  = fopen("meantheta.txt","a");
  FILE* meanphi  = fopen("meanphi.txt","a");
  
  for(Int_t sector=0;sector<MAXSEC;sector++)
    {
      fprintf(meantheta,"Mean:{%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f},\n",
	      meanvalues_theta[sector][0],meanvalues_theta[sector][1],meanvalues_theta[sector][2],
	      meanvalues_theta[sector][3],meanvalues_theta[sector][4],meanvalues_theta[sector][5],
	      meanvalues_theta[sector][6],meanvalues_theta[sector][7],meanvalues_theta[sector][8]);
      //meanvalues_theta[sector][9],meanvalues_theta[sector][10],meanvalues_theta[sector][11]);
    
      fprintf(sigmatheta,"Sigma:{%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f},\n",
	      sigmavalues_theta[sector][0],sigmavalues_theta[sector][1],sigmavalues_theta[sector][2],
	      sigmavalues_theta[sector][3],sigmavalues_theta[sector][4],sigmavalues_theta[sector][5],
	      sigmavalues_theta[sector][6],sigmavalues_theta[sector][7],sigmavalues_theta[sector][8]);
      //sigmavalues_theta[sector][9],sigmavalues_theta[sector][10],sigmavalues_theta[sector][11]);
      
      fprintf(meanphi,"Mean:{%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f},\n",
	      meanvalues_phi[sector][0],meanvalues_phi[sector][1],meanvalues_phi[sector][2],
	      meanvalues_phi[sector][3],meanvalues_phi[sector][4],meanvalues_phi[sector][5],
	      meanvalues_phi[sector][6],meanvalues_phi[sector][7],meanvalues_phi[sector][8]);
      //meanvalues_phi[sector][9],meanvalues_phi[sector][10],meanvalues_phi[sector][11]);
      fprintf(sigmaphi,"Sigma:{%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f},\n",
	      sigmavalues_phi[sector][0],sigmavalues_phi[sector][1],sigmavalues_phi[sector][2],
	      sigmavalues_phi[sector][3],sigmavalues_phi[sector][4],sigmavalues_phi[sector][5],
	      sigmavalues_phi[sector][6],sigmavalues_phi[sector][7],sigmavalues_phi[sector][8]);
      //sigmavalues_phi[sector][9],sigmavalues_phi[sector][10],sigmavalues_phi[sector][11]);
    }
  fclose(meantheta);
  fclose(meanphi);
  fclose(sigmatheta);
  fclose(sigmaphi);
  
}

fitHistogram(TH1F* histogram, Float_t& meanvalue,Float_t& sigmavalue, Float_t fitrange, Float_t fixedwidth)
{
  //fitrange
  Float_t center=0.0;
  Float_t lower_threshold=0.0;
  Float_t upper_threshold=0.0;
  
  //We build a three component function (see documentation): Two gaussians and a flat background
  //The second gaussian will be given a user-supplied fiyed width later on
  TString* Signal= new TString("[0]*TMath::Exp(-( (x-[1])*(x-[1])) / (2*[2]*[2]) )");
  TString* Shadow= new TString("+[3]*TMath::Exp(-( (x-[4])*(x-[4])) / (2*[5]*[5]) )");
  TString* Background= new TString("+[6]");
  
  
  TString* SB= new TString(Signal->Data());
  SB->Append(Shadow->Data());
  SB->Append(Background->Data());
  
  TF1 *SUM = new TF1("SUM",SB->Data(),-10.0, 10.0);
  
  //Determine suitable starting points for the fit and the range
  center = histogram->GetMean();
  lower_threshold=center-fitrange;
  upper_threshold=center+fitrange;
  
  if(lower_threshold<-7.0)
    {
      lower_threshold=-7.0;
    }
  
  if(upper_threshold>7.0)
    {
      upper_threshold=7.0;
    }
  
  SUM->SetParameter(0,100.0);
  SUM->SetParameter(1,center);
  SUM->SetParameter(2,1.0);
  
  SUM->SetParameter(3,1.0);
  SUM->SetParameter(4,center);
  SUM->SetParameter(5,2.0);
  
  SUM->SetParameter(6,1.0);
  
  
  SUM->SetParLimits(0,0.00,1.0e+8);
  SUM->SetParLimits(1,-3.0,3.0);
  SUM->SetParLimits(2,0.4,3.0);
  
  SUM->SetParLimits(3,0.00,1.0e+8);
  SUM->SetParLimits(4,-3.0,3.0);
  
  //If the user supplies a default value for one of the gaussian components we 
  //freeze it to this value
  if(fixedwidth>0.0)
    {
      SUM->FixParameter(5,fixedwidth);
    }
  
  SUM->SetParLimits(6,0.00,1.0e+8);
  
  
  SUM->SetParName(0,"Signal_Constant");
  SUM->SetParName(1,"Signal_Mean");
  SUM->SetParName(2,"Signal_Sigma");  
  SUM->SetParName(3,"Shadow_Constant");
  SUM->SetParName(4,"Shadow_Mean");
  SUM->SetParName(5,"Shadow_Sigma");  
  SUM->SetParName(6,"Background_Constant");
  
  //Here the fit is executed
  SUM->SetRange(lower_threshold, upper_threshold);      
  //cout << "Fitting now sector: " << sector <<", momentum bin: " << mombin << "..." <<endl; 
  histogram->Fit(SUM,"r");
  
  //As we have two components of the fit we take the more narrow one (by convention) to 
  //parametrize the distribution of the lepton-induced track/ring pairs.
  if(abs(SUM->GetParameter(2))<abs(SUM->GetParameter(5)))
    {
      meanvalue=SUM->GetParameter(1);
      sigmavalue=SUM->GetParameter(2);
    }
  else()
    {
      meanvalue=SUM->GetParameter(4);
      sigmavalue=SUM->GetParameter(5);
    }
}

Int_t getmombin(Float_t trackmomentum)
{
  //cout << "get" << endl;
  if(trackmomentum<100) return 0;
  if(trackmomentum<150) return 1;
  if(trackmomentum<200) return 2;
  if(trackmomentum<250) return 3;
  if(trackmomentum<300) return 4;
  if(trackmomentum<400) return 5;
  if(trackmomentum<500) return 6;
  if(trackmomentum<1000) return 7;
  return 8;
}
