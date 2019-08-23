//Find out whether the application of RK-shifts to the segment direction helps to improve
//the correlation between MDC and Shower. Plots a 1D histogram of deltaX and deltaY before and
//after the correction is applied.
compareCorrelationWidth(Int_t system, Bool_t useTrueParticlesOnly, Int_t nentries=900000000)
{
  TChain* N = makeChain("N");

  TH1F* deviationX_rk[6];
  TH1F* deviationY_rk[6];
  TH1F* deviationX_mdc[6];
  TH1F* deviationY_mdc[6];
  
  TString histname;
  TString DrawString;


  TString systemcond("n_system==");
  systemcond+=system;
  TCut SystemCut(systemcond);

  TString sectorcond;

  
  TCut TrackCut;
  TCut SectorCut;

  if(useTrueParticlesOnly)
    {
      TrackCut="b_rktrack &&f_rk_quality<1000 && (i_geant_corrflag==74 || i_geant_corrflag==78)";
    }
  else
    {
      TrackCut="b_rktrack &&f_rk_quality<1000";
    }


  
  for(Int_t sector=0;sector<6; sector++)
    {
      histname = "deviationX_rk_s";
      histname+=sector;
      deviationX_rk[sector]= new TH1F(histname.Data(),histname.Data(),200,-100,100);

      histname = "deviationY_rk_s";
      histname+=sector;
      deviationY_rk[sector]= new TH1F(histname.Data(),histname.Data(),200,-100,100);

      histname = "deviationX_mdc_s";
      histname+=sector;
      deviationX_mdc[sector]= new TH1F(histname.Data(),histname.Data(),200,-100,100);

      histname = "deviationY_mdc_s";
      histname+=sector;
      deviationY_mdc[sector]= new TH1F(histname.Data(),histname.Data(),200,-100,100);
    }

  for(Int_t sector=0;sector<6; sector++)
    {
      sectorcond="n_sector==";
      sectorcond+=sector;
      SectorCut=sectorcond;


      DrawString="-f_meta_rk_dx>>deviationX_rk_s";
      DrawString+=sector;
      N->Draw(DrawString.Data(),TrackCut && SectorCut && SystemCut,"",nentries);

      DrawString="-f_meta_rk_dy>>deviationY_rk_s";
      DrawString+=sector;
      N->Draw(DrawString.Data(),TrackCut && SectorCut && SystemCut,"",nentries);

      DrawString="f_meta_mdc_dx>>deviationX_mdc_s";
      DrawString+=sector;
      N->Draw(DrawString.Data(),TrackCut && SectorCut && SystemCut,"",nentries);

      DrawString="f_meta_mdc_dy>>deviationY_mdc_s";
      DrawString+=sector;
      N->Draw(DrawString.Data(),TrackCut && SectorCut && SystemCut,"",nentries);

    }

  TCanvas* Delta_x = new TCanvas("Delta_x","Delta_x");
  Delta_x->Divide(2,3);
  TCanvas* Delta_y = new TCanvas("Delta_y","Delta_y");
  Delta_y->Divide(2,3);


  for(Int_t sector=0;sector<6; sector++)
    {
      Delta_x->cd(sector+1);
      deviationX_rk[sector]->SetLineColor(2);
      deviationX_rk[sector]->Draw();
      deviationX_mdc[sector]->Draw("same");

      Delta_y->cd(sector+1);
      deviationY_rk[sector]->SetLineColor(2);
      deviationY_rk[sector]->Draw();
      deviationY_mdc[sector]->Draw("same");
    }

  if(useTrueParticlesOnly)
    {
      TString NameBaseX = "Delta_x_true_particles_system_";
      TString NameBaseY = "Delta_y_true_particles_system_";
      TString FileNameX = NameBaseX+system+".ps";
      TString FileNameY = NameBaseY+system+".ps";

      Delta_x->SaveAs(FileNameX.Data());
      Delta_y->SaveAs(FileNameY.Data());

      FileNameX=NameBaseX+system+".root";
      FileNameY=NameBaseY+system+".root";
      Delta_x->SaveAs(FileNameX.Data());
      Delta_y->SaveAs(FileNameY.Data());

    }
  else
    {
      TString NameBaseX = "Delta_x_all_particles_system_";
      TString NameBaseY = "Delta_y_all_particles_system_";
      TString FileNameX = NameBaseX+system+".ps";
      TString FileNameY = NameBaseY+system+".ps";

      Delta_x->SaveAs(FileNameX.Data());
      Delta_y->SaveAs(FileNameY.Data());

      FileNameX=NameBaseX+system+".root";
      FileNameY=NameBaseY+system+".root";
      Delta_x->SaveAs(FileNameX.Data());
      Delta_y->SaveAs(FileNameY.Data());
    }
}


