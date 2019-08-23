// @(#)$Id: hpidtrackfillerpar.cc,v 1.18 2008-05-09 16:15:06 halo Exp $
//*-- Author : Marcin Jaskula 09/10/2002
//*-- Modified: Marcin Jaskula and Jacek Otwinowski 05/02/2003
//           - iTofinoMult removed
//*-- Modified: Marcin Jaskula 14/03/2003
//           - meta overlap added

//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidTrackFillerPar                                                         //
//                                                                            //
// HPidFrackFiller parameters:                                                //
// Holds histograms that store the momentum-dependent correlation windows     //
// between RICH and MDC (Theta and Phi) and the analytical parametrization    //
// of the polar-angle correction that must be applied to the ring coordinate  //
// in order to account for the vertex-position (strictly: the deviation of    //
// vertex from the ideal position.                                            //
//                                                                            //
// The parameter container has 30 histograms (5 per sector).                  //
// They are arranged in the following way:                                    //
// 1. There are only histograms for one species filled (PID==0), because the  //
//    container is NOT particle dependent!                                    //
// 2. The histograms contain the following data:                              //
//    offset=0: lower limits for delta phi rich/mdc                           //
//    offset=1: uppeer limits for delta phi rich/mdc                          //
//    offset=2: lower limits for delta theta rich/mdc                         //
//    offset=3: upper limits for delta theta rich/mdc                         //
//    offset=4: 2D-histogram containing the coefficients for the polynomial   //
//    correction of the rich ring coordinate for matching with MDC            //
//    (first dimension: z-coordinate, second dimension: order of the          //
//    coefficient                                                             //
//                                                                            // 
////////////////////////////////////////////////////////////////////////////////


using namespace std;

#include "hpidtrackfillerpar.h"
#include <iostream>
#include "TH1.h"
// -----------------------------------------------------------------------------

ClassImp(HPidTrackFillerPar)


HPidTrackFillerPar::HPidTrackFillerPar(const Char_t *name, const Char_t *title,
                                const Char_t *context)
                                : HPidHistogramsCont(name, title, context,5)
{
  // constructor
}

Float_t HPidTrackFillerPar::getInterpolatedValue(TH1*pH, Int_t sector,Float_t MomValue)
{
  TAxis *pA;
  Int_t  iBinX, iBinXLower;
  
  pA = pH->GetXaxis();
  
  iBinX = pA->FindBin(MomValue);

  //We need to start interpolation from the next-lower bin boundary

  if(iBinX>1&&iBinX<=pA->GetNbins())
  {   // only here it make sense to interpolate
      iBinXLower = iBinX-1;

      Float_t InterpolationStartValue=pH->GetBinContent(iBinXLower);

      Float_t InterpolationEndValue=pH->GetBinContent(iBinX);

      Float_t InterpolationStartCoordinate = pA->GetBinUpEdge(iBinXLower);

      Float_t Slope=(InterpolationEndValue-InterpolationStartValue)/pA->GetBinWidth(iBinX);

      Float_t Interpolation=InterpolationStartValue+Slope*(MomValue-InterpolationStartCoordinate);

      return Interpolation;
  } else {
      // noting to interpolate....

      if(iBinX <= 1)
      {
	  Error("HPidTrackFillerPar::getInterpolatedValue()","Non-intercepted bin-out-of-range-situation (bin too low)");

	  // lower boundary violation ...return lowest bin
	  return  pH->GetBinContent(1);
      }
      else
      {
	  Error("HPidTrackFillerPar::getInterpolatedValue()","Non-intercepted bin-out-of-range-situation (bin too high)");

      // upper boundary violation ...return highest bin
	  return  pH->GetBinContent(pA->GetNbins());
      }
  }
}

//return the minimum allowed value of the deviation of phi coordinates of MDC and RING (RING-SEGMENT)
Float_t HPidTrackFillerPar::getDeltaPhiMin(Int_t sector, Float_t MomValue) 
{
  //intercept lowest bin (no interpolation required) and overflow bin. 
#warning This should be made independent of the lower / upper histogram boundaries!
  if(MomValue<=100.0 || MomValue>=2000)
    {
      return (Float_t)getValue(0, 0, sector, MomValue);
    }
  

  //linear interpolation between two bin boundaries
  TH1 *pH;
  
  if((pH = (TH1 *) getHistogram(0,0, sector )) == NULL)
    {      
      Error("HPidTrackFillerPar::getDeltaPhiMin","No parameter histogram found");
      //This shouldn't happen!
      exit(-1);
      //return 0.0f;
      
    }
  
  return getInterpolatedValue(pH, sector, MomValue);
  
}  
//return the maximum allowed value of the deviation of phi coordinates of MDC and RING (RING-SEGMENT)
Float_t HPidTrackFillerPar::getDeltaPhiMax(Int_t sector, Float_t MomValue) 
{
#warning This should be made independent of the lower / upper histogram boundaries!
  if(MomValue<=100.0 || MomValue>=2000)
    {
      return (Float_t)getValue(0, 1, sector, MomValue);
    }
  
  TH1 *pH;
  
  if((pH = (TH1 *) getHistogram(0,1, sector )) == NULL)
    {      
      Error("HPidTrackFillerPar::getDeltaPhiMin","No parameter histogram found");
      exit(-1);
      //return 0.0f;   
    }
  
  return getInterpolatedValue(pH, sector, MomValue);
}

//return the minimum allowed value of the deviation of theta coordinates of MDC and RING (RING-SEGMENT)
Float_t HPidTrackFillerPar::getDeltaThetaMin(Int_t sector, Float_t MomValue) 
{
#warning This should be made independent of the lower / upper histogram boundaries!

  if(MomValue<=100.0 || MomValue>=2000)
    {
      return (Float_t)getValue(0, 2, sector, MomValue);
    }
  
  TH1 *pH;
  
  if((pH = (TH1 *) getHistogram(0,2, sector )) == NULL)
    {      
      Error("HPidTrackFillerPar::getDeltaPhiMin","No parameter histogram found");
      exit(-1);
    }

  return getInterpolatedValue(pH, sector, MomValue);
}

//return the maximum allowed value of the deviation of theta coordinates of MDC and RING (RING-SEGMENT)
Float_t HPidTrackFillerPar::getDeltaThetaMax(Int_t sector, Float_t MomValue) 
{
#warning This should be made independent of the lower / upper histogram boundaries!
  if(MomValue<=100.0 || MomValue>=2000)
    {
      return (Float_t)getValue(0, 3, sector, MomValue);
    }
  
  TH1 *pH;
  
  if((pH = (TH1 *) getHistogram(0,3, sector )) == NULL)
    {      
      Error("HPidTrackFillerPar::getDeltaPhiMin","No parameter histogram found");
      exit(-1);
      
    }
  return getInterpolatedValue(pH, sector, MomValue);

}

//Based on the sector and the event vertex the relevant shift-paramtrization i stored in the array which 
//is given by address
Float_t HPidTrackFillerPar::getPolynomialCoeff(Int_t sector, Float_t VertexZCoord, Float_t* coefficients)
{
  for(Int_t order=0;order<8;order++)
    {
      //offset for coefficients is 4 - see documentation above
      coefficients[order]=getValue(0, 4, sector, VertexZCoord, order);
    }
  return 0.0;
}
// -----------------------------------------------------------------------------


