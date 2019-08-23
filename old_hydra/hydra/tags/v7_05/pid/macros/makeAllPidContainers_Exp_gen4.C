//The macro reads histograms from files, puts them into containers
//and stores in the RTDB for all available PID algorithms
// *** Modified: 25/04/2003
// Fake ids changed to -1 (fake-) and -2 (fake+)
// In the input files the corresponding histograms should have the "old" names
// 500 for fake- and 501 for fake+

//Below all runs for nov01 are listed
Int_t  iRunIds[] = {
  1007248432,
  1007248491,
  1007248797,
  1007249519,
  1007252897,
  1007253064,
  1007254393,
  1007255539,
  1007256661,
  1007256755,
  1007257525,
  1007258953,
  1007262297,
  1007263146,
  1007263546,
  1007264153,
  1007264628,
  1007265260,
  1007266039,
  1007266672,
  1007267099,
  1007267627,
  1007268205,
  1007332012,
  1007332624,
  1007332927,
  1007333348,
  1007334331,
  1007334918,
  1007335273,
  1007335872,
  1007336628,
  1007337176,
  1007337734,
  1007338299,
  1007338867,
  1007339427,
  1007340236,
  1007340322,
  1007341100,
  1007341363,
  1007341440,
  1007342267,
  1007342831,
  1007343400,
  1007343960,
  1007344539,
  1007345526,
  1007345840,
  1007346894,
  1007349572,
  1007350571,
  1007350868,
  1007351839,
  1007353199,
  1007415918,
  1007416407,
  1007416921,
  1007417472,
  1007418012,
  1007418743,
  1007419286,
  1007419822,
  1007420389,
  1007420650,
  1007421206,
  1007421760,
  1007422289,
  1007422840,
  1007423390,
  1007423735,
  1007423835,
  1007424365,
  1007425295,
  1007425889,
  1007426459,
  1007427038,
  1007427633,
  1007428207,
  1007428790,
  1007429425,
  1007430166,
  1007430693,
  1007431254,
  1007431850,
  1007432478,
  1007433037,
  1007433571,
  1007433960,
  1007434468,
  1007435187,
  1007435668,
  1007436218,
  1007436797,
  1007437374,
  1007437900,
  1007438573,
  1007439197,
  1007439758,
  1007440293,
  1007441007,
  1007441529,
  1007507460,
  1007507938,
  1007508157,
  1007508731,
  1007508968,
  1007509555,
  1007509610,
  1007510069,
  1007510618,
  1007511314,
  1007511817,
  1007512410,
  1007512877,
  1007513770,
  1007514259,
  1007514724,
  1007515171,
  1007515636,
  1007516109,
  1007516781,
  1007517698,
  1007518638,
  1007519093,
  1007519659,
  1007520140,
  1007520627,
  1007520980,
  1007521493,
  1007521972,
  1007522089,
  1007522558,
  1007523188,
  1007524212,
  1007524681,
  1007525141,
  1007525590,
  1007526051,
  1007526761,
  1007527690,
  1007593758,
  1007594375,
  1007595029,
  1007595669,
  1007596297,
  1007596971,
  1007597478,
  1007598194,
  1007598949,
  1007600030,
  1007600499,
  1007601236,
  1007601958,
  1007602688,
  1007603390,
  1007604075,
  1007604784,
  1007605800,
  1007606493,
  1007607245,
  1007607799,
  1007608349,
  1007609037,
  1007609723,
  1007610769,
  1007611731,
  1007612439,
  1007612976,
  1007613564,
  1007652912,
  1007653452,
  1007655434,
  1007657078,
  1007658495,
  1007663002,
  1007664579,
  1007681490,
  1007681822,
  1007682538,
  1007684573,
  1007685450,
  1007686089,
  1007686733,
  1007689848,
  1007690318,
  1007690989,
  1007691649,
  1007692287,
  1007692922,
  1007693559,
  1007694415,
  1007695721,
  1007696035,
  1007696672,
  1007697315,
  1007698192,
  1007698823,
  1007699576,
  1007700211,
  1007700840,
  1007701460,
  1007724215,
  1007726219,
  1007727510,
  1007728378,
  1007729168,
  1007731825,
  1007733049,
  1007734277,
  1007735978,
  1007736641,
  1007737524,
  1007739213,
  1007740629,
  1007742644,
  1007743321,
  1007743848,
  1007744496,
  1007745115,
  1007746054,
  1007747108,
  1007750525,
  1007751826,
  1007752466,
  1007753119,
  1007754836,
  1007756140,
  1007773051,
  1007773929,
  1007774776,
  1007775484,
  1007775913,
  1007776612,
  1007777307,
  1007777992,
  1007778683,
  1007779128,
  1007779825
};

// -----------------------------------------------------------------------------

//Here we copy the components of the RICH pattern - eigenvectors required by the
//ring recognition algorithm for 4 and 5 D transformation into the eigenspace:
//These numbers are the result from the PCAs.
//5D:
static Double_t g5DSigmaValues[] = {
  233.893,
  8.88753,
  0.88211,
  1.97674,
  5.41408
  };

static Double_t g5DMeanValues[] = {
  500.515,
  17.5781,
  1.55246,
  5.36095,
  12.848
  };

static Double_t g5DEigenVectors[] = {
  0.511807,
  -0.0669607,
  -0.19247,
  0.555715,
  0.622661,
  0.520511,
  -0.072322,
  -0.207421,
  0.300906,
  -0.76829,
  -0.358738,
  -0.60784,
  -0.708359,
  0.00399291,
  0.0069804,
  0.466428,
  0.244896,
  -0.449009,
  -0.708658,
  0.13662,
  0.347668,
  -0.748894,
  0.46535,
  -0.313726,
  0.0575319
};


