// @(#)$Id: hpidgaussconf.cc,v 1.4 2006-08-12 12:49:35 halo Exp $
//*-- Author : Marcin Jaskula 25/10/2002
//  Modified : Marcin Jaskula 15/11/2002
//             - use TMath::Erf insted of the histogram

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////////////
//
// HPidGaussConf
//
// A class for calculating confidence level from gauss distribution
//
////////////////////////////////////////////////////////////////////////////////

#include "hpidgaussconf.h"

// -----------------------------------------------------------------------------

ClassImp(HPidGaussConf)

// -----------------------------------------------------------------------------

Double_t HPidGaussConf::dDiv_Sqrt_2 = 1.0 / TMath::Sqrt(2.0);
