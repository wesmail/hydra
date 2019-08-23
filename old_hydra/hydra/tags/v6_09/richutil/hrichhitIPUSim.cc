#include "hrichhitIPUSim.h"

ClassImp(HRichHitIPUSim)


HRichHitIPUSim::HRichHitIPUSim() {
 iRingX = 0;
 iRingY = 0;
 iRingQuality = 0;
 iVetoQuality = 0;
 nSector = 0;
 nPads = 0;
 fTheta =0.;
 fPhi =0.;
}

HRichHitIPUSim::~HRichHitIPUSim() {}

HRichHitIPUSim::HRichHitIPUSim(const HRichHitIPUSim& source) {
 iRingX = source.iRingX;
 iRingY = source.iRingY;
 iRingQuality = source.iRingQuality;
 iVetoQuality = source.iVetoQuality;
 nSector = source.nSector;
 nPads = source.nPads;
 fTheta =source.fTheta;
 fPhi =source.fPhi;
}

HRichHitIPUSim& HRichHitIPUSim::operator=(const HRichHitIPUSim& source) {
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


void HRichHitIPUSim::Reset() {
 iRingX = 0;
 iRingY = 0;
 iRingQuality = 0;
 iVetoQuality = 0;
 nSector = 0;
 nPads = 0;
 fTheta =0.;
 fPhi =0.;
}