//4D:
static Double_t g4DMeanValues[] = {
  447.082,
  15.3036,
  4.61516,
  13.1432
  };
  
  // Assignment to sigma value vector. Zero-based.
  static Double_t g4DSigmaValues[] = {
    184.546,
    6.91697,
    1.60377,
    5.24936
  };


static Double_t g4DEigenVectors[] = {
  0.562784,
  0.0529041,
  -0.516639,
  -0.643086,
  0.573241,
  0.0544357,
  -0.311105,
  0.756072,
  0.48151,
  0.500875,
  0.710972,
  -0.108588,
  0.350453,
  -0.862184,
  0.361687,
  -0.0548066
};

//Efficiencies of the RICH for leptons and hadrons, measured from simulations:
Float_t RichLeptonEfficiency = 0.53;
Float_t RichHadronEfficiency = 4.0E-3;


char   sOutputFile[] = "AllExpPidContainersNov01_gen4.root";  
Int_t  aIds[] = { 2, 3, 8, 9, 11, 14, 45, -1, -2 };  

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
  
  new TFile("/d/hades/pid/PID_PARAMS/CURRENT_VERSION/pidpar_tof_relint_nov01gen4_280303.root");
  setRelInt(1, pRecPar);
  
  new TFile("/d/hades/pid/PID_PARAMS/CURRENT_VERSION/pidpar_tofino_relint_nov01gen4_280303.root");
  setRelInt(0, pRecPar);
  
  pRecPar->print();
  
  // -------------------------------------------------------------------------
  // HPidAlgMomVsBetaPar
  HPidAlgMomVsBetaPar *pMomVsBeta
    = (HPidAlgMomVsBetaPar *) rtdb->getContainer("PidAlgMomVsBetaPar");
  
  new TFile("/d/hades/pid/PID_PARAMS/CURRENT_VERSION/pidpar_tof_pdf_nov01gen4_280303.root");
  
  // try to renormalize the pol2 distributions
  getAndRenormalize(1);
  setPdf(1, pMomVsBeta);
  
  new TFile("/d/hades/pid/PID_PARAMS/CURRENT_VERSION/pidpar_tofino_pdf_nov01gen4_280303.root");
  getAndRenormalize(0);
  setPdf(0, pMomVsBeta);
  
  pMomVsBeta->print();
  
  // -------------------------------------------------------------------------
  // HPidAlgMomVsBetaPar
  
  HPidAlgMomVsElossPar *pMomVsEloss = (HPidAlgMomVsElossPar *) rtdb->getContainer("PidAlgMomVsElossPar");
  
  new TFile("/u/laserhad/dusan/pid/gen4/par/exp/elosspdf/pidpar_elosspdf_exp_tof_240603.root");
  setPdf(1, pMomVsEloss);
  new TFile("/u/laserhad/dusan/pid/gen4/par/exp/elosspdf/pidpar_elosspdf_exp_tofino_240603.root");
  setPdf(0, pMomVsEloss);
  pMomVsEloss->print();
  
  
  HPidElossMPVShiftPar *pElossMPVShift = (HPidElossMPVShiftPar *) rtdb->getContainer("PidElossMPVShiftPar");
  new TFile("/u/laserhad/dusan/pid/gen4/par/exp/elossmpvshift/pidpar_elossmpvshift_exp_tof_240603.root");
  setPdf(1, pElossMPVShift);
  new TFile("/u/laserhad/dusan/pid/gen4/par/exp/elossmpvshift/pidpar_elossmpvshift_exp_tofino_240603.root");
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
  
  new TFile("/d/hades/pid/PID_PARAMS/CURRENT_VERSION/RICH_PID_PDFs_sim_nov01_gen4.root");
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
	 sprintf(s,"pdf_leptons_withcentroid_sim_nov01_gen4");
	 offset=0;
	 pH = (TH1 *) gDirectory->Get(s);
	 pRingProperties->addHistogram(aIds[i],offset, pH);
	 sprintf(s,"pdf_leptons_nocentroid_sim_nov01_gen4");
	 offset=1;
	 pH = (TH1 *) gDirectory->Get(s);
	 pRingProperties->addHistogram(aIds[i],offset, pH);
	 sprintf(s,"cl_leptons_withcentroid_sim_nov01_gen4");
	 offset=2;
	 pH = (TH1 *) gDirectory->Get(s);
	 pRingProperties->addHistogram(aIds[i],offset, pH);
	 sprintf(s,"cl_leptons_nocentroid_sim_nov01_gen4");
	 offset=3;
	 pH = (TH1 *) gDirectory->Get(s);
	 pRingProperties->addHistogram(aIds[i],offset, pH);
       }
     else //we get the pdfs and cls for "hadronic rings" (i.e. fakes)
       {
	 sprintf(s,"pdf_hadrons_withcentroid_sim_nov01_gen4");
	 offset=0;
	 pH = (TH1 *) gDirectory->Get(s);
	 pRingProperties->addHistogram(aIds[i],offset, pH);
	 sprintf(s,"pdf_hadrons_nocentroid_sim_nov01_gen4");
	 offset=1;
	 pH = (TH1 *) gDirectory->Get(s);
	 pRingProperties->addHistogram(aIds[i],offset, pH);
	 sprintf(s,"cl_hadrons_withcentroid_sim_nov01_gen4");
	 offset=2;
	 pH = (TH1 *) gDirectory->Get(s);
	 pRingProperties->addHistogram(aIds[i],offset, pH);
	 sprintf(s,"cl_hadrons_nocentroid_sim_nov01_gen4");
	 offset=3;
	 pH = (TH1 *) gDirectory->Get(s);
	 pRingProperties->addHistogram(aIds[i],offset, pH);
       }
   }
}
