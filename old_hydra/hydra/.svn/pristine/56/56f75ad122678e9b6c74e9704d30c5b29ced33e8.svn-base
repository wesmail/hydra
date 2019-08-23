//@(#)$Id: saveHPidReconstructorPar.C,v 1.3 2002-12-04 18:30:08 jaskula Exp $
//The macro reads hitogram from files, puts them into containers
//and stores in the RTDB

Int_t  iRunIds[] = { 1, 990456436, 990420780, 1007598949 };
char   sOutputFile[] = "pidparams.root";

Int_t  aIds[] = { 2, 3, 8, 9, 14, 45, 500, 501 };

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

    new TFile("Pavel_pidpar0.root");
    setRelInt(1, pRecPar);

    new TFile("pidpar_relint_tofino_nov01gen3_181102.root");
    setRelInt(0, pRecPar);

    pRecPar->print();

    // -------------------------------------------------------------------------
    // HPidAlgMomVsBetaPar
HPidAlgMomVsBetaPar *pMomVsBeta
        = (HPidAlgMomVsBetaPar *) rtdb->getContainer("PidAlgMomVsBetaPar");

    new TFile("pidpar_pdf_nov01gen3_161002.root");

    // try to renormalize the pol2 distributions
    getAndRenormalize(1);
    setPdf(1, pMomVsBeta);

    new TFile("pidpar_pdf_tofino_nov01gen3_181102.root");
    getAndRenormalize(0);
    setPdf(0, pMomVsBeta);

    pMomVsBeta->print();

    // -------------------------------------------------------------------------
    // HPidTrackFillerPar
HPidTrackFillerPar *pFillerPar
        = (HPidTrackFillerPar *) rtdb->getContainer("PidTrackFillerPar");

    pFillerPar->setTofinoMult(1);
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
Int_t    iX, iY, iXMax, iYMax, i;
Stat_t   aVal[3];
Double_t dD, dX1, dX2;

    for(iX = 1; iX <= hPol[0]->GetNbinsX(); iX++)
    {
        for(iY = 1; iY <= hPol[0]->GetNbinsY(); iY++)
        {
            for(i = 0; i < 3; i++)
                aVal[i] = hPol[i]->GetBinContent(hPol[i]->GetBin(iX, iY));

            // check parameters (aVal[2] MUST BE < 0)
            if(aVal[2] >= 0.0)
            {
                for(i = 0; i < 3; i++)
                    aVal[i] = 0.0;
            }
            else
            {
                // find pol2(X) = 0
                dD = aVal[1] * aVal[1] - 4.0 * aVal[0] * aVal[2];
                if(dD <= 0.0)
                {
                    for(i = 0; i < 3; i++)
                        aVal[i] = 0.0;
                }
                else
                {
                    dD = sqrt(dD);
                    dX1 = (-aVal[1] - dD) / (2.0 * aVal[2]);
                    dX2 = (-aVal[1] + dD) / (2.0 * aVal[2]);
                    // swap to make dX1 < dX2
                    if(dX1 > dX2)
                    {
                        dD = dX2; dX2 = dX1; dX1 = dD;
                    }

                    // calculate integral
                    dD = aVal[0] * (dX2 - dX1)
                        + aVal[1] * (dX2 * dX2 - dX1 * dX1) / 2.0
                        + aVal[2] * (dX2 * dX2 * dX2 - dX1 * dX1 * dX1) / 3.0;

                    // imposible, but ....
                    if(dD <= 0)
                    {
                        for(i = 0; i < 3; i++)
                            aVal[i] = 0.0;
                    }
                    else
                    {
                        for(i = 0; i < 3; i++)
                            aVal[i] /= dD;
                    }
                }
            }

            for(i = 0; i < 3; i++)
                hPol[i]->SetBinContent(hPol[i]->GetBin(iX, iY), aVal[i]);
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
        sprintf(s, "h_relint_s%d_%d", iSys, aIds[i]);
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
        for(par = 0; par < ((aIds[i] < 500) ? 2 : 3); par++)
        {
            sprintf(s, "h_pdf_s%d_%d_%d", iSys, aIds[i],
                        (aIds[i] < 500) ? (par + 1) : par);

            if((pH = (TH1 *) gDirectory->Get(s)) == NULL)
            {
                printf("No histogram: %s !!!\n", s);
                continue;
            }

            pMomVsBeta->addHistogram(aIds[i], 3 * iSys + par, pH);
        }
    }
}
