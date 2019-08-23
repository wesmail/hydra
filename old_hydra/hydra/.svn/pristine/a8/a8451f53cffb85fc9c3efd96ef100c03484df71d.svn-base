#ifndef MPTOOLS__H
#define MPTOOLS__H

////////////////////////////////////////////////////////////////////////////////
//
// MPTools
// Maths:Peter's Tools
// 
//            Author: Peter W.Zumbruch
//           Contact: P.Zumbruch@gsi.de
//           Created: Mar 18, 2004
// Last modification: May 24, 2004
// 
////////////////////////////////////////////////////////////////////////////////

#include "hgeomvector.h"

class MPTools 
{
private:

 public:
  static Double_t        getDistancePointToStraight(HGeomVector &point, HGeomVector &base, HGeomVector &direction);
  static TH1*            calculateLikeSignCombinatorialBackground(TH1* pp, TH1* mm, TString name="background");
  static Double_t        poissonDistribution(double *x, double *par);
  static Double_t        poissonDistribution(Double_t x, Double_t mean, Double_t amplitude=0.);
  static Double_t        binomialDistribution(double *x, double *par);
  static Double_t        binomialDistribution(Double_t x, Double_t singleProbability, Double_t N, Double_t amplitude=0.);
  static Double_t        chiSquareDistribution(double *x, double *par);
  static Double_t        chiSquareDistribution(Double_t x, Double_t ndf, Double_t amplitude=0. );
  static Double_t        calcCMMomentumOfPairDecay(Double_t minv, Double_t m1, Double_t m2, 
						   Double_t minvErr, Double_t m1Err, Double_t m2Err, 
						   Double_t &err);
  static Double_t        calcCMMomentumOfPairDecay(Double_t minv, Double_t m1, Double_t m2);
  static Double_t        calcCMEnergyOfPairDecay(Double_t minv, Double_t m1, Double_t m2);
  //  static Double_t        calcArmenterosPt(Double_t alpha, Double_t minv, Double_t m1, Double_t m2);
};

#endif
