#include "hrichpid.h"

ClassImp(HRichPID)


HRichPID::HRichPID() {
 fX = 0.0F;
 fY = 0.0F;
 fTheta = 0.0F;
 fPhi = 0.0F;

 iRingQuality = 0;
 iRingAmplitude = 0;
 iRingPadNr = 0;

 // lEventNr = 0;
 nSector = 0;
}

HRichPID::~HRichPID() {}

HRichPID::HRichPID(const HRichPID& source) {
 fX = source.fX;
 fY = source.fY;
 fTheta = source.fTheta;
 fPhi = source.fPhi;

 iRingQuality = source.iRingQuality;
 iRingAmplitude = source.iRingAmplitude;
 iRingPadNr = source.iRingPadNr;


 // lEventNr = source.lEventNr;
 nSector = source.nSector;
}

HRichPID& HRichPID::operator=(const HRichPID& source) {
 if (this != &source) {
  fX = source.fX;
  fY = source.fY;
  fTheta = source.fTheta;
  fPhi = source.fPhi;

  iRingQuality = source.iRingQuality;
  iRingAmplitude = source.iRingAmplitude;
  iRingPadNr = source.iRingPadNr;

//  lEventNr = source.lEventNr;
  nSector = source.nSector;
 }

return *this;
}


void HRichPID::Reset() {
 fY = fX = 0.0F;
 fPhi = fTheta = 0.0F;
 iRingQuality = 0;
 iRingPadNr = iRingAmplitude = 0;
// lEventNr = 0;
 nSector = 0;
}

