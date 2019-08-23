////////////////////////////////////////////////////////////////////////////
//  HGeantShower
// 
//  GEANT SHOWER event header
//
//  last modified on 21/11/98  by R.Holzmann  (GSI)
////////////////////////////////////////////////////////////////////////////
#include "hgeantshower.h"

ClassImp(HGeantShower)

HGeantShower::HGeantShower(void) {
  trackNumber = 0;
  eHit = 0.0;
  xHit = 0.0;
  yHit = 0.0;
  thetaHit = 0.0;
  phiHit = 0.0;
  betaHit = 0.0;
  sector = -1;
  module = -1;
}

HGeantShower::HGeantShower(HGeantShower &aShower) {
  trackNumber = aShower.trackNumber;
  eHit = aShower.eHit;
  xHit = aShower.xHit;
  yHit = aShower.yHit;
  thetaHit = aShower.thetaHit;
  phiHit = aShower.phiHit;
  betaHit = aShower.betaHit;
  sector = aShower.sector;
  module = aShower.module;
}

HGeantShower::~HGeantShower(void) {
}

void HGeantShower::setHit(Float_t ae, Float_t ax, Float_t ay, Float_t abeta) {
  eHit = ae;
  xHit = ax;
  yHit = ay;
  betaHit = abeta;
}

void HGeantShower::setIncidence(Float_t ath, Float_t aph) {
  thetaHit = ath;
  phiHit = aph;
}

void HGeantShower::setAddress(Char_t s, Char_t m) {
  sector = s;
  module = m;
}

void HGeantShower::getHit(Float_t &ae, Float_t &ax, Float_t &ay, Float_t &at) {
  ae = eHit;
  ax = xHit;
  ay = yHit;
  at = betaHit;
}

void HGeantShower::getIncidence(Float_t &ath, Float_t &aph) {
  ath = thetaHit;
  aph = phiHit;
}

inline Int_t HGeantShower::getLocationIndex(Int_t i) {
  switch (i) {
    case 0 : return sector;
    case 1 : return module;
  }
  return -1;
}