//This function plots the momentum dependence of the correlation width for different species.
computeCorrelationWidthVsMomentum(Int_t system, Int_t species, Bool_t useTrueParticlesOnly, Int_t nentries=900000000)
{
  TChain* N = makeChain("N");

  TH2F* deviationX_rk[6];
  TH2F* deviationY_rk[6];
  TH2F* deviationX_mdc[6];
  TH2F* deviationY_mdc[6];
  
  TString histname;
  TString DrawString;

  TString SpeciesString("");
  if(species==2 || species==3)
    {
      //SpeciesString="(i_geant_pid==2 || i_geant_pid==3)";
      SpeciesString="n_rich_patmatrix>400 && f_rk_beta>0.85";
    }
  
  if(species==8 || species==9)
    {
      //SpeciesString="(i_geant_pid==8 || i_geant_pid==9)";
      SpeciesString="!b_ring && n_charge_rk<0";
    }
  if(species==14)
    {
      //SpeciesString="i_geant_pid==14";
      SpeciesString="!b_ring && n_charge_rk>0 && f_rk_beta<0.8";
    }
  
  TCut SpeciesCut(SpeciesString);

  TString systemcond("n_system==");
  systemcond+=system;
  TCut SystemCut(systemcond);

  TString sectorcond;
  
  TCut TrackCut;
  if(useTrueParticlesOnly)
    {
      TrackCut = "b_rktrack &&f_rk_quality<1000 && (i_geant_corrflag==74 || i_geant_corrflag==78)";
    }
  else
    {
      TCut TrackCut("b_rktrack &&f_rk_quality<1000");
    }

  TCut SectorCut("");
  
  for(Int_t sector=0;sector<6; sector++)
    {
      histname = "deviationX_rk_s";
      histname+=sector;
      deviationX_rk[sector]= new TH2F(histname.Data(),histname.Data(),100,0,2000,200,-100,100);

      histname = "deviationY_rk_s";
      histname+=sector;
      deviationY_rk[sector]= new TH2F(histname.Data(),histname.Data(),100,0,2000,200,-100,100);

      histname = "deviationX_mdc_s";
      histname+=sector;
      deviationX_mdc[sector]= new TH2F(histname.Data(),histname.Data(),100,0,2000,200,-100,100);

      histname = "deviationY_mdc_s";
      histname+=sector;
      deviationY_mdc[sector]= new TH2F(histname.Data(),histname.Data(),100,0,2000,200,-100,100);
    }

  for(Int_t sector=0;sector<6; sector++)
    {
      sectorcond="n_sector==";
      sectorcond+=sector;
      SectorCut=sectorcond;

      DrawString="f_meta_rk_dx:f_rk_mom_exp>>deviationX_rk_s";
      DrawString+=sector;
      N->Draw(DrawString.Data(),TrackCut && SectorCut && SystemCut && SpeciesCut,"",nentries);

      DrawString="f_meta_rk_dy:f_rk_mom_exp>>deviationY_rk_s";
      DrawString+=sector;
      N->Draw(DrawString.Data(),TrackCut && SectorCut && SystemCut && SpeciesCut,"",nentries);
    }

  TCanvas* Delta_x = new TCanvas("Delta_x","Delta_x");
  Delta_x->Divide(2,3);
  TCanvas* Delta_y = new TCanvas("Delta_y","Delta_y");
  Delta_y->Divide(2,3);


  for(Int_t sector=0;sector<6; sector++)
    {
      Delta_x->cd(sector+1);
      deviationX_rk[sector]->Draw("colz");

      Delta_y->cd(sector+1);
      deviationY_rk[sector]->Draw("colz");
    }

  if(useTrueParticlesOnly)
    {
      TString NameBaseX="Delta_x_vs_p_trueID";
      TString NameBaseY="Delta_y_vs_p_trueID";
      TString FileNameX=NameBaseX+species+"_system_"+system+".ps";
      TString FileNameY=NameBaseY+species+"_system_"+system+".ps";
      Delta_x->SaveAs(FileNameX.Data());
      Delta_y->SaveAs(FileNameY.Data());
      
      FileNameX=NameBaseX+species+"_system_"+system+".root";
      FileNameY=NameBaseY+species+"_system_"+system+".root";
      Delta_x->SaveAs(FileNameX.Data());
      Delta_y->SaveAs(FileNameY.Data());
    }
  else
    {
      TString NameBaseX="Delta_x_vs_p_reconstructedID";
      TString NameBaseY="Delta_y_vs_p_reconstructedID";
      TString FileNameX=NameBaseX+species+"_system_"+system+".ps";
      TString FileNameY=NameBaseY+species+"_system_"+system+".ps";
      Delta_x->SaveAs(FileNameX.Data());
      Delta_y->SaveAs(FileNameY.Data());
      
      FileNameX=NameBaseX+species+"_system_"+system+".root";
      FileNameY=NameBaseY+species+"_system_"+system+".root";
      Delta_x->SaveAs(FileNameX.Data());
      Delta_y->SaveAs(FileNameY.Data());

    }
}



