makeDifferentialEffMatrices(TChain* E,Int_t nentries=0)
{

  //TChain* E = makeChain("E");
  Float_t pi=TMath::Pi();  
  
  
  Float_t lower_theta_limit = 0.0;
  Float_t upper_theta_limit = pi/2;
  Int_t n_theta_bins = 30;
  
  Float_t lower_phi_limit = -pi;
  Float_t upper_phi_limit = pi;
  Int_t n_phi_bins = 90;
  
  Float_t E_isFullyReconstructed;
  Float_t E_isAccepted;
  Float_t E_isCloseToEmbeddedParticle;
  Float_t E_isCloseToOtherParticle;
  Float_t E_isEmbeddedTrack;
  Float_t E_geant_id;
  Float_t E_isIdentified;
  Float_t E_isLegOfPair;
  Float_t E_isLegOfSurvivingPair;
  Float_t E_isRemovedByActivePairCut;
  Float_t E_isRemovedByCPCandidateCutL;
  Float_t E_isRemovedByCPCandidateCutH;
  Float_t E_phi;
  Float_t E_theta;
  Float_t E_geant_phi;
  Float_t E_geant_theta;
  Float_t E_geant_mom;
  Float_t E_rec_mom;
  
  E->SetBranchAddress("E_theta",&E_theta);
  E->SetBranchAddress("E_isFullyReconstructed",&E_isFullyReconstructed);
  E->SetBranchAddress("E_isAccepted",&E_isAccepted);
  E->SetBranchAddress("E_isCloseToEmbeddedParticle",&E_isCloseToEmbeddedParticle);
  E->SetBranchAddress("E_isCloseToOtherParticle",&E_isCloseToOtherParticle);
  E->SetBranchAddress("E_isEmbeddedTrack",&E_isEmbeddedTrack);
  E->SetBranchAddress("E_geant_id",&E_geant_id);
  E->SetBranchAddress("E_isIdentified",&E_isIdentified);
  E->SetBranchAddress("E_isLegOfPair",&E_isLegOfPair);
  E->SetBranchAddress("E_isLegOfSurvivingPair",&E_isLegOfSurvivingPair);
  E->SetBranchAddress("E_isRemovedByActivePairCut",&E_isRemovedByActivePairCut);
  E->SetBranchAddress("E_isRemovedByCPCandidateCutH",&E_isRemovedByCPCandidateCutH);
  E->SetBranchAddress("E_isRemovedByCPCandidateCutL",&E_isRemovedByCPCandidateCutL);
  E->SetBranchAddress("E_phi",&E_phi);
  E->SetBranchAddress("E_theta",&E_theta);
  E->SetBranchAddress("E_geant_phi",&E_geant_phi);
  E->SetBranchAddress("E_geant_theta",&E_geant_theta);
  E->SetBranchAddress("E_geant_mom",&E_geant_mom);
  E->SetBranchAddress("E_rec_mom",&E_rec_mom);
  //E->SetBranchAddress("E_rec_mom",&E_corr_ring);
  
  static const  Int_t nmombins=100;
  //NO CUTS
  TH2F* electronEfficienciesNoCuts[nmombins];
  TH2F* positronEfficienciesNoCuts[nmombins];
  TH2F* electronNumeratorNoCuts[nmombins];
  TH2F* positronNumeratorNoCuts[nmombins];
  TH2F* electronDenominatorNoCuts[nmombins];
  TH2F* positronDenominatorNoCuts[nmombins];
  
  //ALL CUTS
  TH2F* electronEfficienciesAllCuts[nmombins];
  TH2F* positronEfficienciesAllCuts[nmombins];
  TH2F* electronNumeratorAllCuts[nmombins];
  TH2F* positronNumeratorAllCuts[nmombins];
  TH2F* electronDenominatorAllCuts[nmombins];
  TH2F* positronDenominatorAllCuts[nmombins];
  

  

  Char_t histname[256];

  
  for (Int_t mombin=0; mombin<nmombins; mombin++)
    {
      //NO CUTS
      sprintf(histname,"%s%d","electronDenominatorNoCuts_",mombin);
      electronDenominatorNoCuts[mombin]=new TH2F(histname,histname,
						 n_phi_bins,lower_phi_limit,upper_phi_limit,
						 n_theta_bins,lower_theta_limit,upper_theta_limit);
      electronDenominatorNoCuts[mombin]->Sumw2();


      sprintf(histname,"%s%d","electronNumeratorNoCuts_",mombin);
      electronNumeratorNoCuts[mombin]=new TH2F(histname,histname,
					       n_phi_bins,lower_phi_limit,upper_phi_limit,
					       n_theta_bins,lower_theta_limit,upper_theta_limit);
      electronNumeratorNoCuts[mombin]->Sumw2();


      sprintf(histname,"%s%d","electronEfficienciesNoCuts_",mombin);
      electronEfficienciesNoCuts[mombin]=new TH2F(histname,histname,
						  n_phi_bins,lower_phi_limit,upper_phi_limit,
						  n_theta_bins,lower_theta_limit,upper_theta_limit);
      electronEfficienciesNoCuts[mombin]->Sumw2();
	    

      sprintf(histname,"%s%d","positronDenominatorNoCuts_",mombin);
      positronDenominatorNoCuts[mombin]=new TH2F(histname,histname,
						 n_phi_bins,lower_phi_limit,upper_phi_limit,
						 n_theta_bins,lower_theta_limit,upper_theta_limit);
      positronDenominatorNoCuts[mombin]->Sumw2();
      

      sprintf(histname,"%s%d","positronNumeratorNoCuts_",mombin);
      positronNumeratorNoCuts[mombin]=new TH2F(histname,histname,
					       n_phi_bins,lower_phi_limit,upper_phi_limit,
					       n_theta_bins,lower_theta_limit,upper_theta_limit);
      positronNumeratorNoCuts[mombin]->Sumw2();
	    

      sprintf(histname,"%s%d","positronEfficienciesNoCuts_",mombin);
      positronEfficienciesNoCuts[mombin]=new TH2F(histname,histname,
						  n_phi_bins,lower_phi_limit,upper_phi_limit,
						  n_theta_bins,lower_theta_limit,upper_theta_limit);
      positronEfficienciesNoCuts[mombin]->Sumw2();


      //ALL CUTS
      sprintf(histname,"%s%d","electronDenominatorAllCuts_",mombin);
      electronDenominatorAllCuts[mombin]=new TH2F(histname,histname,
						 n_phi_bins,lower_phi_limit,upper_phi_limit,
						 n_theta_bins,lower_theta_limit,upper_theta_limit);
      electronDenominatorAllCuts[mombin]->Sumw2();


      sprintf(histname,"%s%d","electronNumeratorAllCuts_",mombin);
      electronNumeratorAllCuts[mombin]=new TH2F(histname,histname,
					       n_phi_bins,lower_phi_limit,upper_phi_limit,
					       n_theta_bins,lower_theta_limit,upper_theta_limit);
      electronNumeratorAllCuts[mombin]->Sumw2();


      sprintf(histname,"%s%d","electronEfficienciesAllCuts_",mombin);
      electronEfficienciesAllCuts[mombin]=new TH2F(histname,histname,
						  n_phi_bins,lower_phi_limit,upper_phi_limit,
						  n_theta_bins,lower_theta_limit,upper_theta_limit);
      electronEfficienciesAllCuts[mombin]->Sumw2();
	    

      sprintf(histname,"%s%d","positronDenominatorAllCuts_",mombin);
      positronDenominatorAllCuts[mombin]=new TH2F(histname,histname,
						 n_phi_bins,lower_phi_limit,upper_phi_limit,
						 n_theta_bins,lower_theta_limit,upper_theta_limit);
      positronDenominatorAllCuts[mombin]->Sumw2();
      

      sprintf(histname,"%s%d","positronNumeratorAllCuts_",mombin);
      positronNumeratorAllCuts[mombin]=new TH2F(histname,histname,
					       n_phi_bins,lower_phi_limit,upper_phi_limit,
					       n_theta_bins,lower_theta_limit,upper_theta_limit);
      positronNumeratorAllCuts[mombin]->Sumw2();
	    

      sprintf(histname,"%s%d","positronEfficienciesAllCuts_",mombin);
      positronEfficienciesAllCuts[mombin]=new TH2F(histname,histname,
						  n_phi_bins,lower_phi_limit,upper_phi_limit,
						  n_theta_bins,lower_theta_limit,upper_theta_limit);
      positronEfficienciesAllCuts[mombin]->Sumw2();



    }
  
  //Loop over all entries in the ntuple
  

  Long_t maxentries=0;
  if(nentries)
    {
      maxentries = nentries;
    }
  else
    {
      maxentries = E->GetEntries();
    }

  Int_t mombin = 0;
  Int_t mombingeant = 0;
  //for(Int_t entry=0;entry<maxentries;entry++)
  Int_t entry=0;
  Int_t nogood[nmombins];
  Int_t good[nmombins];
  for(Int_t g=0;g<nmombins;g++)
    {
      nogood[g]=0;
      good[g]=0;
    }
  
  while(E->GetEntry(entry) && entry<maxentries)
    {
      if(!(entry%100000)) {
	cout << "Processing entry " << entry << " of " << maxentries << "..." << endl;
      }

      if(!(entry%300000)) {	
	for(Int_t g=0;g<nmombins;g++)
	  {
	    if(nogood[g]>0)
	      {
		Float_t Ratio = ((Float_t)nogood[g])/(nogood[g]+good[g]);
		cout << "MomBin: " << g << ". Ratio: " << Ratio << "." << endl;
		cout << "good[" << g << "]: " << good[g] << endl;;
		cout << "nogood[" << g << "]: " << nogood[g] << endl;;
	      }
	  }
      }

      if(E_geant_id!=2 && E_geant_id!=3)
	{
	  entry++;
	  continue;
	}

      Bool_t b_Acc=(Bool_t)(E_isAccepted);
      Bool_t b_Emb=(Bool_t)(E_isEmbeddedTrack);
      
      if(!(b_Acc && b_Emb))
	{
	  entry++;
	  continue;
	}

      Bool_t b_ClE=(Bool_t)(E_isCloseToEmbeddedParticle);
      Bool_t b_ClO=(Bool_t)(E_isCloseToOtherParticle);
      
      /*
      if(b_ClE && b_ClO)
	{
	  entry++;
	  continue;
	}
      */
      entry++;

      Bool_t b_Ide=(Bool_t)(E_isIdentified);
      Bool_t b_Rem=(Bool_t)(E_isRemovedByActivePairCut);
      Bool_t b_Rec=(Bool_t)(E_isFullyReconstructed);
      Bool_t b_CpRemLep=(Bool_t)(E_isRemovedByCPCandidateCutL);
      Bool_t b_CpRemHad=(Bool_t)(E_isRemovedByCPCandidateCutH);
      Bool_t b_Sur=(Bool_t)(E_isLegOfSurvivingPair);
      Bool_t b_Leg=(Bool_t)(E_isLegOfPair);


      
      mombingeant = getMomentumBin(E_geant_mom,nmombins);
      //mombin = getMomentumBin(E_rec_mom,nmombins);

      //cout << "Passed 1" << endl;

      //Transform to radians
      /*      if(E_geant_phi>180)
	{
	  E_geant_phi=(E_geant_phi-360)*TMath::DegToRad();
	}
      else
	{
	  E_geant_phi=E_phi*TMath::DegToRad();
	}
      E_geant_theta=E_geant_theta*TMath::DegToRad();      
      cout << "E_geant_phi: " << E_geant_phi << endl;
      cout << "E_geant_theta: " << E_geant_theta << endl;*/

      //cout << "Passed 2" << endl;
      //NO CUTS

      
      //if(!b_ClE)
      if(1)
	{
	  
	  if(E_geant_id==2)
	    {
	      positronDenominatorNoCuts[mombingeant]->Fill(E_geant_phi,E_geant_theta);
	    }
	  if(E_geant_id==3)
	    {
	      //cout << "Filling denmoinator mombin: " << mombin << endl;
	      electronDenominatorNoCuts[mombingeant]->Fill(E_geant_phi,E_geant_theta);
	    }
	  //cout << "Passed 3" << endl;
	  //Condition for no-pair-cut-numerator  
	  if(b_Ide)
	    {
	      
	      if(E_geant_id==2)
		{
		  positronNumeratorNoCuts[mombingeant]->Fill(E_geant_phi,E_geant_theta);
		}
	      if(E_geant_id==3)
		{
		  //cout << "Filling numerator mombin: " << mombin << endl;
		  //cout <<  E_phi << " " << E_theta << endl;
		  electronNumeratorNoCuts[mombingeant]->Fill(E_geant_phi,E_geant_theta);
		  //cout << electronNumeratorNoCuts[mombingeant]->Integral() << endl;
		}


	    }
	}
      

      //ALL CUTS
      if(!b_ClE)
      //if(1)
	{
	  //cout << "hhhheeeerreee" << endl;
	  if(E_geant_id==2)
	    {
	      positronDenominatorAllCuts[mombingeant]->Fill(E_geant_phi,E_geant_theta);
	    }
	  if(E_geant_id==3)
	    {
	      //cout << "Filling denmoinator mombin: " << mombin << endl;
	      electronDenominatorAllCuts[mombingeant]->Fill(E_geant_phi,E_geant_theta);
	    }

	  //Condition for all-pair-cut-numerator  
	  Bool_t before = b_CpRemLep;
	  //if((b_Ide && !b_Leg && !b_CpRemLep) || (b_Sur && !b_Rem && !b_CpRemLep))
	  if(!b_CpRemLep && b_Rec && b_Ide && !b_Rem && (b_Sur || !b_Leg))
	    {
	      
	      if(!b_CpRemLep)
	      {
		//cout << "b_CpRemLep1:" <<b_CpRemLep << endl;
		good[mombingeant]++;
	      }
	      
	      if(b_CpRemLep)
		{
		  nogood[mombingeant]++;
		  //cout << "b_CpRemLep2:" <<b_CpRemLep << endl;
		}

	      if(b_CpRemLep!=before)
		{
		  cout << "flip" << endl;
		}

	      if(E_geant_id==2)
		{
		  positronNumeratorAllCuts[mombingeant]->Fill(E_geant_phi,E_geant_theta);
		}
	      if(E_geant_id==3)
		{
		  //cout << "Filling numerator mombin: " << mombin << endl;
		  //cout <<  E_phi << " " << E_theta << endl;
		  electronNumeratorAllCuts[mombingeant]->Fill(E_geant_phi,E_geant_theta);
		  //cout << electronNumeratorAllCuts[mombingeant]->Integral() << endl;
		}


	    }
	}
      


    }

  for(Int_t i=0;i<nmombins;i++)
    {
      //NO CUTS
      electronEfficienciesNoCuts[i]->Divide(electronNumeratorNoCuts[i],electronDenominatorNoCuts[i]);
      positronEfficienciesNoCuts[i]->Divide(positronNumeratorNoCuts[i],positronDenominatorNoCuts[i]);

      //ALL CUTS
      electronEfficienciesAllCuts[i]->Divide(electronNumeratorAllCuts[i],electronDenominatorAllCuts[i]);
      positronEfficienciesAllCuts[i]->Divide(positronNumeratorAllCuts[i],positronDenominatorAllCuts[i]);

    }
  cout << "Done filling" << endl;

  //NO CUTS
  TH3F* positronEfficiencyNoCut3D = new TH3F("positronEfficiencyNoCut3D",
					      "positronEfficiencyNoCut3D",
					      90,-pi,pi,30,0,pi/2,nmombins,0,2000);

  TH3F* electronEfficiencyNoCut3D = new TH3F("electronEfficiencyNoCut3D",
					      "electronEfficiencyNoCut3D",
					      90,-pi,pi,30,0,pi/2,nmombins,0,2000);

  //ALL CUTS
  TH3F* positronEfficiencyAllCut3D = new TH3F("positronEfficiencyAllCut3D",
					      "positronEfficiencyAllCut3D",
					      90,-pi,pi,30,0,pi/2,nmombins,0,2000);

  TH3F* electronEfficiencyAllCut3D = new TH3F("electronEfficiencyAllCut3D",
					      "electronEfficiencyAllCut3D",
					      90,-pi,pi,30,0,pi/2,nmombins,0,2000);



  //Join 2D matrices into 3D matrices
  for(Int_t i=0;i<nmombins;i++){          //Loop over momentum
    for(Int_t j = 1;j<31;j++){      //Loop over theta
      for(Int_t k = 1;k<91;k++){    //Loop over phi
	//NO CUTS
	positronEfficiencyNoCut3D->SetBinContent(k,j,i+1,
						 positronEfficienciesNoCuts[i]->GetBinContent(k,j));
	electronEfficiencyNoCut3D->SetBinContent(k,j,i+1,
						 electronEfficienciesNoCuts[i]->GetBinContent(k,j));
	positronEfficiencyNoCut3D->SetBinError(k,j,i+1,
					       positronEfficienciesNoCuts[i]->GetBinError(k,j));
	electronEfficiencyNoCut3D->SetBinError(k,j,i+1,
					       electronEfficienciesNoCuts[i]->GetBinError(k,j));

	//ALL CUTS
	positronEfficiencyAllCut3D->SetBinContent(k,j,i+1,
						 positronEfficienciesAllCuts[i]->GetBinContent(k,j));
	electronEfficiencyAllCut3D->SetBinContent(k,j,i+1,
						 electronEfficienciesAllCuts[i]->GetBinContent(k,j));
	positronEfficiencyAllCut3D->SetBinError(k,j,i+1,
					       positronEfficienciesAllCuts[i]->GetBinError(k,j));
	electronEfficiencyAllCut3D->SetBinError(k,j,i+1,
					       electronEfficienciesAllCuts[i]->GetBinError(k,j));


      }
    }
  }
  
  
  cout << "Done joining" << endl;  

  TFile* file = TFile::Open("matrixTassiloHighResolutionNewAcceptanceFiltering.root","recreate");
  for(Int_t i=0;i<nmombins;i++)
    {
      //NO CUT
      electronEfficienciesNoCuts[i]->Write();
      electronNumeratorNoCuts[i]->Write();
      electronDenominatorNoCuts[i]->Write();
      positronEfficienciesNoCuts[i]->Write();
      positronNumeratorNoCuts[i]->Write();
      positronDenominatorNoCuts[i]->Write();

      //ALL CUT
      electronEfficienciesAllCuts[i]->Write();
      electronNumeratorAllCuts[i]->Write();
      electronDenominatorAllCuts[i]->Write();
      positronEfficienciesAllCuts[i]->Write();
      positronNumeratorAllCuts[i]->Write();
      positronDenominatorAllCuts[i]->Write();


    }
  //NO CUT
  electronEfficiencyNoCut3D->Write();
  positronEfficiencyNoCut3D->Write();

  //ALL CUTS
  electronEfficiencyAllCut3D->Write();
  positronEfficiencyAllCut3D->Write();
  file->Close();

  return;
  
      
}


Int_t getMomentumBin(Float_t value,Int_t nmombins)
{ 
  Int_t retval=TMath::Floor(value/20.);
  if(retval >= nmombins)
    {
      retval=nmombins-1;
    }
  return retval;
}
