//@(#)$Id: makeAllPidContainers_Exp_Nov01_gen6_3D.C,v 1.1 2004-04-30 16:22:25 christ Exp $
//The macro reads hitogram from files, puts them into containers
//and stores in the RTDB for all available PID algorithms
// *** Modified: 25/04/2003
// Fake ids changed to -1 (fake-) and -2 (fake+)
// In the input files the corresponding histograms should have the "old" names
// 500 for fake- and 501 for fake+

Int_t  iRunIds[] = { 1, 2,3,4 };
char   sOutputFile[] = "~/hades/hydra/macros/PID/RESULTS/NOV01_GEN6_2/AllExpPidContainersNov01_gen6_3D.root";

//Here we copy the components of the RICH pattern - eigenvectors required by the
//ring recognition algorithm for 4 and 5 D transformation into the eigenspace:
//These numbers are the result from the PCAs.
//5D:
static Double_t g5DSigmaValues[] = {
  254.767,
  9.96323,
  0.812024,
  2.12707,
  5.65131
};

static Double_t g5DMeanValues[] = {
  655.433,
  23.9619,
  1.31085,
  6.23431,
  17.0778
};

static Double_t g5DEigenVectors[] = {
  0.513059,
  -0.0172409,
  -0.199397,
  0.595494,
  0.584894,
  0.525723,
  -0.0334878,
  -0.228409,
  0.238797,
  -0.783134,
  -0.381405,
  -0.452647,
  -0.805111,
  0.0367433,
  0.00933883,
  0.475013,
  0.233109,
  -0.387005,
  -0.728259,
  0.19972,
  0.298795,
  -0.859856,
  0.331804,
  -0.238015,
  0.0680005};
//4D:
static Double_t g4DMeanValues[] = {
  558.631,
  19.8035,
  5.17075,
  16.9618
};
  
  // Assignment to sigma value vector. Zero-based.
static Double_t g4DSigmaValues[] = {
  205.781,
  7.9107,
  1.73394,
  5.78266
};


static Double_t g4DEigenVectors[] = {
  0.566004,
  0.068258,
  -0.535285,
  -0.623258,
  0.580081,
  0.0607166,
  -0.27064,
  0.765881,
  0.504831,
  0.392407,
  0.754732,
  -0.14677,
  0.297138,
  -0.915244,
  0.265713,
  -0.058601};

//Fractions of leptons and hadrons associated to a RICH signal, measured from simulations: 
//(This is NOT RICH efficiency!)
Float_t RichLeptonFraction[10] = {0.46,0.76,0.73,0.69,0.65,0.59,0.53,0.4,0.23,0.04};
Float_t RichHadronFraction[10] = {0.0066,0.0032,0.0018,0.0016,0.0016,0.0016,0.0017,0.0017,0.0017,0.0004};


Int_t  aIds[] = { 2, 3, 8, 9, 11, 14, 45, -1, -2 };

// -----------------------------------------------------------------------------