//This function plots the dependence of the correlation width upon the local x-coordinate for different species.
computeCorrelationWidthVsLocalX(Int_t system, Int_t species, Bool_t useTrueParticlesOnly, Int_t nentries=900000000)
{
  TChain* N = makeChain("N");

  TH2F* deviationX_rk[6];
  TH2F* deviationY_rk[6];
  TH2F* deviationX_mdc[6];
  TH2F* deviationY_mdc[6];
  
  TString histname;
  TString DrawString;

  TString SpeciesString("");
  if(species==2 || species==3)
    {
      //SpeciesString="(i_geant_pid==2 || i_geant_pid==3)";
      SpeciesString="n_rich_patmatrix>400 && f_rk_beta>0.85";
    }
  
  if(species==8 || species==9)
    {
      //SpeciesString="(i_geant_pid==8 || i_geant_pid==9)";
      SpeciesString="!b_ring && n_charge_rk<0";
    }
  if(species==14)
    {
      //SpeciesString="i_geant_pid==14";
      SpeciesString="!b_ring && n_charge_rk>0 && f_rk_beta<0.8";
    }
  
  TCut SpeciesCut(SpeciesString);

  TString systemcond("n_system==");
  systemcond+=system;
  TCut SystemCut(systemcond);

  TString sectorcond;
  
  TCut TrackCut;
  if(useTrueParticlesOnly)
    {
      TrackCut = "b_rktrack &&f_rk_quality<1000 && (i_geant_corrflag==74 || i_geant_corrflag==78)";
    }
  else
    {
      TCut TrackCut("b_rktrack &&f_rk_quality<1000");
    }

  TCut SectorCut("");
  
  for(Int_t sector=0;sector<6; sector++)
    {
      histname = "deviationX_rk_s";
      histname+=sector;
      deviationX_rk[sector]= new TH2F(histname.Data(),histname.Data(),200,-1000,1000,200,-100,100);

      histname = "deviationY_rk_s";
      histname+=sector;
      deviationY_rk[sector]= new TH2F(histname.Data(),histname.Data(),200,-1000,1000,200,-100,100);

      histname = "deviationX_mdc_s";
      histname+=sector;
      deviationX_mdc[sector]= new TH2F(histname.Data(),histname.Data(),200,1000,1000,200,-100,100);

      histname = "deviationY_mdc_s";
      histname+=sector;
      deviationY_mdc[sector]= new TH2F(histname.Data(),histname.Data(),200,-1000,1000,200,-100,100);
    }

  for(Int_t sector=0;sector<6; sector++)
    {
      sectorcond="n_sector==";
      sectorcond+=sector;
      SectorCut=sectorcond;

      DrawString="f_meta_rk_dx:f_meta_local_x>>deviationX_rk_s";
      DrawString+=sector;
      N->Draw(DrawString.Data(),TrackCut && SectorCut && SystemCut,"",nentries);

      DrawString="f_meta_rk_dy:f_meta_local_x>>deviationY_rk_s";
      DrawString+=sector;
      N->Draw(DrawString.Data(),TrackCut && SectorCut && SystemCut,"",nentries);
    }

  TCanvas* Delta_x = new TCanvas("Delta_x","Delta_x");
  Delta_x->Divide(2,3);
  TCanvas* Delta_y = new TCanvas("Delta_y","Delta_y");
  Delta_y->Divide(2,3);


  for(Int_t sector=0;sector<6; sector++)
    {
      Delta_x->cd(sector+1);
      deviationX_rk[sector]->Draw("colz");

      Delta_y->cd(sector+1);
      deviationY_rk[sector]->Draw("colz");
    }

  if(useTrueParticlesOnly)
    {
      TString NameBaseX="Delta_x_vs_locx_trueID";
      TString NameBaseY="Delta_y_vs_locx_trueID";
      TString FileNameX=NameBaseX+species+"_system_"+system+".ps";
      TString FileNameY=NameBaseY+species+"_system_"+system+".ps";
      Delta_x->SaveAs(FileNameX.Data());
      Delta_y->SaveAs(FileNameY.Data());
      
      FileNameX=NameBaseX+species+"_system_"+system+".root";
      FileNameY=NameBaseY+species+"_system_"+system+".root";
      Delta_x->SaveAs(FileNameX.Data());
      Delta_y->SaveAs(FileNameY.Data());
    }
  else
    {
      TString NameBaseX="Delta_x_vs_locx_reconstructedID";
      TString NameBaseY="Delta_y_vs_locx_reconstructedID";
      TString FileNameX=NameBaseX+species+"_system_"+system+".ps";
      TString FileNameY=NameBaseY+species+"_system_"+system+".ps";
      Delta_x->SaveAs(FileNameX.Data());
      Delta_y->SaveAs(FileNameY.Data());
      
      FileNameX=NameBaseX+species+"_system_"+system+".root";
      FileNameY=NameBaseY+species+"_system_"+system+".root";
      Delta_x->SaveAs(FileNameX.Data());
      Delta_y->SaveAs(FileNameY.Data());

    }
}


