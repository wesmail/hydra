//@(#)$Id: saveHPidReconstructorPar.C,v 1.1 2002-10-30 10:55:33 halo Exp $
//The macro reads histogram from files, puts them into containers
//and stores in the RTDB
void saveHPidReconstructorPar(void)
{
Int_t iRunIds[] = { 1, 990456436, 990420780, 1007598949 };
char  sOutputFile[] = "pidparams.root";

Hades myHades;
HRuntimeDb *rtdb = gHades->getRuntimeDb();

    // -------------------------------------------------------------------------
    // copy histograms from Pavel's files
Int_t  aIds[] = { 2, 3, 8, 9, 14, 45, 500, 501 };
Int_t  i, sys, par;
char   s[100];
TH1   *pH;

    // -------------------------------------------------------------------------
    // HPidReconstructorPar
HPidReconstructorPar *pRecPar
        = (HPidReconstructorPar *) rtdb->getContainer("PidReconstructorPar");

    new TFile("Pavel_pidpar0.root");

    for(sys = 0; sys <= 1; sys++)
    {
        for(i = 0; i < sizeof(aIds) / sizeof(Int_t); i++)
        {
            sprintf(s, "h_relint_s%d_%d", 1 /*sys*/, aIds[i]);
            if((pH = (TH1 *) gDirectory->Get(s)) == NULL)
            {
                printf("No histogram: %s !!!\n", s);
                continue;
            }

            pRecPar->addHistogram(aIds[i], sys, pH);
        }
    }

    pRecPar->print();

    // -------------------------------------------------------------------------
    // HPidAlgMomVsBetaPar
HPidAlgMomVsBetaPar *pMomVsBeta
        = (HPidAlgMomVsBetaPar *) rtdb->getContainer("PidAlgMomVsBetaPar");

    new TFile("pidpar_pdf_nov01gen3_161002.root");

    // try to renormalize the pol2 distributions
TH1 *hPol[3];
Bool_t bOk;

    for(sys = 0; sys <= 1; sys++)
    {
        for(i = 500; i <= 501; i++)
        {
            bOk = kTRUE;
            for(par = 0; par < 3; par++)
            {
                sprintf(s, "h_pdf_s%d_%d_%d", 1 /*sys*/, i, par);

                if((hPol[par] = (TH1 *) gDirectory->Get(s)) != NULL)
                    continue;

                printf("No histogram: %s !!!\n", s);

                bOk = kFALSE;
            }

            if( ! bOk)
            {
                printf("No all histograms: cannot reanormalize\n");
                continue;
            }

            renormalize(hPol);
        }
    }

    for(sys = 0; sys <= 1; sys++)
    {
        for(i = 0; i < sizeof(aIds) / sizeof(Int_t); i++)
        {
            for(par = 0; par < ((aIds[i] < 500) ? 2 : 3); par++)
            {
                sprintf(s, "h_pdf_s%d_%d_%d", 1 /*sys*/, aIds[i],
                            (aIds[i] < 500) ? (par + 1) : par);

                if((pH = (TH1 *) gDirectory->Get(s)) == NULL)
                {
                    printf("No histogram: %s !!!\n", s);
                    continue;
                }

                pMomVsBeta->addHistogram(aIds[i], 3 * sys + par, pH);
            }
        }
    }

    pMomVsBeta->print();

    // -------------------------------------------------------------------------
    // HPidTrackFillerPar
HPidTrackFillerPar *pFillerPar
        = (HPidTrackFillerPar *) rtdb->getContainer("PidTrackFillerPar");

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
