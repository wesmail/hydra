// File: hgeantrich.cc
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

//*-- Author : Romain Holzmann (r.holzmann@gsi.de)
//*-- Modified : 1999/10/07 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)

#include "hgeantrich.h"

// ***************************************************************************
//////////////////////////////////////////////////////////////////////////////
//
//  HGeantRichPhoton
// 
//  Reading of GEANT simulated data of RICH Cherenkov photons.
//
//////////////////////////////////////////////////////////////////////////////
// ***************************************************************************

ClassImp(HGeantRichPhoton)

//----------------------------------------------------------------------------
HGeantRichPhoton::HGeantRichPhoton() 
{
// Default constructor.

 parentTrack = 0;
 xHit = 0.0;
 yHit = 0.0;
 eHit = 0.0;
 sector = -1;
}
//============================================================================

//----------------------------------------------------------------------------
HGeantRichPhoton::HGeantRichPhoton(const HGeantRichPhoton &source) 
{
// Copy constructor.

  parentTrack = source.parentTrack;
  xHit = source.xHit;
  yHit = source.yHit;
  eHit = source.eHit;
  if(eHit < 0.) eHit = 0.;
  sector = source.sector;
}
//============================================================================

//----------------------------------------------------------------------------
HGeantRichPhoton::~HGeantRichPhoton() 
{
// Destructor.
}
//============================================================================

//----------------------------------------------------------------------------
void HGeantRichPhoton::setHit(Float_t ax, Float_t ay, Float_t ae) 
{
// Photon hit information consists of x,y position and photon energy (eV).

  xHit = ax;
  yHit = ay;
  yHit = ay;
  eHit = ae;
}
//============================================================================

//----------------------------------------------------------------------------
void HGeantRichPhoton::getHit(Float_t &ax, Float_t &ay, Float_t &ae) 
{
// Photon hit information consists of x,y position and photon energy (eV).

  ax = xHit;
  ay = yHit;
  ae = eHit;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HGeantRichPhoton::getLocationIndex(Int_t i) 
{
// Location index is here sector number.

  switch (i) {
    case 0 : return sector;
  }
  return -1;
}
//============================================================================


// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$


//*-- Author : Romain Holzmann (r.holzmann@gsi.de)
//*-- Modified : 1999/10/07 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)

// ***************************************************************************
//////////////////////////////////////////////////////////////////////////////
//
//  HGeantRichDirect
//
//  Reading of GEANT simulated data of RICH direct hits to photon detector.
//
//////////////////////////////////////////////////////////////////////////////
// ***************************************************************************

ClassImp(HGeantRichDirect)

//----------------------------------------------------------------------------
HGeantRichDirect::HGeantRichDirect() 
{
// Default constructor.

  trackNumber = 0;
  xHit = 0.0;
  yHit = 0.0;
  zHit = 0.0;
  particleID = 0;
  momentum = 0.0;
  thetaHit = 0.0;
  phiHit = 0.0;
  eLoss = 0.0;
  trackLength = 0.0;
  sector = -1;
}
//============================================================================

//----------------------------------------------------------------------------
HGeantRichDirect::HGeantRichDirect(const HGeantRichDirect &source) 
{
// Copy constructor.

  trackNumber = source.trackNumber;
  xHit = source.xHit;
  yHit = source.yHit;
  zHit = source.zHit;
  particleID = source.particleID;
  momentum = source.momentum;
  thetaHit = source.thetaHit;
  phiHit = source.phiHit;
  eLoss = source.eLoss;
  trackLength = source.trackLength;
  sector = source.sector;
}
//============================================================================

//----------------------------------------------------------------------------
HGeantRichDirect::~HGeantRichDirect() 
{
// Destructor.
}
//============================================================================

//----------------------------------------------------------------------------
void HGeantRichDirect::setTrack(Int_t aTrack, Int_t aID) {
  trackNumber = aTrack;
  particleID = aID;
}
//============================================================================

//----------------------------------------------------------------------------
void HGeantRichDirect::setHit(Float_t ax, Float_t ay, Float_t az) {
  xHit = ax;
  yHit = ay;
  zHit = az;
}
//============================================================================

//----------------------------------------------------------------------------
void HGeantRichDirect::setMomentum(Float_t amom, Float_t ath, Float_t aph) {
  momentum = amom;
  thetaHit = ath;
  phiHit = aph;
}
//============================================================================

//----------------------------------------------------------------------------
void HGeantRichDirect::setELoss(Float_t ael, Float_t alen) {
  eLoss = ael;
  trackLength = alen;
}
//============================================================================

//----------------------------------------------------------------------------
void HGeantRichDirect::getTrack(Int_t &aTrack, Int_t &aID) {
  aTrack = trackNumber;
  aID = particleID;
}
//============================================================================

//----------------------------------------------------------------------------
void HGeantRichDirect::getHit(Float_t &ax, Float_t &ay, Float_t &az) {
  ax = xHit;
  ay = yHit;
  az = zHit;
}
//============================================================================

//----------------------------------------------------------------------------
void HGeantRichDirect::getMomentum(Float_t &amom, Float_t &ath, Float_t &aph) {
  amom = momentum;
  ath = thetaHit;
  aph = phiHit;
}
//============================================================================

//----------------------------------------------------------------------------
void HGeantRichDirect::getELoss(Float_t &ael, Float_t &alen) {
  ael = eLoss;
  alen = trackLength;
}
//============================================================================




ClassImp(HGeantRichMirror)

//----------------------------------------------------------------------------
HGeantRichMirror::HGeantRichMirror() 
{
// Default constructor.

  trackNumber = 0;
  xLep = 0.0;
  yLep = 0.0;
  zLep = 0.0;
  lepID  = 0;
  sector = -1;
  numPhot = 0;
  xRing = 0.;
  yRing = 0.;
}
//============================================================================

//----------------------------------------------------------------------------
HGeantRichMirror::HGeantRichMirror(const HGeantRichMirror &source) 
{
// Copy constructor.

  trackNumber = source.trackNumber;
  xLep = source.xLep;
  yLep = source.yLep;
  zLep = source.zLep;
  lepID = source.lepID; 
  sector = source.sector;
  xRing =  source.xRing;
  yRing =  source.yRing;
  numPhot = source.numPhot;
}
//============================================================================

//----------------------------------------------------------------------------
HGeantRichMirror::~HGeantRichMirror() 
{
// Destructor.
}
//============================================================================

//----------------------------------------------------------------------------
void HGeantRichMirror::setTrack(Int_t aTrack, Int_t aID) {
  trackNumber = aTrack;
  lepID = aID;
}
//============================================================================

//----------------------------------------------------------------------------
void HGeantRichMirror::setHit(Float_t ax, Float_t ay, Float_t az) {
  xLep= ax;
  yLep= ay;
  zLep = az;
}
//============================================================================


//----------------------------------------------------------------------------
void HGeantRichMirror::getTrack(Int_t &aTrack, Int_t &aID) {
  aTrack = trackNumber;
  aID = lepID;
}
//============================================================================

//----------------------------------------------------------------------------
void HGeantRichMirror::getHit(Float_t &ax, Float_t &ay, Float_t &az) {
  ax = xLep;
  ay = yLep;
  az = zLep;
}
//============================================================================



void HGeantRichMirror::setXYring(Float_t ax, Float_t ay) {
 xRing = ax;
 yRing = ay; 
}


