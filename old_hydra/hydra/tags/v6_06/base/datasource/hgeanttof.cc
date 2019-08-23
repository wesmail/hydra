////////////////////////////////////////////////////////////////////////////
//  HGeantTof
// 
//  GEANT TOF event header
//
//  last modified on 22/11/2000  by R.Holzmann  (GSI)
////////////////////////////////////////////////////////////////////////////
#include "hgeanttof.h"

ClassImp(HGeantTof)

HGeantTof::HGeantTof(void) {
  trackNumber = 0;
  trackLength = 0.0;
  eHit = 0.0;
  xHit = 0.0;
  yHit = 0.0;
  tofHit = 0.0;
  momHit = 0.0;
  sector = -1;
  module = -1;
  cell = -1;
}

HGeantTof::HGeantTof(HGeantTof &aTof) {
  trackNumber = aTof.trackNumber;
  trackLength = aTof.trackLength;
  eHit = aTof.eHit;
  xHit = aTof.xHit;
  yHit = aTof.yHit;
  tofHit = aTof.tofHit;
  momHit = aTof.momHit;
  sector = aTof.sector;
  module = aTof.module;
  cell = aTof.cell;
}

HGeantTof::~HGeantTof(void) {
}

void HGeantTof::setHit(Float_t ae, Float_t ax, Float_t ay, Float_t atof,
                       Float_t amom, Float_t alen) {
  eHit = ae;
  xHit = ax;
  yHit = ay;
  tofHit = atof;
  momHit = amom;
  trackLength = alen;
}

inline void HGeantTof::setAddress(Char_t s, Char_t m, Char_t c) {
  sector = s;
  module = m;
  cell = c;
}

void HGeantTof::getHit(Float_t& ae, Float_t& ax,  Float_t& ay, Float_t& atof,
                       Float_t& amom, Float_t& alen) {
  ae = eHit;
  ax = xHit;
  ay = yHit;
  atof = tofHit;
  amom = momHit;
  alen = trackLength;
}

inline Int_t HGeantTof::getLocationIndex(Int_t i) {
  switch (i) {
    case 0 : return sector;
    case 1 : return module;
    case 2 : return cell;
  }
  return -1;
}

void HGeantTof::Streamer(TBuffer &R__b) {

   // Stream an object of class HGeantTof.

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion();
      HLinkedDataObject::Streamer(R__b);
      R__b >> trackNumber;
      if ( R__v <= 2 )  trackLength = 0.;
      else              R__b >> trackLength;
      R__b >> eHit;
      R__b >> xHit;
      R__b >> yHit;
      R__b >> tofHit;
      if ( R__v <= 2 )  momHit = 0.;
      else              R__b >> momHit;
      R__b >> sector;
      R__b >> module;
      R__b >> cell;
   } else {
      R__b.WriteVersion(HGeantTof::IsA());
      HLinkedDataObject::Streamer(R__b);
      R__b << trackNumber;
      R__b << trackLength;
      R__b << eHit;
      R__b << xHit;
      R__b << yHit;
      R__b << tofHit;
      R__b << momHit;
      R__b << sector;
      R__b << module;
      R__b << cell;
   }
} 