void makeAllPidContainers_Exp(void)
{
  Hades myHades;
  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  
  // -------------------------------------------------------------------------
  // copy histograms from Pavel's files
  Int_t  i, sys, par;
  char   s[100];
  TH1   *pH;
  
  // -------------------------------------------------------------------------
  // HPidReconstructorPar
  HPidReconstructorPar *pRecPar
    = (HPidReconstructorPar *) rtdb->getContainer("PidReconstructorPar");
  
  new TFile("~/hades/PID_PARAMS/CURRENT_VERSION/EXP/pidpar_tof_relint_nov01gen4_280303.root");
  setRelInt(1, pRecPar);
  
  new TFile("~/hades/PID_PARAMS/CURRENT_VERSION/EXP/pidpar_tofino_relint_nov01gen4_280303.root");
  setRelInt(0, pRecPar);
  
  pRecPar->print();
  
  // -------------------------------------------------------------------------
  // HPidAlgMomVsBetaPar
  HPidAlgMomVsBetaPar *pMomVsBeta
    = (HPidAlgMomVsBetaPar *) rtdb->getContainer("PidAlgMomVsBetaPar");
  
  new TFile("~/hades/PID_PARAMS/CURRENT_VERSION/EXP/pidpar_tof_pdf_nov01gen4_280303.root");
  
  // try to renormalize the pol2 distributions
  getAndRenormalize(1);
  setPdf(1, pMomVsBeta);
  
  new TFile("~/hades/PID_PARAMS/CURRENT_VERSION/EXP/pidpar_tofino_pdf_nov01gen4_280303.root");
  getAndRenormalize(0);
  setPdf(0, pMomVsBeta);
  
  pMomVsBeta->print();
  
  // -------------------------------------------------------------------------
  // HPidAlgMomVsBetaPar
  
  HPidAlgMomVsElossPar *pMomVsEloss = (HPidAlgMomVsElossPar *) rtdb->getContainer("PidAlgMomVsElossPar");
  new TFile("~/hades/PID_PARAMS/CURRENT_VERSION/EXP/pidpar_elosspdf_exp_nov01_gen6_tof_240603.root");
  setPdf(1, pMomVsEloss);

  new TFile("~/hades/PID_PARAMS/CURRENT_VERSION/EXP/pidpar_elosspdf_exp_nov01_gen6_tofino_240603.root");
  setPdf(0, pMomVsEloss);
  pMomVsEloss->print();
  
  
  HPidElossMPVShiftPar *pElossMPVShift = (HPidElossMPVShiftPar *) rtdb->getContainer("PidElossMPVShiftPar");
  new TFile("~/hades/PID_PARAMS/CURRENT_VERSION/EXP/pidpar_elossmpvshift_exp_nov01_gen6_tof_240603.root");
  setPdf(1, pElossMPVShift);
  new TFile("~/hades/PID_PARAMS/CURRENT_VERSION/EXP/pidpar_elossmpvshift_exp_nov01_gen6_tofino_240603.root");
  setPdf(0, pElossMPVShift);
  pElossMPVShift->print();
  
  // -------------------------------------------------------------------------
  // HPidTrackFillerPar
  HPidTrackFillerPar *pFillerPar
    = (HPidTrackFillerPar *) rtdb->getContainer("PidTrackFillerPar");
  
  //pFillerPar->setTofinoMult(1);
  pFillerPar->print();
  
  // -------------------------------------------------------------------------
  // HPidRingPropertiesPar
  HPidAlgRingPropertiesPar *pRingProperties
    = (HPidAlgRingPropertiesPar *) rtdb->getContainer("PidAlgRingPropertiesPar");
  
  TFile* rich_pdfs= new TFile("~/hades/PID_PARAMS/CURRENT_VERSION/EXP/RICH_PID_PDFs_sim_nov01_gen6_3D.root");
  //YES! for RICH sim and exp are identical!
  addRichPdfs(pRingProperties);
  cout << "added ring pdfs" << endl;
  //Here we set the Eigenvectors, Sigmas and Means for the transformation into the
  //Pattern-Eigenspace in 4 and 5 dimensions:
  for(Int_t counter4D=0;counter4D<16;counter4D++)
    {
      pRingProperties->set4DEvec(counter4D,g4DEigenVectors[counter4D]);
      if(counter4D<4)
	{
	  pRingProperties->set4DSigma(counter4D,g4DSigmaValues[counter4D]);
	  pRingProperties->set4DMean(counter4D,g4DMeanValues[counter4D]);
	}
      
    }
  for(Int_t counter5D=0;counter5D<25;counter5D++)
    {
      pRingProperties->set5DEvec(counter5D,g5DEigenVectors[counter5D]);
      if(counter5D<5)
	{
	  pRingProperties->set5DSigma(counter5D,g5DSigmaValues[counter5D]);
	  pRingProperties->set5DMean(counter5D,g5DMeanValues[counter5D]);
	}
      
    }
  for(int i=0;i<10;i++){
    pRingProperties->setRichHadronFraction(i,RichHadronFraction[i]);
    pRingProperties->setRichLeptonFraction(i,RichLeptonFraction[i]);
  }
  
  pRingProperties->print();
  
  // -------------------------------------------------------------------------
  // write the parameters to RTDB output
  
  HParRootFileIo *out = new HParRootFileIo;
  
  out->open(sOutputFile, "RECREATE");
  rtdb->setOutput(out);
  
  rtdb->initContainers(iRunIds[0]);
  
  pRecPar->setChanged();
  pMomVsBeta->setChanged();
  pMomVsEloss->setChanged();
  pElossMPVShift->setChanged();
  pFillerPar->setChanged();
  pRingProperties->setChanged();
  rtdb->setContainersStatic();
  
  for(i = 1; i < sizeof(iRunIds) / sizeof(Int_t); i++)
    rtdb->initContainers(iRunIds[i], iRunIds[0]);
  
  rtdb->print();
  
  rtdb->saveOutput();
  rtdb->closeOutput();
}

// -----------------------------------------------------------------------------

