#include "hgeanttof.h"

//*-- Author : Romain Holzmann, GSI
//*-- Modified : 29/04/99 by Romain Holzmann
//*-- Copyright : GSI, Darmstadt

////////////////////////////////////////////////////////////////////////////
//  HGeantTof
// 
//  HGeant TOF event header
//
////////////////////////////////////////////////////////////////////////////

ClassImp(HGeantTof)

HGeantTof::HGeantTof(void) {
  trackNumber = 0;
  eHit = 0.0;
  xHit = 0.0;
  tofHit = 0.0;
  sector = -1;
  module = -1;
  cell = -1;
}

HGeantTof::HGeantTof(HGeantTof &aTof) {
  trackNumber = aTof.trackNumber;
  eHit = aTof.eHit;
  xHit = aTof.xHit;
  tofHit = aTof.tofHit;
  sector = aTof.sector;
  module = aTof.module;
  cell = aTof.cell;
}

HGeantTof::~HGeantTof(void) {
}

void HGeantTof::setHit(Float_t ae, Float_t ax, Float_t atof) {
  eHit = ae;
  xHit = ax;
  tofHit = atof;
}

void HGeantTof::setAddress(Char_t s, Char_t m, Char_t c) {
  sector = s;
  module = m;
  cell = c;
}

void HGeantTof::getHit(Float_t& ae, Float_t& ax, Float_t& atof) {
  ae = eHit;
  ax = xHit;
  atof = tofHit;
}

inline Int_t HGeantTof::getLocationIndex(Int_t i) {
  switch (i) {
    case 0 : return sector;
    case 1 : return module;
    case 2 : return cell;
  }
  return -1;
}







