////////////////////////////////////////////////////////////////////////////
//  HGeantWall
// 
//  GEANT Forward Wall event header
//
//  last modified on 25/11/2004  by R.Holzmann  (GSI)
////////////////////////////////////////////////////////////////////////////
#include "hgeantwall.h"

ClassImp(HGeantWall)

HGeantWall::HGeantWall(void) {
  trackNumber = 0;
  trackLength = 0.0;
  eHit = 0.0;
  xHit = 0.0;
  yHit = 0.0;
  tofHit = 0.0;
  momHit = 0.0;
  module = -1;
  cell = -1;
}

HGeantWall::HGeantWall(HGeantWall &aWall) {
  trackNumber = aWall.trackNumber;
  trackLength = aWall.trackLength;
  eHit = aWall.eHit;
  xHit = aWall.xHit;
  yHit = aWall.yHit;
  tofHit = aWall.tofHit;
  momHit = aWall.momHit;
  module = aWall.module;
  cell = aWall.cell;
}

HGeantWall::~HGeantWall(void) {
}

void HGeantWall::setHit(Float_t ae, Float_t ax, Float_t ay, Float_t atof,
                       Float_t amom, Float_t alen) {
  eHit = ae;
  xHit = ax;
  yHit = ay;
  tofHit = atof;
  momHit = amom;
  trackLength = alen;
}

void HGeantWall::getHit(Float_t& ae, Float_t& ax,  Float_t& ay, Float_t& atof,
                       Float_t& amom, Float_t& alen) {
  ae = eHit;
  ax = xHit;
  ay = yHit;
  atof = tofHit;
  amom = momHit;
  alen = trackLength;
}






