//@(#)
// Draw mom*q vs. experimental beta for all identified particles
// Use it when the file with T tree is loaded.
// like: root particles.root draw3.C
{
Int_t aParts[] = { 2, 3, 8, 9, 14, 45, -2, -1, -4, -3 };
const char *sSys[] = { "Shower", "TOF" };
Int_t iMax = sizeof(aParts) / sizeof(Int_t);
Int_t iCharge, i, k;
char  sName[1000];
char  sTitle[1000];
char  sDraw[1000];
TH3F *h;
TH1  *aH[2][iMax];
Int_t iSys;
TLegend *l[2];
TLegendEntry *e;
int iSysMin = 0;
int iSysMax = 1;

    gStyle->SetOptTitle(0);
    gStyle->SetOptStat(0);
    gStyle->SetPadTopMargin(0.03);
    gStyle->SetPadRightMargin(0.03);
    gStyle->SetCanvasColor(10);
    gStyle->SetPadColor(10);

    h = new TH3F("h3", "temp hist", 40, -4, 36, 100, 0.0, 2.0,
                    300, -1500, 1500);

    strcpy(sDraw, "HPidParticleHit.fData.P()*HPidParticleHit.fData.charge()"
    ":HPidParticleHit.fData.getBetaExp()"
    ":((HPidParticleHit.fData.nPid%20)+20*HPidParticleHit.fData.getSystem())"
    ">>h3");

    printf("%s\n", sDraw);

    T.Draw(sDraw);

    for(iSys = iSysMin; iSys <= iSysMax; iSys++)
    {
        l[iSys] = new TLegend(0.7, 0.15, 0.985, 0.6, sSys[iSys]);

        for(i = 0; i < iMax; i++)
        {
            k = (aParts[i] % 20) + (20 * iSys);
            h3->GetXaxis()->SetRange(k + 1 + 4, k + 1 + 4);
            aH[iSys][i] = h3->Project3D("zy");

            sprintf(sName, "hPid_%d_%d", iSys, aParts[i]);
            sprintf(sTitle, "q*p vs beta: %s %s",
                        HPidPhysicsConstants::pid(aParts[i]), sSys[iSys]);

            aH[iSys][i]->SetMarkerColor(i + 1);
            aH[iSys][i]->SetLineColor(i + 1);

            aH[iSys][i]->SetName(sName);
            aH[iSys][i]->SetTitle(sTitle);

            printf("%d / %d : %s %p\n", i + 1, iMax, sTitle, aH[iSys][i]);

            iCharge = aH[iSys][i]->Integral();

            printf("Entries: %d\n", iCharge);

            sprintf(sName, "%s %d\n", HPidPhysicsConstants::pid(aParts[i]),
                        iCharge);

            e = l[iSys]->AddEntry(aH[iSys][i], sName, "l");
            e->SetMarkerSize(6);
            e->SetMarkerStyle(8);
        }
    }

    for(iSys = iSysMin; iSys <= iSysMax; iSys++)
    {
        if(iSys != iSysMin)
        {
        TCanvas *c = new TCanvas;

            c->cd();
        }

        for(i = 0; i < iMax; i++)
            aH[iSys][i]->Draw((i == 0) ? "" : "same");

        l[iSys]->Draw();
    }

    printf("End\n");
}
