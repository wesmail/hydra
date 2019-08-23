// $Id: hrichphotonhitheader.cc,v 1.2 2002-09-25 16:19:53 eberl Exp $
// Last update by Thomas Eberl: 02/09/25 17:44:32
//
//**********************************************************
// this class is a container class meant to hold information
// about all photon hits in a sector 
//**********************************************************
#include "hrichphotonhitheader.h"

ClassImp(HRichPhotonHitHeader)

HRichPhotonHitHeader::HRichPhotonHitHeader() {
    Reset();
}

HRichPhotonHitHeader::~HRichPhotonHitHeader() {}

HRichPhotonHitHeader::HRichPhotonHitHeader(const HRichPhotonHitHeader& source) {
 nSector = source.nSector;
 nRingX = source.nRingX;
 nRingY = source.nRingY;
 fRingPhi = source.fRingPhi;
 fRingTheta = source.fRingTheta;
 nPhotonsFound = source.nPhotonsFound;
}

HRichPhotonHitHeader& HRichPhotonHitHeader::operator=(const HRichPhotonHitHeader& source) {
 if (this != &source) {
  nSector = source.nSector;
  nRingX = source.nRingX;
  nRingY = source.nRingY;
  fRingPhi = source.fRingPhi;
  fRingTheta = source.fRingTheta;
  nPhotonsFound = source.nPhotonsFound;
 }
 
 return *this;
}


void HRichPhotonHitHeader::Reset(void) {
    nSector = 0;
    nRingX = 0;
    nRingY = 0;
    fRingPhi = 0.;
    fRingTheta = 0.;
    nPhotonsFound = 0;
}






