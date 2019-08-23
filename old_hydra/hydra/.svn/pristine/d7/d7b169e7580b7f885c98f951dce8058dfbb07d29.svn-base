////////////////////////////////////////////////////////////////////////////////
//
// Peter's Math Tools
// 
//            Author: Peter W. Zumbruch
//           Contact: P.Zumbruch@gsi.de
//           Created: March 18, 2004
// 
// File: $RCSfile: mptools.cc,v $ 
// Version: $Revision: 1.10 $
// Modified by $Author: zumbruch $ on $Date: 2004-08-15 20:01:04 $  
////////////////////////////////////////////////////////////////////////////////

using namespace std;

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>

#include "TApplication.h"
#include "TCanvas.h"
#include "TError.h"
#include "TFile.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "THStack.h"
#include "TLatex.h"
#include "TMath.h"
#include "TObject.h"
#include "TPaveText.h"
#include "TPad.h"
#include "TROOT.h"
#include "TStopwatch.h"
#include "TSystem.h"
#include "TString.h"
#include "TStyle.h"
#include "TTree.h"
#include "TUnixSystem.h"

#include "ptools.h"
#include "mptools.h"

ClassImp(MPTools)

////////////////////////////////////////////////////////////////////////////////

// --------------------------------------------------------------------------------

Double_t MPTools::getDistancePointToStraight(HGeomVector &point, HGeomVector &base, HGeomVector &direction)
{
  if (direction.length() > 0)
    {
      direction*=1/direction.length();
    }
  else
    {
      return -1.;
    }

    Double_t val = 
      pow(point(0)-base(0),2)+
      pow(point(1)-base(1),2)+
      pow(point(2)-base(2),2)
      - (
	 pow(
	     (point(0)-base(0))*direction(0)+
	     (point(1)-base(1))*direction(1)+
	     (point(2)-base(2))*direction(2)
	     ,2)
	 );

  if (val<0.)
    {
      return -1.;
    }
  return sqrt(val);

}

// --------------------------------------------------------------------------------

TH1* MPTools::calculateLikeSignCombinatorialBackground(TH1* pp, TH1* mm, TString name)
{
  // calculates binwise the likesign combinatorial background of two given histograms 
  // using the formula
  //
  //              /---------
  // back = 2 *  V pp * mm
  //         
  //
  // and returns the pointer to the background histogramm
  // with the name assigned assigned by name [default: "background"]
  // 
  // if pp or mm are null pointers NULL is returned 
  // if pp and mm are not compatible NULL is returned 
  
  // check for not NULL pointer
  if (!pp || !mm)
    {
      Error("calculateLikeSignCombinatorialBackground","can't calculate, ++ (%p) or -- (%p) is null pointer, returning NULL",pp, mm);
      return NULL;
    }

  // check for compatibility
  if (! PTools::areHistogramsCompatible(pp,mm))
    {
      Warning("calculateLikeSignCombinatorialBackground","can't calculate, hists ++ and -- are not compatible, return NULL!");
      return NULL;
    }

  // create hist error structure
  if (! pp->GetSumw2N()) pp->Sumw2();
  if (! mm->GetSumw2N()) mm->Sumw2();

  // Clone 
  TH1 *background = (TH1*) pp->Clone(name.Data());
  background->Reset();

  // 2 * sqrt ( pp * mm );
  background->Multiply(pp,mm);
  PTools::sqrt(background);
  background->Scale(2.);
  
  return background;
} 

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------

TH1* MPTools::calculateLikeSignCombinatorialBackground(TH1* pp, TH1* mm, TH1* correction, TString name)
{
  // calculates binwise the likesign combinatorial background of two given histograms 
  // using the formula
  //
  //                          /---------
  // back = 2 * correction * V pp * mm
  //         
  //
  // and returns the pointer to the background histogramm
  // with the name assigned assigned by name [default: "background"]
  // 
  // correction is also a histogram containing bin-wise correction factors (like acceptance correction)
  //
  // if correction, pp or mm are null pointers NULL is returned 
  // if correction, pp and mm are not compatible NULL is returned 
  
  // check for not NULL pointer
  if (!correction)
    {
      Error("calculateLikeSignCombinatorialBackground","can't calculate, correction (%p) is null pointer, returning NULL",correction);
      return NULL;
    }

  // check for compatibility
  if (! PTools::areHistogramsCompatible(pp,mm))
    {
      Warning("calculateLikeSignCombinatorialBackground","can't calculate, hists ++ and -- are not compatible, return NULL!");
      return NULL;
    }

  if (! PTools::areHistogramsCompatible(correction,mm))
    {
      Warning("calculateLikeSignCombinatorialBackground","can't calculate, hists correction and --/++ are not compatible, return NULL!");
      return NULL;
    }
  
  // 2 * sqrt ( pp * mm );
  TH1* background = calculateLikeSignCombinatorialBackground(pp, mm, name);

  // correction 2 * sqrt ( pp * mm );
  background->Multiply(correction);

  return background;
} 

