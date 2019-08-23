normalize()
{
  //computeNormalizationBatch("./","/scratch/e12i/hades/dst/nov02/sim/gen2c/PDF_DATA_FILTERED/filelist.txt","nov02simgen2c","LowRes");
  computeNormalizationBatch("./","/scratch/e12i/hades/dst/nov02/sim/gen2c/PDF_DATA_FILTERED/filelist.txt","nov02simgen2c","BestRes",1,0,1,1,0,1);
}
computeNormalizationBatch(Char_t* targetdirectory, Char_t* filelist, Char_t* experiment_ID,Char_t* TrackingContext="LowRes",
			  Int_t res0=0,Int_t res1=0,Int_t res2=0,Int_t res3=0,Int_t res4=0,Int_t res5=0)
{
  Int_t ResolutionInSectors[6];
  ResolutionInSectors[0]=res0;
  ResolutionInSectors[1]=res1;
  ResolutionInSectors[2]=res2;
  ResolutionInSectors[3]=res3;
  ResolutionInSectors[4]=res4;
  ResolutionInSectors[5]=res5;
  
  
  for(Int_t sector =0;sector<6;sector++)
  {
    if(ResolutionInSectors[sector]==0)
      {
	cout << "Using Kick for sector :" << sector << endl;
	computeNormalizationKick(sector, targetdirectory, filelist, experiment_ID, TrackingContext);	  
      }
    else if(ResolutionInSectors[sector]==1)
      {
	cout << "Using Spline for sector :" << sector << endl;
	computeNormalizationSpline(sector, targetdirectory, filelist, experiment_ID, TrackingContext);	  
      }
    else
      {
	cout << "This resolution for sector: " << sector<< " is not valid!: " << ResolutionInSectors[sector] << endl;
      }
  } 
}