void renormalize(TH1 *hPol[3])
{
  // Normalize the pol2 in range [0, 1.5] for bins where pol2>0
  Int_t    iX, iY, i;
  Stat_t   aVal[3];
  Double_t dX, dL, dF, dS;
  Int_t    iBins = 1000;
  Double_t dRangeMin = 0.0;
  Double_t dRangeMax = 1.5;
  
  dL = (dRangeMax - dRangeMin) / iBins;
  
  for(iX = 1; iX <= hPol[0]->GetNbinsX(); iX++)
    {
      for(iY = 1; iY <= hPol[0]->GetNbinsY(); iY++)
        {
	  dS = 0.0;
	  
	  for(i = 0; i < 3; i++)
	    aVal[i] = hPol[i]->GetBinContent(hPol[i]->GetBin(iX, iY));
	  
	  for(i = 0; i < iBins; i++)
            {
	      dX = (i + 0.5) * dL + dRangeMin;
	      dF = aVal[0] + aVal[1] * dX + aVal[2] * dX * dX;
	      
	      if(dF > 0.0)
		dS += dF * dL;
            }
	  
	  if(dS > 0.0)
            {
	      for(i = 0; i < 3; i++)
		aVal[i] /= dS;
	      
	      for(i = 0; i < 3; i++)
		hPol[i]->SetBinContent(hPol[i]->GetBin(iX, iY), aVal[i]);
            }
        }
    }
}

// -----------------------------------------------------------------------------

void setRelInt(Int_t iSys, HPidReconstructorPar *pRecPar)
{
  Int_t i;
  TH1  *pH;
  char  s[100];
  
  for(i = 0; i < sizeof(aIds) / sizeof(Int_t); i++)
    {
      if(aIds[i] >= 0)
        {
	  sprintf(s, "h_relint_s%d_%d", iSys, aIds[i]);
        }
      else
        {
	  sprintf(s, "h_relint_s%d_%d", iSys, 499 - aIds[i]);
        }
      
      if((pH = (TH1 *) gDirectory->Get(s)) == NULL)
        {
	  Error("setRelInt", "No histogram: %s !!!\n", s);
	  continue;
        }
      
      pRecPar->addHistogram(aIds[i], iSys, pH);
    }
}

// -----------------------------------------------------------------------------

void getAndRenormalize(Int_t iSys)
{
  TH1   *hPol[3];
  Bool_t bOk;
  Int_t  i, par;
  char   s[100];
  
  for(i = 500; i <= 501; i++)
    {
      bOk = kTRUE;
      for(par = 0; par < 3; par++)
        {
	  sprintf(s, "h_pdf_s%d_%d_%d", iSys, i, par);
	  
	  if((hPol[par] = (TH1 *) gDirectory->Get(s)) != NULL)
	    continue;
	  
	  Error("getAndRenormalize", "No histogram: %s", s);
	  bOk = kFALSE;
        }
      
      if( ! bOk)
        {
	  Error("getAndRenormalize", "No histogram: %s", s);
	  continue;
        }
      
      printf("renormalizing %d %d\n", iSys, i);
      
      renormalize(hPol);
    }
}

// -----------------------------------------------------------------------------

void setPdf(Int_t iSys, HPidAlgMomVsBetaPar *pMomVsBeta)
{
  Int_t i, par;
  char  s[100];
  TH1  *pH;
  
  for(i = 0; i < sizeof(aIds) / sizeof(Int_t); i++)
    {
      for(par = 0; par < ((aIds[i] >= 0) ? 2 : 3); par++)
        {
	  if(aIds[i] >= 0)
            {
	      sprintf(s, "h_pdf_s%d_%d_%d", iSys, aIds[i], par + 1);
            }
	  else
            {
	      sprintf(s, "h_pdf_s%d_%d_%d", iSys, 499 - aIds[i], par);
            }
	  
	  if((pH = (TH1 *) gDirectory->Get(s)) == NULL)
            {
	      printf("No histogram: %s !!!\n", s);
	      continue;
            }
	  
	  pMomVsBeta->addHistogram(aIds[i], 3 * iSys + par, pH);
        }
    }
}
// -----------------------------------------------------------------------------

void setPdf(Int_t iSys, HPidAlgMomVsElossPar *pMomVsEloss)
{
  Int_t i, par;
  char  s[100];
  TH1  *pH;
  
  for(i = 0; i < sizeof(aIds) / sizeof(Int_t); i++)
    {
      for(par = 0; par < ((aIds[i] >= 0) ? 4 : 3); par++)
        {
	  if(aIds[i] >= 0)
            {
	      sprintf(s, "h_pdf_s%d_%d_%d", iSys, aIds[i], par + 1);
            }
	  else
            {
	      sprintf(s, "h_pdf_s%d_%d_%d", iSys, 499 - aIds[i], par);
            }
	  if((pH = (TH1 *) gDirectory->Get(s)) == NULL)
            {
	      printf("No histogram: %s !!!\n", s);
	      continue;
            }
	  pMomVsEloss->addHistogram(aIds[i], 4 * iSys + par, pH);
	  
        }
    }
}

