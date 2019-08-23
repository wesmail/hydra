//@(#)$Id: makeAllPidContainers_Sim_gen6.C,v 1.1 2003-11-06 12:13:28 christ Exp $
//The macro reads hitogram from files, puts them into containers
//and stores in the RTDB for all available PID algorithms
// *** Modified: 25/04/2003
// Fake ids changed to -1 (fake-) and -2 (fake+)
// In the input files the corresponding histograms should have the "old" names
// 500 for fake- and 501 for fake+

Int_t  iRunIds[] = { 1, 2,3,4 };
char   sOutputFile[] = "AllSimPidContainersNov01_gen6.root";

//Here we copy the components of the RICH pattern - eigenvectors required by the
//ring recognition algorithm for 4 and 5 D transformation into the eigenspace:
//These numbers are the result from the PCAs.
//5D:
static Double_t g5DSigmaValues[] = {
  254.272,
  9.95008,
  0.815296,
  2.12174,
  5.62902
};

static Double_t g5DMeanValues[] = {
  655.041,
  23.9409,
  1.31384,
  6.22681,
  17.0839
};

static Double_t g5DEigenVectors[] = {
0.512993,
  -0.0158782,
  -0.202355,
  0.590602,
  0.588921,
  0.52541,
  -0.032946,
  -0.229784,
  0.243353,
  -0.781561,
  -0.383304,
  -0.445214,
  -0.808445,
  0.0344852,
  0.00951428,
  0.474916,
  0.234536,
  -0.383237,
  -0.731305,
  0.194348,
  0.297179,
  -0.863389,
  0.325267,
  -0.236586,
  0.0668806
};


//4D:
static Double_t g4DMeanValues[] = {
  557.489,
  19.7653,
  5.16761,
  16.9831
  };
  
  // Assignment to sigma value vector. Zero-based.
  static Double_t g4DSigmaValues[] = {
  205.643,
  7.86032,
  1.73207,
  5.82129
  };


static Double_t g4DEigenVectors[] = {
0.565749,
  0.0723779,
  -0.534636,
  -0.623582,
  0.579746,
  0.060112,
  -0.271622,
  0.765835,
  0.505984,
  0.387295,
  0.756946,
  -0.144966,
  0.296315,
  -0.917142,
  0.259652,
  -0.0602331
};

//Efficiencies of the RICH for leptons and hadrons, measured from simulations:
Float_t RichLeptonEfficiency = 0.38;
Float_t RichHadronEfficiency = 3.0E-3;


Int_t  aIds[] = { 2, 3, 8, 9, 11, 14, 45, -1, -2 };

// -----------------------------------------------------------------------------

