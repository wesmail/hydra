////////////////////////////////////////////////////////////////////////////
//  HGeantRpc
// 
//  GEANT RPC event header
//
//  last modified on 10/08/2007  by D. Gonzalez-Diaz
////////////////////////////////////////////////////////////////////////////
#include "hgeantrpc.h"

ClassImp(HGeantRpc)

HGeantRpc::HGeantRpc(void) {
  trackNumber    = 0;
  trackLength    = 0.0;
  loctrackLength = 0.0;
  eHit           = 0.0;
  xHit           = 0.0;
  yHit           = 0.0;
  zHit           = 0.0;
  tofHit         = 0.0;
  momHit         = 0.0;
  thetaHit       = 0.0;
  phiHit         = 0.0;
  detectorID     = -999;
}

HGeantRpc::HGeantRpc(HGeantRpc &aRpc) {
  trackNumber    = aRpc.trackNumber;
  trackLength    = aRpc.trackLength;
  loctrackLength = aRpc.loctrackLength;
  eHit           = aRpc.eHit;
  xHit           = aRpc.xHit;
  yHit           = aRpc.yHit;
  zHit           = aRpc.zHit;
  tofHit         = aRpc.tofHit;
  momHit         = aRpc.momHit;
  thetaHit       = aRpc.thetaHit;
  phiHit         = aRpc.phiHit;
  detectorID     = aRpc.detectorID;
}

HGeantRpc::~HGeantRpc(void) {
}

void HGeantRpc::setHit(Float_t axHit, Float_t ayHit, Float_t azHit,
		       Float_t atofHit, Float_t amomHit, Float_t aeHit)
{
  eHit        = aeHit;
  xHit        = axHit;
  yHit        = ayHit;
  zHit        = azHit;
  tofHit      = atofHit;
  momHit      = amomHit;
}

void HGeantRpc:: getHit(Float_t& axHit, Float_t& ayHit, Float_t& azHit, 
			Float_t& atofHit, Float_t& amomHit, Float_t& aeHit)
{
  aeHit          = eHit;
  axHit          = xHit;
  ayHit          = yHit;
  azHit          = zHit;
  atofHit        = tofHit;
  amomHit        = momHit;
}

void HGeantRpc::setIncidence(Float_t athetaHit, Float_t aphiHit)
{
  thetaHit       = athetaHit;
  phiHit         = aphiHit;
}

void HGeantRpc:: getIncidence(Float_t& athetaHit, Float_t& aphiHit)
{
  athetaHit      = thetaHit;
  aphiHit        = phiHit;
}

void HGeantRpc::setTLength(Float_t atrackLength, Float_t aloctrackLength)
{
  trackLength    = atrackLength;
  loctrackLength = aloctrackLength;
}

void HGeantRpc:: getTLength(Float_t& atrackLength, Float_t& aloctrackLength)
{
  atrackLength    = trackLength;
  aloctrackLength = loctrackLength;
}







