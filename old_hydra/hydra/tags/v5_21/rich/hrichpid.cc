#include "hrichpid.h"

ClassImp(HRichPID)


HRichPID::HRichPID() {
 fLabX = 0.;
 fLabY = 0.;
 fLabZ = 0.;
 fX = 0.;
 fY = 0.;
 fTheta = 0.;
 fPhi = 0.;

 iRingQuality = 0;
 iRingAmplitude = 0;
 iRingPadNr = 0;

 lEventNr = 0;
 nSector = 0;
}

HRichPID::~HRichPID() {}

HRichPID::HRichPID(const HRichPID& source) {
 fLabX = source.fLabX;
 fLabY = source.fLabY;
 fLabZ = source.fLabZ;
 fX = source.fX;
 fY = source.fY;
 fTheta = source.fTheta;
 fPhi = source.fPhi;

 iRingQuality = source.iRingQuality;
 iRingAmplitude = source.iRingAmplitude;
 iRingPadNr = source.iRingPadNr;


 lEventNr = source.lEventNr;
 nSector = source.nSector;
}

HRichPID& HRichPID::operator=(const HRichPID& source) {
 if (this != &source) {
  fLabX = source.fLabX;
  fLabY = source.fLabY;
  fLabZ = source.fLabZ;
  fX = source.fX;
  fY = source.fY;
  fTheta = source.fTheta;
  fPhi = source.fPhi;

  iRingQuality = source.iRingQuality;
  iRingAmplitude = source.iRingAmplitude;
  iRingPadNr = source.iRingPadNr;

  lEventNr = source.lEventNr;
  nSector = source.nSector;
 }

return *this;
}


void HRichPID::Reset() {
 fLabX = 0.;
 fLabY = 0.;
 fLabZ = 0.;
 fX = 0.;
 fY = 0.;
 fTheta = 0.;
 fPhi = 0.;
 iRingQuality = 0;
 iRingAmplitude = 0;
 iRingPadNr = 0;
 lEventNr = 0;
 nSector = 0;
}