// --------------------------------------------------------------------------------

Double_t MPTools::poissonDistribution(double *x, double *par)
{ 
  // discrete poisson distribution as continuous function
  // poisson is the limiting form of the binomial distribution 
  // for p->0 and N->infinity
  //
  //             r  -mean
  //         mean  e    
  // P(r) = ------------- 
  //             r!
  //
  // translates to 
  //
  //                          r  -mean
  //          amplitude   mean  e     
  // P(x) = 10           ---------------
  //                        Gamma(r+1)
  //
  // if x<0 -1 is returned
  // 
  // par[0] : amplitude
  // par[1] : mean
  //
  Double_t ampl = par[0];
  Double_t mean = par[1];
  Double_t xx   =   x[0];

  if (xx <= 0.) { return -1; }

  Double_t denom = TMath::Gamma(xx+1);
  
  if (!TMath::Finite(denom))
    {
      Warning( "MPTools::binomialDistribution","cannot handle such large numbers Gamma(%f) infinite.. returning -10",xx);
      return -10;
    }

  Double_t res = ((pow(10,ampl) * (pow(mean,xx)) * exp(-mean)) /  denom);

  return res;
}

Double_t MPTools::poissonDistribution(Double_t x, Double_t mean, Double_t amplitude)
{ 
  // discrete poisson distribution as continuous function
  // poisson is the limiting form of the binomial distribution 
  // for p->0 and N->infinity
  //
  //             r  -mean
  //         mean  e    
  // P(r) = ------------- 
  //             r!
  //
  // translates to 
  //
  //                          r  -mean
  //          amplitude   mean  e     
  // P(x) = 10           ---------------
  //                        Gamma(r+1)
  //
  // if x<0 -1 is returned
  
  Double_t xx[1];
  Double_t par[3];
  
  xx[0] = x;
  par[0] = amplitude;
  par[1] = mean;

  return poissonDistribution(xx,par);
}
// --------------------------------------------------------------------------------

Double_t MPTools::binomialDistribution(double *x, double *par)
{
  // discrete binomial distribution as continuous function
  // 
  //              N!       r       N-r
  // P(r) = ------------- p (1 - p)
  //         r! (N - r) !
  //
  // translates to 
  //
  //          Amplitude           Gamma(N+1)           r       N-r
  // P(x) = 10           ---------------------------- p (1 - p)
  //                     Gamma(r+1) Gamma (N - r + 1)
  //
  // P(x) the probability of r successes in N tries
  // p is the single probability 
  //
  // par[0]: Amplitude, chosen for Normalization purposes 
  // par[1]: number of tries (N)
  // par[2]: single probability (p)
  //
  // if x<0 -1 is returned
  // 
  // if x>N -1 is returned

  Double_t ampl              = par[0];
  Double_t N                 = par[1];
  Double_t singleProbability = par[2];
  Double_t xx                =   x[0];

  if (xx > N) return -1.;

  if ((xx+1) > 150)
    {
      Warning( "MPTools::binomialDistribution","cannot handle such large numbers x(%d) > 150).. returning -10",xx);
      return -10;
    }
  if ((N) > 150)
    {
      Warning( "MPTools::binomialDistribution","cannot handle such large numbers Number of tries (%d) > 150).. returning -10",N);
      return -10;
    }
  if ((N-xx+1) > 150)
    {
      Warning( "MPTools::binomialDistribution","cannot handle such large numbers .. returning -10");
      return -10;
    }

  Double_t denom = TMath::Gamma(xx+1)*TMath::Gamma(N-xx+1);

  Double_t nom = pow(10,ampl) * TMath::Gamma(N+1) * pow(singleProbability,xx) * pow(1-singleProbability,N-xx);

  return nom/denom;
  
}


// --------------------------------------------------------------------------------

