{
// @(#)$Id: changeFakes.C,v 1.1 2003-03-05 11:24:39 jaskula Exp $
// Fast changing of the fakes parameters in Pid container
TFile              *pFile = new TFile("params.root", "UPDATE");
HPidHistogramsCont *c;
int                 i, k;
TH1                *h;

    if((c = (HPidHistogramsCont*) pFile->Get("PidAlgMomVsBetaPar")) == NULL)
    {
        ::Error("", "Cannot get PidAlgMomVsBetaPar");
    }
    else
    {
        for(i = 500; i <= 501; i++)
        {
            for(k = 0; k < 6; k++)
            {
                h = c->getHistogram(i, k);
                c->addHistogram(499 - i, k, h);
                c->removeHistogram(i, k);
            }
        }

        pFile->Delete("PidAlgMomVsBetaPar;*");
        c->Write();
    }

    if((c = (HPidHistogramsCont*) pFile->Get("PidReconstructorPar")) == NULL)
    {
        ::Error("", "Cannot get PidReconstructorPar");
    }
    else
    {
        for(i = 500; i <= 501; i++)
        {
            for(k = 0; k < 2; k++)
            {
                h = c->getHistogram(i, k);
                c->addHistogram(499 - i, k, h);
                c->removeHistogram(i, k);
            }
        }

        pFile->Delete("PidReconstructorPar;*");
        c->Write();
    }

    pFile->Close();
}
