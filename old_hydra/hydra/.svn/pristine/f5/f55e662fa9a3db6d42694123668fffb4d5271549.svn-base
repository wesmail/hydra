// @(#)$Id: hpidtrackfillerpar.cc,v 1.10 2005-01-26 14:42:26 wojcik Exp $
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

HPidTrackFillerPar::HPidTrackFillerPar(const char *name, const char *title,
                                const char *context)
                                : HPidHistogramsCont(name, title, context,4)
{
// constructor
//This parameter container has 24 histograms. They are of Type TH1F and contain the lower
//and upper limits for the difference in the angular coordinates between a ring in RICH
//and a segment in MDC. the convention for computation is MDC_coord - RICH_coord
//Eventually we might need 4 histograms per Tracking method. At the moment we hope that
//we can live with one set.
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


// -----------------------------------------------------------------------------


