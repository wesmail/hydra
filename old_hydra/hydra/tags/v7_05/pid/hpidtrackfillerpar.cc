// @(#)$Id: hpidtrackfillerpar.cc,v 1.11 2005-02-16 10:23:55 christ Exp $
//*-- Author : Marcin Jaskula 09/10/2002
//*-- Modified: Marcin Jaskula and Jacek Otwinowski 05/02/2003
//           - iTofinoMult removed
//*-- Modified: Marcin Jaskula 14/03/2003
//           - meta overlap added

////////////////////////////////////////////////////////////////////////////////
//
// HPidTrackFillerPar
//
// HPidFrackFiller parameters for correlations
//
////////////////////////////////////////////////////////////////////////////////

using namespace std;

#include "hpidtrackfillerpar.h"
#include <iostream>

// -----------------------------------------------------------------------------

ClassImp(HPidTrackFillerPar)

// -----------------------------------------------------------------------------
//This parameter container has 30 histograms (5 per sector). They are arranged in the following way:
//There are only the histograms for one species filled (PID==0). The container is NOT
//particle dependent!
//The histograms contain (for PID==0) the following data:
//offset=0: lower limits for delta phi rich/mdc
//offset=1: uppeer limits for delta phi rich/mdc
//offset=2: lower limits for delta theta rich/mdc
//offset=3: upper limits for delta theta rich/mdc
//offset=4: 2D-histogram containing the coefficients for the polynomial correction of the
//rich ring coordinate for matching with MDC (first dimension: z-coordinate, second dimension: order
//of the coefficient

HPidTrackFillerPar::HPidTrackFillerPar(const char *name, const char *title,
                                const char *context)
                                : HPidHistogramsCont(name, title, context,5)
{
  // constructor
}

Float_t HPidTrackFillerPar::getDeltaPhiMin(Int_t sector, Float_t MomValue) 
  //return the minimum allowed value of the deviation of angular coordinates of MDC and RING (MDC-RING)
{
  return getValue(0, 0, sector, MomValue);
}

Float_t HPidTrackFillerPar::getDeltaPhiMax(Int_t sector, Float_t MomValue) 
  //return the minimum allowed value of the deviation of angular coordinates of MDC and RING (MDC-RING)
{
  return getValue(0, 1, sector, MomValue);
}


Float_t HPidTrackFillerPar::getDeltaThetaMin(Int_t sector, Float_t MomValue) 
  //return the minimum allowed value of the deviation of angular coordinates of MDC and RING (MDC-RING)
{
  return getValue(0, 2, sector, MomValue);
}

Float_t HPidTrackFillerPar::getDeltaThetaMax(Int_t sector, Float_t MomValue) 
  //return the minimum allowed value of the deviation of angular coordinates of MDC and RING (MDC-RING)
{
  return getValue(0, 3, sector, MomValue);
}

Float_t HPidTrackFillerPar::getPolynomialCoeff(Int_t sector, Float_t VertexZCoord, Float_t* coefficients)
{
  for(Int_t order=0;order<8;order++)
    {
      coefficients[order]=getValue(0, 4, sector, VertexZCoord, order);
      //cout << coefficients[order] << endl;
    }
  return 0.0;
}
// -----------------------------------------------------------------------------