Double_t MPTools::binomialDistribution(Double_t x, Double_t singleProbability, Double_t N, Double_t amplitude)
{
  // discrete binomial distribution as continuous function
  // 
  //              N!       r       N-r
  // P(r) = ------------- p (1 - p)
  //         r! (N - r) !
  //
  // translates to 
  //
  //          Amplitude           Gamma(N+1)           r       N-r
  // P(x) = 10           ---------------------------- p (1 - p)
  //                     Gamma(r+1) Gamma (N - r + 1)
  //
  // P(x) the probability of r successes in N tries
  // p is the single probability 
  //
  // par[0]: Amplitude, chosen for Normalization purposes 
  // par[1]: number of tries (N)
  // par[2]: single probability (p)
  // if x<0 -1 is returned
  // 
  // if x>N -1 is returned

  Double_t xx[1];
  Double_t par[3];
  
  xx[0] = x;
  par[0] = amplitude;
  par[1] = N;
  par[2] = singleProbability;
  
  return binomialDistribution(xx, par);
}

// --------------------------------------------------------------------------------

Double_t MPTools::chiSquareDistribution(double *x, double *par)
{
  // chi square distriubtion
  //
  //                       (0.5 * NDF) -1     (- 0.5 * chi2)
  //           (0.5 * chi2)               * e
  // P(chi2) = ----------------------------------------------
  //                         2 * Gamma (0.5 * NDF)
  // 
  // adding a normalization amplitude 
  // 
  //                                    (0.5 * NDF) -1     (- 0.5 * x[0])
  //             amplitude   (0.5 * x[0])               * e
  // P(x[0]) = 10         *  ------------------------------------------------
  //                                       2 * Gamma (0.5 * NDF)
  //
  // 
  // par[1]: NDF are number of degrees of freedom
  // par[0]: amplitude
  //
  // if ndf <=0 -1 is returned

  Double_t amplitude = par[0];
  Double_t ndf = par[1];
  Double_t xx = x[0];


  if (ndf<=0) return -1;
  
  Double_t denom = 2 * TMath::Gamma( 0.5 * ndf);
  
  Double_t nom = pow (10, amplitude) * pow(0.5 * xx, 0.5*ndf -1) * exp(-0.5*xx);

  return nom/denom;
  
}

// --------------------------------------------------------------------------------

Double_t MPTools::chiSquareDistribution(Double_t x, Double_t ndf, Double_t amplitude)
{
  // chi square distriubtion
  //
  //                       (0.5 * NDF) -1     (- 0.5 * chi2)
  //           (0.5 * chi2)               * e
  // P(chi2) = ----------------------------------------------
  //                         2 * Gamma (0.5 * NDF)
  // 
  // adding a normalization amplitude 
  // 
  //                                 (0.5 * NDF) -1     (- 0.5 * x)
  //          amplitude   (0.5 * x)               * e
  // P(x) = 10         *  ------------------------------------------------
  //                                    2 * Gamma (0.5 * NDF)
  //
  // 
  // NDF are number of degrees of freedom
  // amplitude
  //
  // if ndf <=0 -1 is returned

  Double_t xx[1];
  Double_t par[2];
  
  xx[0] = x;
  par[0] = amplitude;
  par[1] = ndf;
  
  return chiSquareDistribution(xx, par);
}

// --------------------------------------------------------------------------------

Double_t MPTools::calcCMMomentumOfPairDecay(Double_t minv, Double_t m1, Double_t m2, 
					    Double_t minvErr, Double_t m1Err, Double_t m2Err, 
					    Double_t &err)
{
  // calculates in a 2-body decay the center of momentum momentum of the decay particles
  // where 
  //    minv is the invariant mass of the parent
  //    m1 is the mass of the particle 1
  //    m2 is the mass of the particle 2
  //    minvErr is the absolute error parents invariant mass
  //    m1Err is absolute error of the mass of the particle 1
  //    m2Err is absolute error of the mass of the particle 2
  // 
  //    the error is returned via err
  //    in case of errors -1. is returned
  //
  // !!!! in case of m1!=m2 I am not quite sure if the result is ok. !!!


  if (m1Err < 0. || m2Err < 0 || minvErr < 0)
    {
      ::Error("calcCMMomentumOfPairDecay",
	      "one or more input error(s) are negativ: minvErr: %f , m1Err: %f, m2Err: %f .. return: -1",
	      minvErr, m1Err, m2Err);
      return -1.;
    }
  
  Double_t p = calcCMMomentumOfPairDecay(minv, m1, m2);
  
  if (p<=0) {return -1;}
  
  Double_t A=0;

  A=(m1*m1 - m2*m2)/(minv*minv);

  if (m1 != m2)
    {
      A=(m1*m1 - m2*m2)/(minv*minv);
    }
  
  err = 
    pow((minv * minvErr)/2 * (1- pow( A ,2)) ,2) +
    pow(m1Err * m1 *( A-1) ,2) +
    pow(m2Err * m2 *(-A-1) ,2);

  if (err<0)
    {
      ::Warning("calcCMMomentumOfPairDecay",
	       "rounding problems? while calculating error: negative square .. return err: -1");
      err = -1.;
    }
  else
    {
      err = 1/(2*p) * sqrt (err);
    } 
  
  return p;

}