void setPdf(Int_t iSys, HPidElossMPVShiftPar *pElossMPVShift)
{
  Int_t i, par;
  char  s[100];
  TH1  *pH;
  
  for(i = 0; i < sizeof(aIds) / sizeof(Int_t); i++)
    {
      for(par = 0; par < 1; par++)
        {
	  sprintf(s, "h_mpvsh_s%d_%d_%d", iSys, aIds[i],(par + 1));
	  
	  if((pH = (TH1 *) gDirectory->Get(s)) == NULL)
            {
	      printf("No histogram: %s !!!\n", s);
	      continue;
            }
	  
	  pElossMPVShift->addHistogram(aIds[i], iSys + par, pH);
        }
    }
}

void addRichPdfs(HPidAlgRingPropertiesPar *pRingProperties)
{
  cout << "starting to add RICH stuff" << endl;
  Int_t i, anglecount;
  char  s[100];
  TH1  *pH;
  Int_t offset=0;
  char thetacutstring[50];
  TString* anglecutstring=0;
  //loop over all species 
  for(i = 0; i < sizeof(aIds) / sizeof(Int_t); i++)
    {
      cout << "PID is : " << aIds[i] << endl;
      if(aIds[i]<=3 && aIds[i]>1) //we are creating pdfs for electrons and positrons!
	{
	  for(anglecount = 0; anglecount<4;anglecount++)
	    {
	      sprintf(thetacutstring,"%s%.1f%s%.1f","_",(2*anglecount+1)*10,"_to_",(2*anglecount+3)*10);
	      anglecutstring = new TString(thetacutstring);
	      anglecutstring->Append("_degrees");
	      sprintf(s,"%s%s","pdf_leptons_withcentroid_sim_nov01_gen6",anglecutstring->Data());
	      cout << "searching for " << s<< endl;
	      offset=0+anglecount;
	      pH = (TH1 *) gDirectory->Get(s);
	      cout << pH;
	      pRingProperties->addHistogram(aIds[i],offset, pH);
	      
	      sprintf(s,"%s%s","pdf_leptons_nocentroid_sim_nov01_gen6",anglecutstring->Data());
	      cout << "searching for " << s<< endl;
	      offset=4+anglecount;
	      pH = (TH1 *) gDirectory->Get(s);
	      cout << pH;
	      pRingProperties->addHistogram(aIds[i],offset, pH);
	      
	      sprintf(s,"%s%s","cl_leptons_withcentroid_sim_nov01_gen6",anglecutstring->Data());
	      cout << "searching for " << s<< endl;
	      offset=8+anglecount;
	      pH = (TH1 *) gDirectory->Get(s);
	      cout << pH;
	      pRingProperties->addHistogram(aIds[i],offset, pH);
	      
	      sprintf(s,"%s%s","cl_leptons_nocentroid_sim_nov01_gen6",anglecutstring->Data());
	      cout << "searching for " << s<< endl;
	      offset=12+anglecount;
	      pH = (TH1 *) gDirectory->Get(s);
	      cout << pH;
	      pRingProperties->addHistogram(aIds[i],offset, pH);
	      delete anglecutstring;
	    }
	  cout << "done with PID: " << aIds[i] << endl;
	}
      else
	{
	  for(anglecount = 0; anglecount<4;anglecount++)
	    {
	      sprintf(thetacutstring,"%s%.1f%s%.1f","_",(2*anglecount+1)*10,"_to_",(2*anglecount+3)*10);
	      anglecutstring = new TString(thetacutstring);
	      anglecutstring->Append("_degrees");
	      
	      sprintf(s,"%s%s","pdf_hadrons_withcentroid_sim_nov01_gen6",anglecutstring->Data());
	      cout << "searching for " << s<< endl;
	      offset=0+anglecount;
	      pH = (TH1 *) gDirectory->Get(s);
	      cout << pH;
	      pRingProperties->addHistogram(aIds[i],offset, pH);
	      sprintf(s,"%s%s","pdf_hadrons_nocentroid_sim_nov01_gen6",anglecutstring->Data());
	      cout << "searching for " << s<< endl;
	      offset=4+anglecount;
	      pH = (TH1 *) gDirectory->Get(s);
	      pRingProperties->addHistogram(aIds[i],offset, pH);
	      sprintf(s,"%s%s","cl_hadrons_withcentroid_sim_nov01_gen6",anglecutstring->Data());
	      cout << "searching for " << s<< endl;
	      offset=8+anglecount;
	      pH = (TH1 *) gDirectory->Get(s);
	      pRingProperties->addHistogram(aIds[i],offset, pH);
	      sprintf(s,"%s%s","cl_hadrons_nocentroid_sim_nov01_gen6",anglecutstring->Data());
	      cout << "searching for " << s<< endl;
	      offset=12+anglecount;
	      pH = (TH1 *) gDirectory->Get(s);
	      pRingProperties->addHistogram(aIds[i],offset, pH);
	      delete anglecutstring;
	    }
	}
    }
}
