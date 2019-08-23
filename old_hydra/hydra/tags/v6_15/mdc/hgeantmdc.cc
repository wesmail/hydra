#include "hgeantmdc.h"

//*-- Author : Romain Holzmann, GSI
//*-- Modified : 1/12/2000 by Romain Holzmann
//*-- Copyright : GSI, Darmstadt

////////////////////////////////////////////////////////////////////////////
//  HGeantMdc
// 
//  HGeant MDC event header
//
////////////////////////////////////////////////////////////////////////////

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

void HGeantMdc::setHit(Float_t ax, Float_t ay, Float_t atof, Float_t ptof) {
  xHit = ax;
  yHit = ay;
  tofHit = atof;
  momHit = ptof;
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

void HGeantMdc::getHit(Float_t &ax, Float_t &ay, Float_t &atof, Float_t &ptof) {
  ax = xHit;
  ay = yHit;
  atof = tofHit;
  ptof = momHit;
}

void HGeantMdc::getIncidence(Float_t &ath, Float_t &aph) {
  ath = thetaHit;
  aph = phiHit;
}

Int_t HGeantMdc::getLocationIndex(Int_t i) {
  switch (i) {
    case 0 : return sector;
    case 1 : return module;
    case 2 : return layer;
  }
  return -1;
}

void HGeantMdc::Streamer(TBuffer &R__b) {
   // Stream an object of class HGeantMdc.

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion();
      HLinkedDataObject::Streamer(R__b);
      R__b >> trackNumber;
      R__b >> xHit;
      R__b >> yHit;
      R__b >> thetaHit;
      R__b >> phiHit;
      R__b >> tofHit;
      if (R__v <3) momHit=0.;
      else R__b >> momHit;
      R__b >> sector;
      R__b >> module;
      R__b >> layer;
   } else { 
      R__b.WriteVersion(HGeantMdc::IsA());
      HLinkedDataObject::Streamer(R__b);
      R__b << trackNumber;
      R__b << xHit;
      R__b << yHit;
      R__b << thetaHit;
      R__b << phiHit;
      R__b << tofHit;
      R__b << momHit;
      R__b << sector;
      R__b << module;
      R__b << layer;
   } 
}





