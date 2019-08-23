#define MAXSEC 6
#define MAXMOMBINS 12
computeRichMdcCorrelation(Char_t* filelist, Bool_t useHighResTracking, Float_t fitrange,Float_t fixedwidth_theta,Float_t fixedwidth_phi)
{
  //First make one histogram per sector and momentum range
  TH1F* histarray_theta[6][12];
  TH1F* histarray_phi[6][12];
  
  //Variables needed internally to determine the momentum-bin which is associated to a certain
  //histogram.
  Float_t  LowerMomentumThreshold, UpperMomentumThreshold;
  
  for(Int_t sector=0;sector<MAXSEC;sector++)
    {
      for(Int_t mombin=0; mombin<MAXMOMBINS;mombin++)
	{
	  if(mombin==0)
	    {
	      LowerMomentumThreshold=0.0;
	      UpperMomentumThreshold=100.0;
	    }
	  if(mombin>0 && mombin<=10)
	    {
	      LowerMomentumThreshold=(mombin+1)*50;
	      UpperMomentumThreshold=LowerMomentumThreshold+50;
	    }
	  if(mombin==11)
	    {
	      LowerMomentumThreshold=600;
	      UpperMomentumThreshold=2000.0; //There is no need to take into account leptons faster than this ...
	    }
	  //Build meaningful names and titles for all histograms
	  TString histname_theta("Delta_");
	  histname_theta.Append("theta_sector_");
	  
	  TString histname_phi("Delta_");
	  histname_phi.Append("phi_sector_");
	  
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
  
  //Append all input files to one big chain
  Char_t filename[256];
  Int_t fileindex=0;
  Int_t nFiles=0;
  TChain N("N");
  
  ifstream nTupleList(filelist);
  while(nTupleList.getline(filename,256) &&nFiles<40)
    { 
      N.Add(filename);
      nFiles++;
    }
  
  cout << "All files together have: "<< N.GetEntries()<< " entries!" << endl;
  
  //Variables required for filling the histograms with suitable data
  Float_t f_track_theta;
  Float_t f_track_phi;
  
  Float_t f_deltatheta;
  Float_t f_deltaphi;
  
  Float_t f_mdc_chi2;
  
  Float_t b_corrRingRK, b_corrRingKick, b_ring;
  
  Float_t f_shw_tof, f_tof_tof;
  
  Float_t f_rk_quality;
  
  Float_t b_splinetrack, b_kicktrack, b_rktrack;
  
  Float_t n_sector;
  
  Float_t f_kick_mom_exp, f_rk_mom_exp, f_track_momentum;
  
  N.SetBranchAddress("f_track_theta",&f_track_theta);
  N.SetBranchAddress("f_track_phi",&f_track_phi);
  
  N.SetBranchAddress("f_deltatheta",&f_deltatheta);
  N.SetBranchAddress("f_deltaphi",&f_deltaphi);
  
  N.SetBranchAddress("f_mdc_chi2",&f_mdc_chi2);
  
  N.SetBranchAddress("b_ring",&b_ring);
  N.SetBranchAddress("b_corrRingKick",&b_corrRingKick);
  N.SetBranchAddress("b_corrRingRK",&b_corrRingRK);
  
  N.SetBranchAddress("f_tof_tof",&f_tof_tof);
  N.SetBranchAddress("f_shw_tof",&f_shw_tof);
  
  N.SetBranchAddress("f_rk_quality",&f_rk_quality);
  
  N.SetBranchAddress("b_splinetrack",&b_splinetrack);
  N.SetBranchAddress("b_kicktrack",&b_kicktrack);
  N.SetBranchAddress("b_rktrack",&b_rktrack);
  
  N.SetBranchAddress("n_sector",&n_sector);
  
  //Depending on the type of tracking chosen by the user we use the corresponding
  //momentum value.
  if(!useHighResTracking)
    {
      N.SetBranchAddress("f_kick_mom_exp",&f_track_momentum);
    }
  else
    {
      N.SetBranchAddress("f_rk_mom_exp",&f_track_momentum);
    }
  
  //N.SetBranchAddress("f_kick_mom_exp",&f_kick_mom_exp);
  //N.SetBranchAddress("f_rk_mom_exp",&f_rk_mom_exp);
  
  //for(Int_t i=0;i< 10;i++)
  for(Int_t i=0;i< N.GetEntries();i++)
    {
      N.GetEntry(i);
      if(!(i%100000))
	{
	  cout << "Analyzing Entry: " << i << endl;
	}
      
      if(b_ring<1) //no ring - this track is not interesting for us
	{
	  continue;
	}
      
      if(!useHighResTracking) //Kickplane required
	{
	  //cout << "Checking correlation of Kicktrack and ring " << endl;
	  if(b_kicktrack<1)  //No kicktrack correlated to a ring
	    {
	      continue;	   
	    }	
	}  
      else
	{
	  //cout << "Checking correlation of RK track and ring " << endl;
	  if(b_rktrack<0 || b_plinetrack<0 || f_rk_chisquare>1000 )
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
      if(f_tof_tof<0 && f_shw_tof<0)
	{
	  continue;
	}
      
      //cout << "Checking mombin " << endl;
      Int_t targetmombin=-1;
      if(f_track_momentum<=100)
	{
	  targetmombin=0;
	}
      else if(f_track_momentum>600)
	{
	  targetmombin=11;
	}
      else
	{
	  targetmombin=TMath::Ceil((f_track_momentum-100.0)/50);
	}
      //cout << "Booking momentum: " << f_track_momentum << " to bin: " << targetmombin << endl;
      histarray_theta[n_sector][targetmombin]->Fill(f_deltatheta);
      histarray_phi[n_sector][targetmombin]->Fill(f_deltaphi);
      
      
    }
  
  //Now all histograms are filled - we need to fit them!
  //This is what we care for in essence.
  Float_t meanvalues_theta[6][12];
  Float_t sigmavalues_theta[6][12];
  
  Float_t meanvalues_phi[6][12];
  Float_t sigmavalues_phi[6][12];
  
  
  //Loop over all histograms and fit them
  for(Int_t sector=0;sector<6;sector++)
    {

      for(Int_t mombin=0; mombin<12;mombin++)
	{
	  //This function has to be executed once for the polar angle and once for the aziuthal angle
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
  
  
  for(Int_t sector=0;sector<6;sector++)
    {
      TString phiCanvasName("phiCorrelationSector_");
      phiCanvasName+=sector;
      TCanvas* phiCanvas = new TCanvas(phiCanvasName,phiCanvasName);
      phiCanvas->Divide(3,4);
      
      for(Int_t mombin=0; mombin<12;mombin++)
	{
	  phiCanvas->cd(mombin+1);
	  gStyle->SetOptStat(0);
	  histarray_phi[sector][mombin]->Draw();
	}
      phiCanvasName.Append(".ps");
      phiCanvas->SaveAs(phiCanvasName.Data());
    }
  
  for(Int_t sector=0;sector<6;sector++)
    {
      TString thetaCanvasName("thetaCorrelationSector_");
      thetaCanvasName+=sector;
      TCanvas* thetaCanvas = new TCanvas(thetaCanvasName,thetaCanvasName);
      thetaCanvas->Divide(3,4);
      
      for(Int_t mombin=0; mombin<12;mombin++)
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
      fprintf(meantheta,"Mean:{%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f},\n",
	      meanvalues_theta[sector][0],meanvalues_theta[sector][1],meanvalues_theta[sector][2],
	      meanvalues_theta[sector][3],meanvalues_theta[sector][4],meanvalues_theta[sector][5],
	      meanvalues_theta[sector][6],meanvalues_theta[sector][7],meanvalues_theta[sector][8],
	      meanvalues_theta[sector][9],meanvalues_theta[sector][10],meanvalues_theta[sector][11]);
    
      fprintf(sigmatheta,"Sigma:{%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f},\n",
	      sigmavalues_theta[sector][0],sigmavalues_theta[sector][1],sigmavalues_theta[sector][2],
	      sigmavalues_theta[sector][3],sigmavalues_theta[sector][4],sigmavalues_theta[sector][5],
	      sigmavalues_theta[sector][6],sigmavalues_theta[sector][7],sigmavalues_theta[sector][8],
	      sigmavalues_theta[sector][9],sigmavalues_theta[sector][10],sigmavalues_theta[sector][11]);
      
      fprintf(meanphi,"Mean:{%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f},\n",
	      meanvalues_phi[sector][0],meanvalues_phi[sector][1],meanvalues_phi[sector][2],
	      meanvalues_phi[sector][3],meanvalues_phi[sector][4],meanvalues_phi[sector][5],
	      meanvalues_phi[sector][6],meanvalues_phi[sector][7],meanvalues_phi[sector][8],
	      meanvalues_phi[sector][9],meanvalues_phi[sector][10],meanvalues_phi[sector][11]);
      fprintf(sigmaphi,"Sigma:{%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f},\n",
	      sigmavalues_phi[sector][0],sigmavalues_phi[sector][1],sigmavalues_phi[sector][2],
	      sigmavalues_phi[sector][3],sigmavalues_phi[sector][4],sigmavalues_phi[sector][5],
	      sigmavalues_phi[sector][6],sigmavalues_phi[sector][7],sigmavalues_phi[sector][8],
	      sigmavalues_phi[sector][9],sigmavalues_phi[sector][10],sigmavalues_phi[sector][11]);
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
  if(SUM->GetParameter(2)<SUM->GetParameter(5))
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

