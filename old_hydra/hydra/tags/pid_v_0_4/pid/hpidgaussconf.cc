// @(#)$Id: hpidgaussconf.cc,v 1.1 2002-10-30 10:55:33 halo Exp $
//*-- Author : Marcin Jaskula 25/10/2002

////////////////////////////////////////////////////////////////////////////////
//
// HPidGaussConf
//
// A class for calculating confidence level from gauss distribution
//
////////////////////////////////////////////////////////////////////////////////

#include "hpidgaussconf.h"

#include <TError.h>
#include <TH1.h>
#include <TMath.h>

// -----------------------------------------------------------------------------

HPidGaussConf gPidGaussConf;

// -----------------------------------------------------------------------------

HPidGaussConf::HPidGaussConf(Int_t iBins, Double_t dMin)
{
// Constructor, which makes the histogram
// iBins - number of beans in range (dMin, 0)
// dMin  - minimum value for histogram (MUST BE NEGATIVE)

    if(iBins < 1)
    {
        ::Warning("HPidGaussConf::HPidGaussConf", "iBins < 1, set it to 1001");
        iBins = 1001;
    }

    if(dMin >= 0.0)
    {
        ::Warning("HPidGaussConf::HPidGaussConf",
                "dMin >= 0.0, set it to -5.0");

        dMin = -5.0;
    }

TH1D     hGaussPDF("gaussPDF", "Gauss(0,1) pdf", iBins, dMin, 0.0);
Int_t    i;
Double_t dX;

    if((pDistr = new TH1D("gaussDistr", "Gauss(0,1) distribution",
                        iBins, dMin, 0.0)) == NULL)
    {
        ::Error("HPidGaussConf::HPidGaussConf", "Cannot create histogram");
        return;
    }

    pDistr->SetDirectory(NULL);

    for(i = 1; i <= iBins; i++)
    {
        dX = hGaussPDF.GetXaxis()->GetBinCenter(i);
        hGaussPDF.SetBinContent(i, TMath::Exp(-0.5 * dX * dX));
    }

    for(i = 0; i <= iBins; i++)
        pDistr->SetBinContent(i, hGaussPDF.Integral(0, i));

    pDistr->Scale(0.5 / pDistr->GetMaximum());
}

// -----------------------------------------------------------------------------

HPidGaussConf::~HPidGaussConf(void)
{
// Delete the histogram

    if(pDistr != NULL)
        delete pDistr;
}

// -----------------------------------------------------------------------------

Double_t HPidGaussConf::getConfLevel(Double_t dX)
{
// Static function which returns confidence level for normal gauss distr (0,1).
// It uses gPidGaussConf instance of the class

    return gPidGaussConf.getConfLevelP(dX);
}

// -----------------------------------------------------------------------------

Double_t HPidGaussConf::getConfLevel(Double_t dMean, Double_t dSigma,
                Double_t dX)
{
// Static function which returns  confidence level for any gauss distr (M,S)
// dSigma MUST BE > 0
// otherwise -1 is returned
// The method uses gPidGaussConf instance of the class

    if(dSigma <= 0.0)
    {
        ::Error("HPidGaussConf::getConfLevel", "sigma <= 0.0");
        return -1.0;
    }

    return gPidGaussConf.getConfLevelP((dX - dMean) / dSigma);
}

// -----------------------------------------------------------------------------

void HPidGaussConf::draw(void)
{
// Static function for drawing conf. histogram.
// It uses gPidGaussConf instance of the class

    gPidGaussConf.drawP();
}

// -----------------------------------------------------------------------------

Double_t HPidGaussConf::getConfLevelP(Double_t dX)
{
// Function for calculating the confidance level using histogram

    if(dX == 0.0)
        return 0.0;

    if(dX > 0.0)
        dX = -dX;

    if(pDistr == NULL)
    {
        ::Error("HPidGaussConf::getConfLevelP", "no histogram");
        return -1.0;
    }

    return (1.0 - 2.0 * pDistr->GetBinContent(pDistr->FindBin(dX)));
}

// -----------------------------------------------------------------------------

void HPidGaussConf::drawP(void)
{
// Method for drawing conf. histogram

    if(pDistr == NULL)
        ::Error("HPidGaussConf::drawP", "no histogram");
    else
        pDistr->Draw();
}