computeNormalizationKick(Int_t sector, Char_t* targetdirectory, Char_t* filelist, Char_t* experiment_ID,Char_t* TrackingContext)
{

  cout << "Sector: " << sector << endl;

  /*
  const Int_t nMomentumBins=31;
  Float_t bins[nMomentumBins+1] ={0.0, 20.0, 40.0, 60.0, 80.0, 100.0,
				  120.0, 140.0, 160.0, 180.0, 200.0,
				  250.0, 300.0, 350.0, 400.0, 450.0, 500.0,
				  600.0, 700.0, 800.0, 900.0, 1000.0,
				  1100.0, 1200.0, 1300.0, 1400.0, 1500.0,
				  1600.0, 1700.0, 1800.0, 1900.0 ,2000.0};
  const Int_t nThetaBins=4;
  Float_t thetaBins[nThetaBins+1]={10.0,30.0,50.0,70.0,90.0};
  */
  const Int_t nMomentumBins=25;
  Float_t bins[nMomentumBins+1] ={0.0, 20.0, 40.0, 60.0, 80.0, 100.0,
				  120.0, 140.0, 160.0, 180.0, 200.0,
				  250.0, 300.0, 350.0, 400.0, 450.0, 500.0,
				  600.0, 700.0, 800.0, 1000.0, 1200.0, 1400.0,
				  1600.0, 1800.0, 2000.0};
  const Int_t nThetaBins=2;
  Float_t thetaBins[nThetaBins+1]={10.0,50.0,90.0};

  TString targetDir(targetdirectory);
  
  TNtuple* N=NULL;
  const Int_t maxInputFiles=50;
  const Int_t skipFiles=0;
  TFile* inputFile[maxInputFiles];
  TNtuple* ntup[maxInputFiles];
  Char_t* names[maxInputFiles];
  Int_t nFiles=0;
  
  for(Int_t i=0;i<maxInputFiles;i++)
    {
      inputFile[i]=NULL;
      ntup[i]=NULL;
      names[i]=new Char_t[265];
    }
  cout << "filelist: " << filelist << endl;
  ifstream nTupleList(filelist);
  Char_t name[256];
  Int_t wait;
  Int_t k=0;
  Int_t nSkippedFiles=0;
  
  while(nTupleList.getline(name,265) &&nFiles<maxInputFiles)
    //while(nTupleList.getline(name,265) &&nFiles<2)
    { 
      if(nSkippedFiles<skipFiles) 
	{
	  nSkippedFiles++;
	  continue;
	}
      cout << "opening file: " << name<< endl;
      inputFile[k] = TFile::Open(name);
      ntup[k]=(TNtuple*)inputFile[k]->FindObjectAny("FN");
      sprintf(names[k],"%s",name);
      k++;
      nFiles++;
      
    }
  

  TString histogramName(targetDir);

  histogramName.Append("RichNormalizationHistograms_");
  histogramName.Append(experiment_ID);
  //histogramName.Append("_sector");
  //histogramName+=sector;
  histogramName.Append("_");
  histogramName.Append(TrackingContext);
  histogramName.Append(".root");
  TFile* PID_normalization_file= new TFile(histogramName.Data(), "UPDATE");
  cout<< "writing to " << histogramName.Data()<< endl;
  PID_normalization_file->cd();
  /*
  Char_t nameLep[128];
  Char_t nameProt[128];
  Char_t namePiPlus[128];
  Char_t namePiMinus[128];
  Char_t nameFake[128];

  Char_t nameLepRatio[128];
  Char_t nameProtRatio[128];
  Char_t namePiPlusRatio[128];
  Char_t namePiMinusRatio[128];
  Char_t nameFakeRatio[128];

  sprintf(nameLep,"%s%d","MomDistLepRatio_sector_",sector);	  
  sprintf(nameFake,"%s%d","MomDistFakeRatio_sector_",sector);
  
  sprintf(nameProt,"%s%d","MomDistProtonRatio_sector_",sector);
  sprintf(namePiPlus,"%s%d","MomDistPiPlusRatio_sector_",sector);
  sprintf(namePiMinus,"%s%d","MomDistPiMinusRatio_sector_",sector);


  sprintf(nameLep,"%s%d","MomDistLepRatio_sector_",sector);	  
  sprintf(nameFake,"%s%d","MomDistFakeRatio_sector_",sector);
  
  sprintf(nameProt,"%s%d","MomDistProtonRatio_sector_",sector);
  sprintf(namePiPlus,"%s%d","MomDistPiPlusRatio_sector_",sector);
  sprintf(namePiMinus,"%s%d","MomDistPiMinusRatio_sector_",sector);
  */

  //Momentum distributions of all particles with ring
  TH2D* MomDistProtonWithRing = new TH2D("MomDistProtonWithRing","MomDistProtonWithRing",nMomentumBins,bins, nThetaBins,thetaBins);
  TH2D* MomDistPiPlusWithRing = new TH2D("MomDistPiPlusWithRing","MomDistPiPlusWithRing",nMomentumBins,bins, nThetaBins,thetaBins);
  TH2D* MomDistPiMinusWithRing = new TH2D("MomDistPiMinusWithRing","MomDistPiMinusWithRing",nMomentumBins,bins, nThetaBins,thetaBins);
  TH2D* MomDistLepWithRing = new TH2D("MomDistLepWithRing","MomDistLepWithRing",nMomentumBins,bins, nThetaBins,thetaBins);
  TH2D* MomDistFakeWithRing = new TH2D("MomDistFakeWithRing","MomDistFakeWithRing",nMomentumBins,bins, nThetaBins,thetaBins);
  
  //Momentum distributions of all particles (with and without) ring
  TH2D* MomDistProtonAll = new TH2D("MomDistProtonAll","MomDistProtonAll",nMomentumBins,bins, nThetaBins,thetaBins);
  TH2D* MomDistPiPlusAll = new TH2D("MomDistPiPlusAll","MomDistPiPlusAll",nMomentumBins,bins, nThetaBins,thetaBins);
  TH2D* MomDistPiMinusAll = new TH2D("MomDistPiMinusAll","MomDistPiMinusAll",nMomentumBins,bins, nThetaBins,thetaBins);
  TH2D* MomDistLepAll = new TH2D("MomDistLepAll","MomDistLepAll",nMomentumBins,bins, nThetaBins,thetaBins);
  TH2D* MomDistFakeAll = new TH2D("MomDistFakeAll","MomDistFakeAll",nMomentumBins,bins, nThetaBins,thetaBins);

  
  
  Char_t secchar[16];
  sprintf(secchar,"%s%d","n_sector==",sector);
  
  TCut sectorCut(secchar);
  
  TCut isGoodTrack("f_kick_mom_exp>0.0 && i_geant_corrflag>=72");
  TCut isFakeTrack("f_kick_mom_exp>0.0 && i_geant_corrflag<72");
  
  TCut isLeptonID("(i_geant_pid==3||i_geant_pid==2)");
  TCut isProtonID("i_geant_pid==14");
  TCut isPiPlusID("i_geant_pid==8");
  TCut isPiMinusID("i_geant_pid==9");
  TCut isFakeID("i_geant_pid<0");
  
  TCut hasARing("b_corrRingKick==1");
  TCut hasNoRing("b_corrRingKick!=1");
  //cout << "warning! Check b_CorrRingSpline"<< endl;
  
  TCut isLepton = isLeptonID && isGoodTrack;
  TCut isProton = isProtonID && isGoodTrack;
  TCut isPiPlus = isPiPlusID && isGoodTrack;
  TCut isPiMinus = isPiMinusID && isGoodTrack;
  TCut isFake = isFakeID && isFakeTrack;
  //cout << "warning! Check geant if vs tracktype"<< endl;
  
  TCut isLepWithRing = isLepton && hasARing;
  TCut isLepWithoutRing = isLepton&& hasNoRing;
  
  TCut isProtonWithRing = isProton && hasARing;
  TCut isProtonWithoutRing = isProton && hasNoRing;
  
  TCut isPiPlusWithRing = isPiPlus && hasARing;
  TCut isPiPlusWithoutRing = isPiPlus && hasNoRing;
  
  TCut isPiMinusWithRing = isPiMinus  && hasARing;
  TCut isPiMinusWithoutRing = isPiMinus && hasNoRing;

  TCut isFakeWithRing = isFake && hasARing;
  TCut isFakeWithoutRing = isFake && hasNoRing;

  Int_t wait;
  //cin>> wait;
  //cout <<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
  for(Int_t j=0;j<nFiles;j++)
    {
      if(ntup[j])
	{
	  cout <<names[j] << endl;
	  cout <<"fileindex: " << j << endl;

	  cout << ntup[j]->GetEntries()<<endl;
	  ntup[j]->Draw("f_track_theta:f_kick_mom_exp>>+MomDistLepAll",isLepton && sectorCut,"");
	  ntup[j]->Draw("f_track_theta:f_kick_mom_exp>>+MomDistLepWithRing",isLepWithRing && sectorCut,"");
	  cout << "Leptons done" << endl;
	  
	  ntup[j]->Draw("f_track_theta:f_kick_mom_exp>>+MomDistProtonAll",isProton && sectorCut,"");
	  ntup[j]->Draw("f_track_theta:f_kick_mom_exp>>+MomDistProtonWithRing",isProtonWithRing && sectorCut,"");
	  cout << "Protons done" << endl;

	  ntup[j]->Draw("f_track_theta:f_kick_mom_exp>>+MomDistPiPlusAll",isPiPlus && sectorCut,"");
	  ntup[j]->Draw("f_track_theta:f_kick_mom_exp>>+MomDistPiPlusWithRing",isPiPlusWithRing && sectorCut,"");
	  cout << "PiPlus done" << endl;
	  ntup[j]->Draw("f_track_theta:f_kick_mom_exp>>+MomDistPiMinusAll",isPiMinus && sectorCut,"");
	  ntup[j]->Draw("f_track_theta:f_kick_mom_exp>>+MomDistPiMinusWithRing",isPiMinusWithRing && sectorCut,"");
	  cout << "PiMinus done" << endl;
	  ntup[j]->Draw("f_track_theta:f_kick_mom_exp>>+MomDistFakeAll",isFake && sectorCut,"");
	  ntup[j]->Draw("f_track_theta:f_kick_mom_exp>>+MomDistFakeWithRing",isFakeWithRing && sectorCut,"");
	  cout << "Fakes done" << endl;
	  
	  cout << "Nr. of leptons: " << MomDistLepAll->Integral()<< endl;
	  
	  cout << "Nr. of protons: " << MomDistProtonAll->Integral()<< endl;	  
	  inputFile[j]->Close();
	}
    }
  //cout << "WARNING! CHECK LEP HISTO!! STRANGE axises!!" << endl;
  TString MomDistFakeRatioString("MomDistFakeRatio_sector_");
  MomDistFakeRatioString+=sector;
  TString MomDistLepRatioString("MomDistLepRatio_sector_");
  MomDistLepRatioString+=sector;
  TString MomDistProtonRatioString("MomDistProtonRatio_sector_");
  MomDistProtonRatioString+=sector;
  TString MomDistPiPlusRatioString("MomDistPiPlusRatio_sector_");
  MomDistPiPlusRatioString+=sector;
  TString MomDistPiMinusRatioString("MomDistPiMinusRatio_sector_");
  MomDistPiMinusRatioString+=sector;



  PID_normalization_file->cd();
  
  TH2D* MomDistFakeRatio=MomDistFakeWithRing->Clone();
  MomDistFakeRatio->SetName(MomDistFakeRatioString.Data());
  MomDistFakeRatio->SetTitle(MomDistFakeRatioString.Data());
  MomDistFakeRatio->Divide(MomDistFakeAll);
  
  TH2D* MomDistLepRatio=MomDistLepWithRing->Clone();
  MomDistLepRatio->SetName(MomDistLepRatioString.Data());
  MomDistLepRatio->SetTitle(MomDistLepRatioString.Data());
  MomDistLepRatio->Divide(MomDistLepAll);

  TH2D* MomDistProtonRatio=MomDistProtonWithRing->Clone();
  MomDistProtonRatio->SetName(MomDistProtonRatioString.Data());
  MomDistProtonRatio->SetTitle(MomDistProtonRatioString.Data());
  MomDistProtonRatio->Divide(MomDistProtonAll);

  TH2D* MomDistPiPlusRatio=MomDistPiPlusWithRing->Clone();
  MomDistPiPlusRatio->SetName(MomDistPiPlusRatioString.Data());
  MomDistPiPlusRatio->SetTitle(MomDistPiPlusRatioString.Data());
  MomDistPiPlusRatio->Divide(MomDistPiPlusAll);

  TH2D* MomDistPiMinusRatio=MomDistPiMinusWithRing->Clone();
  MomDistPiMinusRatio->SetName(MomDistPiMinusRatioString.Data());
  MomDistPiMinusRatio->SetTitle(MomDistPiMinusRatioString.Data());
  MomDistPiMinusRatio->Divide(MomDistPiMinusAll);


  
  MomDistFakeRatio->Write();
  MomDistLepRatio->Write();
  MomDistProtonRatio->Write();
  MomDistPiPlusRatio->Write();
  MomDistPiMinusRatio->Write();

  MomDistProtonWithRing->Write();
  MomDistPiPlusWithRing->Write();
  MomDistPiMinusWithRing->Write();
  MomDistLepWithRing->Write();
  MomDistFakeWithRing->Write();

  MomDistProtonAll->Write();
  MomDistPiPlusAll->Write();
  MomDistPiMinusAll->Write();
  MomDistLepAll->Write();
  MomDistFakeAll->Write();

  PID_normalization_file->Write();  
  PID_normalization_file->Close();
}


