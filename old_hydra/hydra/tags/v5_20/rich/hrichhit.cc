#include "hrichhit.h"

ClassImp(HRichHit)


Int_t HRichHit::iRingImageSize = 0;
HRichHit::HRichHit() {
 iRingX = 0;
 iRingY = 0;
 iRingQuality = 0;
 iRingPatMat = 0;
 iRingHouTra = 0;
 iRingPadNr = 0;
 iRingAmplitude = 0;
 iRingLocalMax4 = 0;
 iRingLocalMax8 = 0;
 iRingClusterNr = 0;
 iRingImageSize = 0;
 iRingAlgorithmIndex = 0;
 for (int i=0; i<50; i++) {
  iRingClusterSize[i] = 0;
  iRingClusterLMax4[i] = 0;
  iRingClusterLMax8[i] = 0;
 }
 for (int i=0; i<256; i++) iRingImage[i] = 0;
 fLabX = 0.;
 fLabY = 0.;
 fLabZ = 0.;
 fX = 0.;
 fY = 0.;
 fTheta = 0.;
 fPhi = 0.;

 lEventNr = 0;
 nSector = 0;
}

HRichHit::~HRichHit() {}

HRichHit::HRichHit(const HRichHit& source) {
 iRingX = source.iRingX;
 iRingY = source.iRingY;
 iRingQuality = source.iRingQuality;
 iRingPatMat = source.iRingPatMat;
 iRingHouTra = source.iRingHouTra;
 iRingPadNr = source.iRingPadNr;
 iRingAmplitude = source.iRingAmplitude;
 iRingLocalMax4 = source.iRingLocalMax4;
 iRingLocalMax8 = source.iRingLocalMax8;
 iRingClusterNr = source.iRingClusterNr;
 iRingImageSize = source.iRingImageSize;

 for (int i=0; i<iRingClusterNr; i++) {
  iRingClusterSize[i] = source.iRingClusterSize[i];
  iRingClusterLMax4[i] = source.iRingClusterLMax4[i];
  iRingClusterLMax8[i] = source.iRingClusterLMax8[i];
 }

 iRingAlgorithmIndex = source.iRingAlgorithmIndex;
 for (int i=0; i<256; i++) iRingImage[i] = source.iRingImage[i];

 fLabX = source.fLabX;
 fLabY = source.fLabY;
 fLabZ = source.fLabZ;
 fX = source.fX;
 fY = source.fY;
 fTheta = source.fTheta;
 fPhi = source.fPhi;

 lEventNr = source.lEventNr;
 nSector = source.nSector;
}

HRichHit& HRichHit::operator=(const HRichHit& source) {
 if (this != &source) {
  iRingX = source.iRingX;
  iRingY = source.iRingY;
  iRingQuality = source.iRingQuality;
  iRingPatMat = source.iRingPatMat;
  iRingHouTra = source.iRingHouTra;
  iRingPadNr = source.iRingPadNr;
  iRingAmplitude = source.iRingAmplitude;
  iRingLocalMax4 = source.iRingLocalMax4;
  iRingLocalMax8 = source.iRingLocalMax8;
  iRingClusterNr = source.iRingClusterNr;
  iRingImageSize = source.iRingImageSize;

  for (int i=0; i<iRingClusterNr; i++) {
   iRingClusterSize[i] = source.iRingClusterSize[i];
   iRingClusterLMax4[i] = source.iRingClusterLMax4[i];
   iRingClusterLMax8[i] = source.iRingClusterLMax8[i];
  }

  iRingAlgorithmIndex = source.iRingAlgorithmIndex;
  for (int i=0; i<256; i++) iRingImage[i] = source.iRingImage[i];

  fLabX = source.fLabX;
  fLabY = source.fLabY;
  fLabZ = source.fLabZ;
  fX = source.fX;
  fY = source.fY;
  fTheta = source.fTheta;
  fPhi = source.fPhi;

  lEventNr = source.lEventNr;
  nSector = source.nSector;
 }

return *this;
}


void HRichHit::Reset() {
 iRingX = 0;
 iRingY = 0;
 iRingQuality = 0;
 iRingPatMat = 0;
 iRingHouTra = 0;
 iRingPadNr = 0;
 iRingAmplitude = 0;
 iRingLocalMax4 = 0;
 iRingLocalMax8 = 0;
 for (int i=0; i<iRingClusterNr; i++) {
  iRingClusterSize[i] = 0;
  iRingClusterLMax4[i] = 0;
  iRingClusterLMax8[i] = 0;
 }
 iRingClusterNr = 0;
 iRingAlgorithmIndex = 0;
 for (int i=0; i<256; i++) iRingImage[i] = 0;
 fLabX = 0.;
 fLabY = 0.;
 fLabZ = 0.;
 fX = 0.;
 fY = 0.;
 fTheta = 0.;
 fPhi = 0.;
 lEventNr = 0;
 nSector = 0;
}