void makeAllPidContainers_Sim(void)
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
  
  new TFile("/d/hades/pid/PID_PARAMS/CURRENT_VERSION/pidpar_tof_relint_cc2gen4_260503.root");
  setRelInt(1, pRecPar);
  
  new TFile("/d/hades/pid/PID_PARAMS/CURRENT_VERSION/pidpar_tofino_relint_cc2gen4sim_out150503_010603.root");
  setRelInt(0, pRecPar);
  
  pRecPar->print();
  
  // -------------------------------------------------------------------------
  // HPidAlgMomVsBetaPar
  HPidAlgMomVsBetaPar *pMomVsBeta
    = (HPidAlgMomVsBetaPar *) rtdb->getContainer("PidAlgMomVsBetaPar");
  
  new TFile("/d/hades/pid/PID_PARAMS/CURRENT_VERSION/pidpar_tof_pdf_cc2gen4_260503.root");
  
  // try to renormalize the pol2 distributions
  getAndRenormalize(1);
  setPdf(1, pMomVsBeta);
  
  new TFile("/d/hades/pid/PID_PARAMS/CURRENT_VERSION/pidpar_tofino_pdf_cc2gen4sim_out150503_010603.root");
  getAndRenormalize(0);
  setPdf(0, pMomVsBeta);
  
  pMomVsBeta->print();
  
  // -------------------------------------------------------------------------
  // HPidAlgMomVsBetaPar
  
  HPidAlgMomVsElossPar *pMomVsEloss = (HPidAlgMomVsElossPar *) rtdb->getContainer("PidAlgMomVsElossPar");
  new TFile("/d/hades/pid/PID_PARAMS/CURRENT_VERSION/pidpar_elosspdf_sim_nov01_gen4_tof_050603.root");
  setPdf(1, pMomVsEloss);

  new TFile("/d/hades/pid/PID_PARAMS/CURRENT_VERSION/pidpar_elosspdf_sim_nov01_gen4_tofino_050603.root");
  setPdf(0, pMomVsEloss);
  pMomVsEloss->print();
  
  
  HPidElossMPVShiftPar *pElossMPVShift = (HPidElossMPVShiftPar *) rtdb->getContainer("PidElossMPVShiftPar");
  new TFile("/d/hades/pid/PID_PARAMS/CURRENT_VERSION/pidpar_elossmpvshift_sim_nov01_gen4_tof_050603.root");
  setPdf(1, pElossMPVShift);
  new TFile("/d/hades/pid/PID_PARAMS/CURRENT_VERSION/pidpar_elossmpvshift_sim_nov01_gen4_tofino_050603.root");
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
  
  //TFile* rich_pdfs= new TFile("/d/hades/pid/PID_PARAMS/CURRENT_VERSION/RICH_PID_PDFs_sim_nov01_gen6.root");
  TFile* rich_pdfs= new TFile("./RICH_PID_PDFs_sim_nov01_gen6.root");
  addRichPdfs(pRingProperties);

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
  pRingProperties->setRichHadronEfficiency(RichHadronEfficiency);
  pRingProperties->setRichLeptonEfficiency(RichLeptonEfficiency);
  
  
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
  Int_t i;
  char  s[100];
  TH1  *pH;
  Int_t offset=0;
//loop over all species 
 for(i = 0; i < sizeof(aIds) / sizeof(Int_t); i++)
   {
     if(aIds[i]<=3 && aIds[i]>0) //we are creating pdfs for electrons and positrons!
       {
	 sprintf(s,"pdf_leptons_withcentroid_sim_nov01_gen6");
	 offset=0;
	 pH = (TH1 *) gDirectory->Get(s);
	 pRingProperties->addHistogram(aIds[i],offset, pH);
	 sprintf(s,"pdf_leptons_nocentroid_sim_nov01_gen6");
	 offset=1;
	 pH = (TH1 *) gDirectory->Get(s);
	 pRingProperties->addHistogram(aIds[i],offset, pH);
	 sprintf(s,"cl_leptons_withcentroid_sim_nov01_gen6");
	 offset=2;
	 pH = (TH1 *) gDirectory->Get(s);
	 pRingProperties->addHistogram(aIds[i],offset, pH);
	 sprintf(s,"cl_leptons_nocentroid_sim_nov01_gen6");
	 offset=3;
	 pH = (TH1 *) gDirectory->Get(s);
	 pRingProperties->addHistogram(aIds[i],offset, pH);
       }
     else
       {
	 sprintf(s,"pdf_hadrons_withcentroid_sim_nov01_gen6");
	 offset=0;
	 pH = (TH1 *) gDirectory->Get(s);
	 pRingProperties->addHistogram(aIds[i],offset, pH);
	 sprintf(s,"pdf_hadrons_nocentroid_sim_nov01_gen6");
	 offset=1;
	 pH = (TH1 *) gDirectory->Get(s);
	 pRingProperties->addHistogram(aIds[i],offset, pH);
	 sprintf(s,"cl_hadrons_withcentroid_sim_nov01_gen6");
	 offset=2;
	 pH = (TH1 *) gDirectory->Get(s);
	 pRingProperties->addHistogram(aIds[i],offset, pH);
	 sprintf(s,"cl_hadrons_nocentroid_sim_nov01_gen6");
	 offset=3;
	 pH = (TH1 *) gDirectory->Get(s);
	 pRingProperties->addHistogram(aIds[i],offset, pH);
       }
   }
}

