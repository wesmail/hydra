// @(#)$Id: hpidgaussconf.h,v 1.3 2003-11-21 14:41:21 halo Exp $
//*-- Author : Marcin Jaskula 25/10/2002
//  Modified : Marcin Jaskula 15/11/2002
//             - use TMath::Erf insted of the histogram

#ifndef HPIDGAUSSCONF_H
#define HPIDGAUSSCONF_H

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// HPidGaussConf                                                        //
//                                                                      //
// A class for calculating confidence level from gauss distribution     //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <TROOT.h>
#include <TMath.h>
#include <TError.h>

// -----------------------------------------------------------------------------

class HPidGaussConf
{
public:
    inline static Double_t getConfLevel(Double_t dX);
    inline static Double_t getConfLevel(Double_t dMean, Double_t dSigma, Double_t dX);

private:

    static Double_t dDiv_Sqrt_2;
};

// -----------------------------------------------------------------------------

inline Double_t HPidGaussConf::getConfLevel(Double_t dX)
{
// Get CL defined as Integral(-dX, dX) gauss(0, 1)

    return TMath::Abs(TMath::Erf(dX * dDiv_Sqrt_2));
}

// -----------------------------------------------------------------------------

inline Double_t HPidGaussConf::getConfLevel(
                Double_t dMean, Double_t dSigma, Double_t dX)
{
// Static function which returns  confidence level for any gauss distr (M,S)
// dSigma MUST BE > 0 otherwise -1 is returned

    if(dSigma <= 0.0)
    {
        ::Error("HPidGaussConf::getConfLevel", "sigma <= 0.0");
        return -1.0;
    }

    return getConfLevel((dX - dMean) / dSigma);
}

// -----------------------------------------------------------------------------

#endif //HPIDGAUSSCONF_H