computeNormalizationSpline(Int_t sector, Char_t* targetdirectory, Char_t* filelist, Char_t* experiment_ID,Char_t* TrackingContext)
{


  cout << "Sector: " << sector << endl;
  /*
  const Int_t nMomentumBins=31;
  Float_t bins[nMomentumBins+1] ={0.0, 20.0, 40.0, 60.0, 80.0, 100.0,
				  120.0, 140.0, 160.0, 180.0, 200.0,
				  250.0, 300.0, 350.0, 400.0, 450.0, 500.0,
				  600.0, 700.0, 800.0, 900.0, 1000.0,
				  1100.0, 1200.0, 1300.0, 1400.0, 1500.0,
				  1600.0, 1700.0, 1800.0, 1900.0 ,2000.0};
  const Int_t nThetaBins=4;
  Float_t thetaBins[nThetaBins+1]={10.0,30.0,50.0,70.0,90.0};
  */

  const Int_t nMomentumBins=25;
  Float_t bins[nMomentumBins+1] ={0.0, 20.0, 40.0, 60.0, 80.0, 100.0,
				  120.0, 140.0, 160.0, 180.0, 200.0,
				  250.0, 300.0, 350.0, 400.0, 450.0, 500.0,
				  600.0, 700.0, 800.0, 1000.0, 1200.0, 1400.0,
				  1600.0, 1800.0, 2000.0};
  const Int_t nThetaBins=2;
  Float_t thetaBins[nThetaBins+1]={10.0,50.0,90.0};

  TString targetDir(targetdirectory);
  
  TNtuple* N=NULL;
  const Int_t maxInputFiles=50;
  const Int_t skipFiles=0;
  TFile* inputFile[maxInputFiles];
  TNtuple* ntup[maxInputFiles];
  Char_t* names[maxInputFiles];
  Int_t nFiles=0;

  for(Int_t i=0;i<maxInputFiles;i++)
    {
      inputFile[i]=NULL;
      ntup[i]=NULL;
      names[i]=new Char_t[265];
    }
  
  ifstream nTupleList(filelist);
  Char_t name[256];
  Int_t wait;
  Int_t k=0;
  Int_t nSkippedFiles=0;
  
  while(nTupleList.getline(name,265) &&nFiles<maxInputFiles)
    { 
      if(nSkippedFiles<skipFiles) 
	{
	  nSkippedFiles++;
	  continue;
	}
      cout << "opening file: " << name<< endl;
      inputFile[k] = TFile::Open(name);
      ntup[k]=(TNtuple*)inputFile[k]->FindObjectAny("FN");
      sprintf(names[k],"%s",name);
      k++;
      nFiles++;

    }
  

  TString histogramName(targetDir);
  histogramName.Append("RichNormalizationHistograms_");
  histogramName.Append(experiment_ID);
  histogramName.Append("_sector");
  histogramName+=sector;
  histogramName.Append("_");
  histogramName.Append(TrackingContext);

  histogramName.Append(".root");
  TFile* PID_normalization_file= new TFile(histogramName.Data(), "UPDATE");
  cout<< "writing to " << histogramName.Data()<< endl;
  PID_normalization_file->cd();
  
  
  //Momentum distributions of all particles with ring
  TH2D* MomDistProtonWithRing = new TH2D("MomDistProtonWithRing","MomDistProtonWithRing",nMomentumBins,bins, nThetaBins,thetaBins);
  TH2D* MomDistPiPlusWithRing = new TH2D("MomDistPiPlusWithRing","MomDistPiPlusWithRing",nMomentumBins,bins, nThetaBins,thetaBins);
  TH2D* MomDistPiMinusWithRing = new TH2D("MomDistPiMinusWithRing","MomDistPiMinusWithRing",nMomentumBins,bins, nThetaBins,thetaBins);
  TH2D* MomDistLepWithRing = new TH2D("MomDistLepWithRing","MomDistLepWithRing",nMomentumBins,bins, nThetaBins,thetaBins);
  TH2D* MomDistFakeWithRing = new TH2D("MomDistFakeWithRing","MomDistFakeWithRing",nMomentumBins,bins, nThetaBins,thetaBins);
  
  
  //Momentum distributions of all particles (with and without) ring
  TH2D* MomDistProtonAll = new TH2D("MomDistProtonAll","MomDistProtonAll",nMomentumBins,bins, nThetaBins,thetaBins);
  TH2D* MomDistPiPlusAll = new TH2D("MomDistPiPlusAll","MomDistPiPlusAll",nMomentumBins,bins, nThetaBins,thetaBins);
  TH2D* MomDistPiMinusAll = new TH2D("MomDistPiMinusAll","MomDistPiMinusAll",nMomentumBins,bins, nThetaBins,thetaBins);
  TH2D* MomDistLepAll = new TH2D("MomDistLepAll","MomDistLepAll",nMomentumBins,bins, nThetaBins,thetaBins);
  TH2D* MomDistFakeAll = new TH2D("MomDistFakeAll","MomDistFakeAll",nMomentumBins,bins, nThetaBins,thetaBins);
  
  
  Char_t secchar[16];
  sprintf(secchar,"%s%d","n_sector==",sector);
  
  TCut sectorCut(secchar);
  
  //TCut isGoodTrack("b_splinetrack==1 && (f_tof_tof>-1 || f_shw_tof>-1) && i_tracktype>-1");
  //TCut isFakeTrack("b_splinetrack==1 && (f_tof_tof>-1 || f_shw_tof>-1) && i_tracktype==-1");
  
  TCut isGoodTrack("f_spline_mom_exp>0.0 &&i_geant_corrflag>=72");
  TCut isFakeTrack("f_spline_mom_exp>0.0 && i_geant_corrflag<72");
  
  TCut isLeptonID("(i_geant_pid==3||i_geant_pid==2)");
  TCut isProtonID("i_geant_pid==14");
  TCut isPiPlusID("i_geant_pid==8");
  TCut isPiMinusID("i_geant_pid==9");
  TCut isFakeID("i_geant_pid<0");
  
  TCut hasARing("b_corrRingSpline==1");
  TCut hasNoRing("b_corrRingSpline!=1");
  //cout << "warning! Check b_CorrRingSpline"<< endl;
  
  TCut isLepton = isLeptonID && isGoodTrack;
  TCut isProton = isProtonID && isGoodTrack;
  TCut isPiPlus = isPiPlusID && isGoodTrack;
  TCut isPiMinus = isPiMinusID && isGoodTrack;
  TCut isFake = isFakeID && isFakeTrack;
  //cout << "warning! Check geant if vs tracktype"<< endl;
  
  TCut isLepWithRing = isLepton && hasARing;
  TCut isLepWithoutRing = isLepton&& hasNoRing;
  
  TCut isProtonWithRing = isProton && hasARing;
  TCut isProtonWithoutRing = isProton && hasNoRing;
  
  TCut isPiPlusWithRing = isPiPlus && hasARing;
  TCut isPiPlusWithoutRing = isPiPlus && hasNoRing;
  
  TCut isPiMinusWithRing = isPiMinus  && hasARing;
  TCut isPiMinusWithoutRing = isPiMinus && hasNoRing;

  TCut isFakeWithRing = isFake && hasARing;
  TCut isFakeWithoutRing = isFake && hasNoRing;

  Int_t wait;
  //cin>> wait;

  for(Int_t j=0;j<nFiles;j++)
    {
      if(ntup[j])
	{
	  cout <<names[j] << endl;
	  cout <<"fileindex: " << j << endl;
	  cout << ntup[j]->GetEntries()<<endl;

	  ntup[j]->Draw("f_track_theta:f_spline_mom_exp>>+MomDistLepAll",isLepton && sectorCut,"");
	  ntup[j]->Draw("f_track_theta:f_spline_mom_exp>>+MomDistLepWithRing",isLepWithRing && sectorCut,"");
	  
	  ntup[j]->Draw("f_track_theta:f_spline_mom_exp>>+MomDistProtonAll",isProton && sectorCut,"");
	  ntup[j]->Draw("f_track_theta:f_spline_mom_exp>>+MomDistProtonWithRing",isProtonWithRing && sectorCut,"");


	  ntup[j]->Draw("f_track_theta:f_spline_mom_exp>>+MomDistPiPlusAll",isPiPlus && sectorCut,"");
	  ntup[j]->Draw("f_track_theta:f_spline_mom_exp>>+MomDistPiPlusWithRing",isPiPlusWithRing && sectorCut,"");
	  
	  ntup[j]->Draw("f_track_theta:f_spline_mom_exp>>+MomDistPiMinusAll",isPiMinus && sectorCut,"");
	  ntup[j]->Draw("f_track_theta:f_spline_mom_exp>>+MomDistPiMinusWithRing",isPiMinusWithRing && sectorCut,"");
	  
	  ntup[j]->Draw("f_track_theta:f_spline_mom_exp>>+MomDistFakeAll",isFake && sectorCut,"");
	  ntup[j]->Draw("f_track_theta:f_spline_mom_exp>>+MomDistFakeWithRing",isFakeWithRing && sectorCut,"");
	  
	  cout << "Nr. of leptons: " << MomDistLepAll->Integral()<< endl;

	  cout << "Nr. of protons: " << MomDistProtonAll->Integral()<< endl;	  
	  inputFile[j]->Close();
	}
    }
  //cout << "WARNING! CHECK LEP HISTO!! STRANGE axises!!" << endl;
  TString MomDistFakeRatioString("MomDistFakeRatio_sector_");
  MomDistFakeRatioString+=sector;
  TString MomDistLepRatioString("MomDistLepRatio_sector_");
  MomDistLepRatioString+=sector;
  TString MomDistProtonRatioString("MomDistProtonRatio_sector_");
  MomDistProtonRatioString+=sector;
  TString MomDistPiPlusRatioString("MomDistPiPlusRatio_sector_");
  MomDistPiPlusRatioString+=sector;
  TString MomDistPiMinusRatioString("MomDistPiMinusRatio_sector_");
  MomDistPiMinusRatioString+=sector;



  PID_normalization_file->cd();
  
  TH2D* MomDistFakeRatio=MomDistFakeWithRing->Clone();
  MomDistFakeRatio->SetName(MomDistFakeRatioString.Data());
  MomDistFakeRatio->SetTitle(MomDistFakeRatioString.Data());
  MomDistFakeRatio->Divide(MomDistFakeAll);
  
  TH2D* MomDistLepRatio=MomDistLepWithRing->Clone();
  MomDistLepRatio->SetName(MomDistLepRatioString.Data());
  MomDistLepRatio->SetTitle(MomDistLepRatioString.Data());
  MomDistLepRatio->Divide(MomDistLepAll);

  TH2D* MomDistProtonRatio=MomDistProtonWithRing->Clone();
  MomDistProtonRatio->SetName(MomDistProtonRatioString.Data());
  MomDistProtonRatio->SetTitle(MomDistProtonRatioString.Data());
  MomDistProtonRatio->Divide(MomDistProtonAll);

  TH2D* MomDistPiPlusRatio=MomDistPiPlusWithRing->Clone();
  MomDistPiPlusRatio->SetName(MomDistPiPlusRatioString.Data());
  MomDistPiPlusRatio->SetTitle(MomDistPiPlusRatioString.Data());
  MomDistPiPlusRatio->Divide(MomDistPiPlusAll);

  TH2D* MomDistPiMinusRatio=MomDistPiMinusWithRing->Clone();
  MomDistPiMinusRatio->SetName(MomDistPiMinusRatioString.Data());
  MomDistPiMinusRatio->SetTitle(MomDistPiMinusRatioString.Data());
  MomDistPiMinusRatio->Divide(MomDistPiMinusAll);


  
  MomDistFakeRatio->Write();
  MomDistLepRatio->Write();
  MomDistProtonRatio->Write();
  MomDistPiPlusRatio->Write();
  MomDistPiMinusRatio->Write();

  MomDistProtonWithRing->Write();
  MomDistPiPlusWithRing->Write();
  MomDistPiMinusWithRing->Write();
  MomDistLepWithRing->Write();
  MomDistFakeWithRing->Write();

  MomDistProtonAll->Write();
  MomDistPiPlusAll->Write();
  MomDistPiMinusAll->Write();
  MomDistLepAll->Write();
  MomDistFakeAll->Write();

  PID_normalization_file->Write();  
  PID_normalization_file->Close();
}

