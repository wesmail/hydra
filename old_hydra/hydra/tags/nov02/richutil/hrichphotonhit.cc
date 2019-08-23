// $Id: hrichphotonhit.cc,v 1.2 2002-09-25 16:19:53 eberl Exp $
// Last update by Thomas Eberl: 02/09/25 17:44:09
//
//**********************************************************
// this class is a container class meant to hold one
// found photon hit on the padplane 
//**********************************************************
#include "hrichphotonhit.h"

ClassImp(HRichPhotonHit)

HRichPhotonHit::HRichPhotonHit() {
    Reset();
}

HRichPhotonHit::~HRichPhotonHit() {}

HRichPhotonHit::HRichPhotonHit(const HRichPhotonHit& source) {
 fTheta = source.fTheta;
 fPhi = source.fPhi;
 fX = source.fX;
 fY = source.fY;
 nSector = source.nSector;
}

HRichPhotonHit& HRichPhotonHit::operator=(const HRichPhotonHit& source) {
 if (this != &source) {
  fX = source.fX;
  fY = source.fY;
  fTheta = source.fTheta;
  fPhi = source.fPhi;
  nSector = source.nSector;
 }
 
 return *this;
}


void HRichPhotonHit::Reset(void) {
    fX = 0.0F;
    fY = 0.0F;
    fTheta = 0.0F;
    fPhi = 0.0F;
    nSector = 0;
    fCharge = 0.;
    nRingX = 0;
    nRingY = 0;
}






