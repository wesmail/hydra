// @(#)$Id: hpidgaussconf.h,v 1.1 2002-10-30 10:55:33 halo Exp $
//*-- Author : Marcin Jaskula 25/10/2002

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

// -----------------------------------------------------------------------------

class TH1D;

// -----------------------------------------------------------------------------

class HPidGaussConf
{
public:
                    HPidGaussConf(Int_t iBins = 1001, Double_t dMin  = -5.0);
                   ~HPidGaussConf(void);


    static Double_t getConfLevel(Double_t dX);
    static Double_t getConfLevel(Double_t dMean, Double_t dSigma, Double_t dX);

    static void     draw(void);

    Double_t        getConfLevelP(Double_t dX);
    void            drawP(void);

private:
    TH1D           *pDistr;
};

// -----------------------------------------------------------------------------

#endif //HPIDGAUSSCONF_H
