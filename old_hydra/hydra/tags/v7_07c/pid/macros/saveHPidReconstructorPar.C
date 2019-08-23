//@(#)$Id: saveHPidReconstructorPar.C,v 1.4 2003-03-05 11:24:39 jaskula Exp $
//The macro reads hitogram from files, puts them into containers
//and stores in the RTDB
// *** Modified: 05/03/2003
// Fake ids changed to -1 (fake-) and -2 (fake+)
// In the input files the corresponding histograms should have the "old" names
// 500 for fake- and 501 for fake+

Int_t  iRunIds[] = { 1, 990456436, 990420780, 1007598949, 142 };
char   sOutputFile[] = "pidparams.root";

Int_t  aIds[] = { 2, 3, 8, 9, 14, 45, -1, -2 };

// -----------------------------------------------------------------------------

void saveHPidReconstructorPar(void)
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

    new TFile("pidpar_tof_relint_cc2gen2_240103.root");
    setRelInt(1, pRecPar);

    new TFile("pidpar_tofino_relint_cc2gen2_210103.root");
    setRelInt(0, pRecPar);

    pRecPar->print();

    // -------------------------------------------------------------------------
    // HPidAlgMomVsBetaPar
HPidAlgMomVsBetaPar *pMomVsBeta
        = (HPidAlgMomVsBetaPar *) rtdb->getContainer("PidAlgMomVsBetaPar");

    new TFile("pidpar_tof_pdf_cc2gen2_240103.root");

    // try to renormalize the pol2 distributions
    getAndRenormalize(1);
    setPdf(1, pMomVsBeta);

    new TFile("pidpar_tofino_pdf_cc2gen2_210103.root");
    getAndRenormalize(0);
    setPdf(0, pMomVsBeta);

    pMomVsBeta->print();

    // -------------------------------------------------------------------------
    // HPidTrackFillerPar
HPidTrackFillerPar *pFillerPar
        = (HPidTrackFillerPar *) rtdb->getContainer("PidTrackFillerPar");

    //pFillerPar->setTofinoMult(1);
    pFillerPar->print();

    // -------------------------------------------------------------------------
    // write the parameters to RTDB output

HParRootFileIo *out = new HParRootFileIo;

    out->open(sOutputFile, "RECREATE");
    rtdb->setOutput(out);

    rtdb->initContainers(iRunIds[0]);

    pRecPar->setChanged();
    pMomVsBeta->setChanged();
    pFillerPar->setChanged();
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
