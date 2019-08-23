////////////////////////////////////////////////////////////////////////////
//  HGeantRpc
// 
//  GEANT RPC event header
//
//  last modified on 29/12/2004  by D. Gonzalez  (USC)
////////////////////////////////////////////////////////////////////////////
#include "hgeantrpc.h"

ClassImp(HGeantRpc)

HGeantRpc::HGeantRpc(void) {
  trackNumber = 0;
  tlength     = 0.0;
  eHit        = 0.0;
  xHit        = 0.0;
  yHit        = 0.0;
  zHit        = 0.0;
  xHitM       = 0.0;
  yHitM       = 0.0;
  tofHit      = 0.0;
  momHit      = 0.0;
  thetaHit    = 0.0;
  phiHit      = 0.0;
  sector      = -1;
  module      = -1;
  cell        = -1;
  gap         = -1;
}

HGeantRpc::HGeantRpc(HGeantRpc &aRpc) {
  trackNumber = aRpc.trackNumber;
  tlength     = aRpc.tlength;
  eHit        = aRpc.eHit;
  xHit        = aRpc.xHit;
  yHit        = aRpc.yHit;
  zHit        = aRpc.zHit;
  xHitM       = aRpc.xHitM;
  yHitM       = aRpc.yHitM;
  tofHit      = aRpc.tofHit;
  momHit      = aRpc.momHit;
  thetaHit    = aRpc.thetaHit;
  phiHit      = aRpc.phiHit;
  sector      = aRpc.sector;
  module      = aRpc.module;
  cell        = aRpc.cell;
  gap         = aRpc.gap;
}

HGeantRpc::~HGeantRpc(void) {
}

void HGeantRpc::setHit(Float_t ae, Float_t ax, Float_t ay, Float_t az, 
		       Float_t axm, Float_t aym, Float_t atheta, Float_t aphi, 
		       Float_t atof, Float_t amom, Float_t alen) {
  eHit        = ae;
  xHit        = ax;
  yHit        = ay;
  zHit        = az;
  xHitM       = axm;
  yHitM       = aym;
  thetaHit    = atheta;
  phiHit      = aphi;
  tofHit      = atof;
  momHit      = amom;
  tlength     = alen;
}

void HGeantRpc::getHit(Float_t& ae, Float_t& ax,  Float_t& ay, Float_t& az, Float_t& axm, Float_t& aym, 
                       Float_t &atheta, Float_t & aphi, Float_t& atof, Float_t& amom, Float_t& alen) {
  ae          = eHit;
  ax          = xHit;
  ay          = yHit;
  az          = zHit;
  axm         = xHitM;
  aym         = yHitM;
  atheta      = thetaHit;
  aphi        = phiHit;
  atof        = tofHit;
  amom        = momHit;
  alen        = tlength;
}