// --------------------------------------------------------------------------------

Double_t MPTools::calcCMMomentumOfPairDecay(Double_t minv, Double_t m1, Double_t m2)
{
  // calculates in a 2-body decay the center of momentum momentum of the decay particles
  // where 
  //    minv is the invariant mass of the parent
  //    m1 is the mass of the particle 1
  //    m2 is the mass of the particle 2
  // 
  //    the error is returned via err
  //    in case of errors -1. is returned

  if ((m1+m2)>minv)
    {
      ::Error("calcCMMomentumOfPairDecay",
	      "minv is smaller than m1 + m2 : minv: %f , m1: %f, m2: %f .. return: -1",minv, m1, m2);
      return -1.;
    }

  if (minv == 0)
    {
      ::Error("calcCMMomentumOfPairDecay",
	      "minv is zero: minv: %f .. return: -1",minv);
      return -1.;
    }
  if (m1 < 0. || m2 < 0 || minv < 0)
    {
      ::Error("calcCMMomentumOfPairDecay",
	      "one or more input(s) are negativ: minv: %f , m1: %f, m2: %f .. return: -1",minv, m1, m2);
      return -1.;
    }

  Double_t p2;
  if (m1 != m2)
    {
      p2 = pow(minv/2,2) - 0.5 * ( m1*m1 + m2*m2 ) + pow( (m1*m1 - m2*m2)/(2*minv) , 2);
    }
  else
    {
      p2 = pow(minv/2,2) - 0.5 * ( m1*m1 + m2*m2 );
    } 

  if (p2 < 0)
    {
      ::Error("calcCMMomentumOfPairDecay",
	      "rounding problems?: negative square .. return: -1");
      return -1.;
    }
  else
    {
      return sqrt(p2);
    }
}

// --------------------------------------------------------------------------------

Double_t MPTools::calcCMEnergyOfPairDecay(Double_t minv, Double_t m1, Double_t m2)
{
  // calculates in a 2-body decay the center of momentum energy of the decay particles
  // of particle with mass m1
  // where 
  //    minv is the invariant mass of the parent
  //    m1 is the mass of the particle 1
  //    m2 is the mass of the particle 2
  // 
  //    in case of errors -1. is returned

  if ((m1+m2)>minv)
    {
      ::Error("calcCMEnergyOfPairDecay",
	      "minv is smaller than m1 + m2 : minv: %f , m1: %f, m2: %f .. return: -1",minv, m1, m2);
      return -1.;
    }

  if (minv == 0)
    {
      ::Error("calcCMEnergyOfPairDecay",
	      "minv is zero: minv: %f .. return: -1",minv);
      return -1.;
    }
  if (m1 < 0. || m2 < 0 || minv < 0)
    {
      ::Error("calcCMEnergyOfPairDecay",
	      "one or more input(s) are negativ: minv: %f , m1: %f, m2: %f .. return: -1",minv, m1, m2);
      return -1.;
    }

  Double_t p = calcCMMomentumOfPairDecay(minv, m1, m2);
  if (p<=0) {return -1;}

  return sqrt(m1*m1 + p*p);
}

// // --------------------------------------------------------------------------------

// Double_t MPTools::calcArmenterosPt(Double_t alpha, Double_t minv, Double_t m1, Double_t m2)
// {
//   // calculates in the armenteros p_t for a given armenteros alpha
//   // where 
//   //    minv is the invariant mass of the parent
//   //    m1 is the mass of the positive particle 1 
//   //    m2 is the mass of the negative particle 2
//   // 
//   //    in case of errors -1. is returned

//   Double_t Ep =   calcCMEnergyOfPairDecay(minv, m1, m2);
//   Double_t Em =   calcCMEnergyOfPairDecay(minv, m2, m1);
//   Double_t p  = calcCMMomentumOfPairDecay(minv, m1, m2);
  
//   if (Ep < 0. || Em < 0 || p < 0)
//     {
//       return -1.;
//     }

//   Double_t alphaE = (Ep-Em)/minv;
//   Double_t beta = p/minv;

//   Double_t pt2 = p*p - pow( alpha - alphaE ,2)/pow( 2 * beta ,2);

//   return p - pow( alpha - alphaE ,2)/pow( 2 * beta ,2);

//   if (pt2<0)
//     {
//       ::Error("calcArmenterosPt",
// 	      "rounding problems?: negative square .. return: -1");
//       return -1.;
//     }
//   else
//     {
//       return sqrt(pt2);
//     }
// }


