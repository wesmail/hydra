////////////////////////////////////////////////////////////////////////////
//  HGeantMdc
// 
//  GEANT MDC event header
//
//  last modified on 21/11/98  by R.Holzmann  (GSI)
////////////////////////////////////////////////////////////////////////////
#include "hgeantmdc.h"

ClassImp(HGeantMdc)

HGeantMdc::HGeantMdc(void) {
  trackNumber = 0;
  xHit = 0.0;
  yHit = 0.0;
  thetaHit = 0.0;
  phiHit = 0.0;
  tofHit = 0.0;
  sector = -1;
  module = -1;
  layer = -1;
}

HGeantMdc::HGeantMdc(HGeantMdc &aMdc) {
  trackNumber = aMdc.trackNumber;
  xHit = aMdc.xHit;
  yHit = aMdc.yHit;
  thetaHit = aMdc.thetaHit;
  phiHit = aMdc.phiHit;
  tofHit = aMdc.tofHit;
  sector = aMdc.sector;
  module = aMdc.module;
  layer = aMdc.layer;
}

HGeantMdc::~HGeantMdc(void) {
}

void HGeantMdc::setHit(Float_t ax, Float_t ay, Float_t atof) {
  xHit = ax;
  yHit = ay;
  tofHit = atof;
}

void HGeantMdc::setIncidence(Float_t ath, Float_t aph) {
  thetaHit = ath;
  phiHit = aph;
}

void HGeantMdc::setAddress(Char_t s, Char_t m, Char_t l) {
  sector = s;
  module = m;
  layer = l;
}

void HGeantMdc::getHit(Float_t &ax, Float_t &ay, Float_t &atof) {
  ax = xHit;
  ay = yHit;
  atof = tofHit;
}

void HGeantMdc::getIncidence(Float_t &ath, Float_t &aph) {
  ath = thetaHit;
  aph = phiHit;
}

inline Int_t HGeantMdc::getLocationIndex(Int_t i) {
  switch (i) {
    case 0 : return sector;
    case 1 : return module;
    case 2 : return layer;
  }
  return -1;
}