//This function plots the dependence of the correlation width upon the local y-coordinate for different species.
computeCorrelationWidthVsLocalY(Int_t system, Int_t species, Bool_t useTrueParticlesOnly, Int_t nentries=900000000)
{
  TChain* N = makeChain("N");

  TH2F* deviationX_rk[6];
  TH2F* deviationY_rk[6];
  TH2F* deviationX_mdc[6];
  TH2F* deviationY_mdc[6];
  
  TString histname;
  TString DrawString;

  TString SpeciesString("");
  if(species==2 || species==3)
    {
      //SpeciesString="(i_geant_pid==2 || i_geant_pid==3)";
      SpeciesString="n_rich_patmatrix>400 && f_rk_beta>0.85";
    }
  
  if(species==8 || species==9)
    {
      //SpeciesString="(i_geant_pid==8 || i_geant_pid==9)";
      SpeciesString="!b_ring && n_charge_rk<0";
    }
  if(species==14)
    {
      //SpeciesString="i_geant_pid==14";
      SpeciesString="!b_ring && n_charge_rk>0 && f_rk_beta<0.8";
    }
  
  TCut SpeciesCut(SpeciesString);

  TString systemcond("n_system==");
  systemcond+=system;
  TCut SystemCut(systemcond);

  TString sectorcond;
  
  TCut TrackCut;
  if(useTrueParticlesOnly)
    {
      TrackCut = "b_rktrack &&f_rk_quality<1000 && (i_geant_corrflag==74 || i_geant_corrflag==78)";
    }
  else
    {
      TCut TrackCut("b_rktrack &&f_rk_quality<1000");
    }

  TCut SectorCut("");
  
  for(Int_t sector=0;sector<6; sector++)
    {
      histname = "deviationX_rk_s";
      histname+=sector;
      deviationX_rk[sector]= new TH2F(histname.Data(),histname.Data(),200,-1000,1000,200,-100,100);

      histname = "deviationY_rk_s";
      histname+=sector;
      deviationY_rk[sector]= new TH2F(histname.Data(),histname.Data(),200,-1000,1000,200,-100,100);

      histname = "deviationX_mdc_s";
      histname+=sector;
      deviationX_mdc[sector]= new TH2F(histname.Data(),histname.Data(),200,1000,1000,200,-100,100);

      histname = "deviationY_mdc_s";
      histname+=sector;
      deviationY_mdc[sector]= new TH2F(histname.Data(),histname.Data(),200,-1000,1000,200,-100,100);
    }

  for(Int_t sector=0;sector<6; sector++)
    {
      sectorcond="n_sector==";
      sectorcond+=sector;
      SectorCut=sectorcond;

      DrawString="f_meta_rk_dx:f_meta_local_y>>deviationX_rk_s";
      DrawString+=sector;
      N->Draw(DrawString.Data(),TrackCut && SectorCut && SystemCut,"",nentries);

      DrawString="f_meta_rk_dy:f_meta_local_y>>deviationY_rk_s";
      DrawString+=sector;
      N->Draw(DrawString.Data(),TrackCut && SectorCut && SystemCut,"",nentries);
    }

  TCanvas* Delta_x = new TCanvas("Delta_x","Delta_x");
  Delta_x->Divide(2,3);
  TCanvas* Delta_y = new TCanvas("Delta_y","Delta_y");
  Delta_y->Divide(2,3);


  for(Int_t sector=0;sector<6; sector++)
    {
      Delta_x->cd(sector+1);
      deviationX_rk[sector]->Draw("colz");

      Delta_y->cd(sector+1);
      deviationY_rk[sector]->Draw("colz");
    }

  if(useTrueParticlesOnly)
    {
      TString NameBaseX="Delta_x_vs_locy_trueID";
      TString NameBaseY="Delta_y_vs_locy_trueID";
      TString FileNameX=NameBaseX+species+"_system_"+system+".ps";
      TString FileNameY=NameBaseY+species+"_system_"+system+".ps";
      Delta_x->SaveAs(FileNameX.Data());
      Delta_y->SaveAs(FileNameY.Data());
      
      FileNameX=NameBaseX+species+"_system_"+system+".root";
      FileNameY=NameBaseY+species+"_system_"+system+".root";
      Delta_x->SaveAs(FileNameX.Data());
      Delta_y->SaveAs(FileNameY.Data());
    }
  else
    {
      TString NameBaseX="Delta_x_vs_locy_reconstructedID";
      TString NameBaseY="Delta_y_vs_locy_reconstructedID";
      TString FileNameX=NameBaseX+species+"_system_"+system+".ps";
      TString FileNameY=NameBaseY+species+"_system_"+system+".ps";
      Delta_x->SaveAs(FileNameX.Data());
      Delta_y->SaveAs(FileNameY.Data());
      
      FileNameX=NameBaseX+species+"_system_"+system+".root";
      FileNameY=NameBaseY+species+"_system_"+system+".root";
      Delta_x->SaveAs(FileNameX.Data());
      Delta_y->SaveAs(FileNameY.Data());

    }
}

