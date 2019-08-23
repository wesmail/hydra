#include "hrichhitIPU.h"

ClassImp(HRichHitIPU)


HRichHitIPU::HRichHitIPU() {
 iRingX = 0;
 iRingY = 0;
 iRingQuality = 0;
 iVetoQuality = 0;
 nSector = 0;
 nPads = 0;
 fTheta =0.;
 fPhi =0.;
}

HRichHitIPU::~HRichHitIPU() {}

HRichHitIPU::HRichHitIPU(const HRichHitIPU& source) {
 iRingX = source.iRingX;
 iRingY = source.iRingY;
 iRingQuality = source.iRingQuality;
 iVetoQuality = source.iVetoQuality;
 nSector = source.nSector;
 nPads = source.nPads;
 fTheta =source.fTheta;
 fPhi =source.fPhi;
}

HRichHitIPU& HRichHitIPU::operator=(const HRichHitIPU& source) {
 if (this != &source) {
  iRingX = source.iRingX;
  iRingY = source.iRingY;
  iRingQuality = source.iRingQuality;
  iVetoQuality = source.iVetoQuality;
  nSector = source.nSector;
  nPads = source.nPads;
  fTheta =source.fTheta;
  fPhi =source.fPhi;
 }

return *this;
}


void HRichHitIPU::Reset() {
 iRingX = 0;
 iRingY = 0;
 iRingQuality = 0;
 iVetoQuality = 0;
 nSector = 0;
 nPads = 0;
 fTheta =0.;
 fPhi =0.;
}











