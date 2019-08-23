////////////////////////////////////////////////////////////////////////////
//  HGeantRich
// 
//  GEANT RICH event header
//
//  last modified on 22/11/98  by R.Holzmann  (GSI)
////////////////////////////////////////////////////////////////////////////
#include "hgeantrich.h"

ClassImp(HGeantRichPhoton)

HGeantRichPhoton::HGeantRichPhoton(void) {
  parentTrack = 0;
  xHit = 0.0;
  yHit = 0.0;
  eHit = 0.0;
  sector = -1;
}

HGeantRichPhoton::HGeantRichPhoton(HGeantRichPhoton &aRich) {
  parentTrack = aRich.parentTrack;
  xHit = aRich.xHit;
  yHit = aRich.yHit;
  eHit = aRich.eHit;
  if(eHit < 0.) eHit = 0.;
  sector = aRich.sector;
}

HGeantRichPhoton::~HGeantRichPhoton(void) {
}

void HGeantRichPhoton::setHit(Float_t ax, Float_t ay, Float_t ae) {
  xHit = ax;
  yHit = ay;
  eHit = ae;
}

void HGeantRichPhoton::getHit(Float_t &ax, Float_t &ay, Float_t &ae) {
  ax = xHit;
  ay = yHit;
  ae = eHit;
}

inline Int_t HGeantRichPhoton::getLocationIndex(Int_t i) {
  switch (i) {
    case 0 : return sector;
  }
  return -1;
}

//////////////////////////////////////////////////////////////////////////////

ClassImp(HGeantRichDirect)

HGeantRichDirect::HGeantRichDirect(void) {
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

HGeantRichDirect::HGeantRichDirect(HGeantRichDirect &aRich) {
  trackNumber = aRich.trackNumber;
  xHit = aRich.xHit;
  yHit = aRich.yHit;
  zHit = aRich.zHit;
  particleID = aRich.particleID;
  momentum = aRich.momentum;
  thetaHit = aRich.thetaHit;
  phiHit = aRich.phiHit;
  eLoss = aRich.eLoss;
  trackLength = aRich.trackLength;
  sector = aRich.sector;
}

HGeantRichDirect::~HGeantRichDirect(void) {
}

void HGeantRichDirect::setTrack(Int_t aTrack, Int_t aID) {
  trackNumber = aTrack;
  particleID = aID;
}

void HGeantRichDirect::setHit(Float_t ax, Float_t ay, Float_t az) {
  xHit = ax;
  yHit = ay;
  zHit = az;
}

void HGeantRichDirect::setMomentum(Float_t amom, Float_t ath, Float_t aph) {
  momentum = amom;
  thetaHit = ath;
  phiHit = aph;
}

void HGeantRichDirect::setELoss(Float_t ael, Float_t alen) {
  eLoss = ael;
  trackLength = alen;
}

void HGeantRichDirect::getTrack(Int_t &aTrack, Int_t &aID) {
  aTrack = trackNumber;
  aID = particleID;
}

void HGeantRichDirect::getHit(Float_t &ax, Float_t &ay, Float_t &az) {
  ax = xHit;
  ay = yHit;
  az = zHit;
}

void HGeantRichDirect::getMomentum(Float_t &amom, Float_t &ath, Float_t &aph) {
  amom = momentum;
  ath = thetaHit;
  aph = phiHit;
}

void HGeantRichDirect::getELoss(Float_t &ael, Float_t &alen) {
  ael = eLoss;
  alen = trackLength;
}

inline Int_t HGeantRichDirect::getLocationIndex(Int_t i) {
  switch (i) {
    case 0 : return sector;
  }
  return -1;
}