//This function plots the dependence of the correlation width upon the polar for different species.
computeCorrelationWidthVsRKTheta(Int_t system, Int_t species, Bool_t useTrueParticlesOnly, Int_t nentries=900000000)
{
  TChain* N = makeChain("N");

  TH2F* deviationX_rk[6];
  TH2F* deviationY_rk[6];
  TH2F* deviationX_mdc[6];
  TH2F* deviationY_mdc[6];
  
  TString histname;
  TString DrawString;

  TString SpeciesString("");
  if(species==2 || species==3)
    {
      //SpeciesString="(i_geant_pid==2 || i_geant_pid==3)";
      SpeciesString="n_rich_patmatrix>400 && f_rk_beta>0.85";
    }
  
  if(species==8 || species==9)
    {
      //SpeciesString="(i_geant_pid==8 || i_geant_pid==9)";
      SpeciesString="!b_ring && n_charge_rk<0";
    }
  if(species==14)
    {
      //SpeciesString="i_geant_pid==14";
      SpeciesString="!b_ring && n_charge_rk>0 && f_rk_beta<0.8";
    }
  
  TCut SpeciesCut(SpeciesString);

  TString systemcond("n_system==");
  systemcond+=system;
  TCut SystemCut(systemcond);

  TString sectorcond;
  
  TCut TrackCut;
  if(useTrueParticlesOnly)
    {
      TrackCut = "b_rktrack &&f_rk_quality<1000 && (i_geant_corrflag==74 || i_geant_corrflag==78)";
    }
  else
    {
      TCut TrackCut("b_rktrack &&f_rk_quality<1000");
    }

  TCut SectorCut("");
  
  for(Int_t sector=0;sector<6; sector++)
    {
      histname = "deviationX_rk_s";
      histname+=sector;
      deviationX_rk[sector]= new TH2F(histname.Data(),histname.Data(),90,0,90,200,-100,100);

      histname = "deviationY_rk_s";
      histname+=sector;
      deviationY_rk[sector]= new TH2F(histname.Data(),histname.Data(),90,0,90,200,-100,100);

      histname = "deviationX_mdc_s";
      histname+=sector;
      deviationX_mdc[sector]= new TH2F(histname.Data(),histname.Data(),90,0,90,200,-100,100);

      histname = "deviationY_mdc_s";
      histname+=sector;
      deviationY_mdc[sector]= new TH2F(histname.Data(),histname.Data(),90,0,90,200,-100,100);
    }

  for(Int_t sector=0;sector<6; sector++)
    {
      sectorcond="n_sector==";
      sectorcond+=sector;
      SectorCut=sectorcond;

      DrawString="f_meta_rk_dx:f_rk_theta>>deviationX_rk_s";
      DrawString+=sector;
      N->Draw(DrawString.Data(),TrackCut && SectorCut && SystemCut,"",nentries);

      DrawString="f_meta_rk_dy:f_rk_theta>>deviationY_rk_s";
      DrawString+=sector;
      N->Draw(DrawString.Data(),TrackCut && SectorCut && SystemCut,"",nentries);
    }

  TCanvas* Delta_x = new TCanvas("Delta_x","Delta_x");
  Delta_x->Divide(2,3);
  TCanvas* Delta_y = new TCanvas("Delta_y","Delta_y");
  Delta_y->Divide(2,3);


  for(Int_t sector=0;sector<6; sector++)
    {
      Delta_x->cd(sector+1);
      deviationX_rk[sector]->Draw("colz");

      Delta_y->cd(sector+1);
      deviationY_rk[sector]->Draw("colz");
    }

  if(useTrueParticlesOnly)
    {
      TString NameBaseX="Delta_x_vs_rktheta_trueID";
      TString NameBaseY="Delta_y_vs_rktheta_trueID";
      TString FileNameX=NameBaseX+species+"_system_"+system+".ps";
      TString FileNameY=NameBaseY+species+"_system_"+system+".ps";
      Delta_x->SaveAs(FileNameX.Data());
      Delta_y->SaveAs(FileNameY.Data());
      
      FileNameX=NameBaseX+species+"_system_"+system+".root";
      FileNameY=NameBaseY+species+"_system_"+system+".root";
      Delta_x->SaveAs(FileNameX.Data());
      Delta_y->SaveAs(FileNameY.Data());
    }
  else
    {
      TString NameBaseX="Delta_x_vs_rktheta_reconstructedID";
      TString NameBaseY="Delta_y_vs_rktheta_reconstructedID";
      TString FileNameX=NameBaseX+species+"_system_"+system+".ps";
      TString FileNameY=NameBaseY+species+"_system_"+system+".ps";
      Delta_x->SaveAs(FileNameX.Data());
      Delta_y->SaveAs(FileNameY.Data());
      
      FileNameX=NameBaseX+species+"_system_"+system+".root";
      FileNameY=NameBaseY+species+"_system_"+system+".root";
      Delta_x->SaveAs(FileNameX.Data());
      Delta_y->SaveAs(FileNameY.Data());

    }
}



//This function plots the dependence of the correlation width upon the azimuthal angle for different species.
computeCorrelationWidthVsRKPhi(Int_t system, Int_t species, Bool_t useTrueParticlesOnly, Int_t nentries=900000000)
{
  TChain* N = makeChain("N");

  TH2F* deviationX_rk[6];
  TH2F* deviationY_rk[6];
  TH2F* deviationX_mdc[6];
  TH2F* deviationY_mdc[6];
  
  TString histname;
  TString DrawString;

  TString SpeciesString("");
  if(species==2 || species==3)
    {
      //SpeciesString="(i_geant_pid==2 || i_geant_pid==3)";
      SpeciesString="n_rich_patmatrix>400 && f_rk_beta>0.85";
    }
  
  if(species==8 || species==9)
    {
      //SpeciesString="(i_geant_pid==8 || i_geant_pid==9)";
      SpeciesString="!b_ring && n_charge_rk<0";
    }
  if(species==14)
    {
      //SpeciesString="i_geant_pid==14";
      SpeciesString="!b_ring && n_charge_rk>0 && f_rk_beta<0.8";
    }
  
  TCut SpeciesCut(SpeciesString);

  TString systemcond("n_system==");
  systemcond+=system;
  TCut SystemCut(systemcond);

  TString sectorcond;
  
  TCut TrackCut;
  if(useTrueParticlesOnly)
    {
      TrackCut = "b_rktrack &&f_rk_quality<1000 && (i_geant_corrflag==74 || i_geant_corrflag==78)";
    }
  else
    {
      TCut TrackCut("b_rktrack &&f_rk_quality<1000");
    }

  TCut SectorCut("");
  
  for(Int_t sector=0;sector<6; sector++)
    {
      histname = "deviationX_rk_s";
      histname+=sector;
      deviationX_rk[sector]= new TH2F(histname.Data(),histname.Data(),180,0,360,200,-100,100);

      histname = "deviationY_rk_s";
      histname+=sector;
      deviationY_rk[sector]= new TH2F(histname.Data(),histname.Data(),180,0,360,200,-100,100);

      histname = "deviationX_mdc_s";
      histname+=sector;
      deviationX_mdc[sector]= new TH2F(histname.Data(),histname.Data(),180,0,360,200,-100,100);

      histname = "deviationY_mdc_s";
      histname+=sector;
      deviationY_mdc[sector]= new TH2F(histname.Data(),histname.Data(),180,0,360,200,-100,100);
    }

  for(Int_t sector=0;sector<6; sector++)
    {
      sectorcond="n_sector==";
      sectorcond+=sector;
      SectorCut=sectorcond;

      DrawString="f_meta_rk_dx:f_rk_phi>>deviationX_rk_s";
      DrawString+=sector;
      N->Draw(DrawString.Data(),TrackCut && SectorCut && SystemCut,"",nentries);

      DrawString="f_meta_rk_dy:f_rk_phi>>deviationY_rk_s";
      DrawString+=sector;
      N->Draw(DrawString.Data(),TrackCut && SectorCut && SystemCut,"",nentries);
    }

  TCanvas* Delta_x = new TCanvas("Delta_x","Delta_x");
  Delta_x->Divide(2,3);
  TCanvas* Delta_y = new TCanvas("Delta_y","Delta_y");
  Delta_y->Divide(2,3);


  for(Int_t sector=0;sector<6; sector++)
    {
      Delta_x->cd(sector+1);
      deviationX_rk[sector]->Draw("colz");

      Delta_y->cd(sector+1);
      deviationY_rk[sector]->Draw("colz");
    }

  if(useTrueParticlesOnly)
    {
      TString NameBaseX="Delta_x_vs_rkphi_trueID";
      TString NameBaseY="Delta_y_vs_rkphi_trueID";
      TString FileNameX=NameBaseX+species+"_system_"+system+".ps";
      TString FileNameY=NameBaseY+species+"_system_"+system+".ps";
      Delta_x->SaveAs(FileNameX.Data());
      Delta_y->SaveAs(FileNameY.Data());
      
      FileNameX=NameBaseX+species+"_system_"+system+".root";
      FileNameY=NameBaseY+species+"_system_"+system+".root";
      Delta_x->SaveAs(FileNameX.Data());
      Delta_y->SaveAs(FileNameY.Data());
    }
  else
    {
      TString NameBaseX="Delta_x_vs_rkphi_reconstructedID";
      TString NameBaseY="Delta_y_vs_rkphi_reconstructedID";
      TString FileNameX=NameBaseX+species+"_system_"+system+".ps";
      TString FileNameY=NameBaseY+species+"_system_"+system+".ps";
      Delta_x->SaveAs(FileNameX.Data());
      Delta_y->SaveAs(FileNameY.Data());
      
      FileNameX=NameBaseX+species+"_system_"+system+".root";
      FileNameY=NameBaseY+species+"_system_"+system+".root";
      Delta_x->SaveAs(FileNameX.Data());
      Delta_y->SaveAs(FileNameY.Data());

    }
}
