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
 
 iRingClusterSize = 0.0F;
 iRingClusterLMax4 = 0.0F;
 iRingClusterLMax8 = 0.0F;

 for (int i=0; i<256; iRingImage[i++] = 0);
 fY = fX = 0.0F;
 fPhi = fTheta = 0.0F;

// lEventNr = 0;
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
 centroid = source.centroid;
 iCentX = source.iCentX;
 iCentY = source.iCentY;
 radius = source.radius;

 iRingClusterSize = source.iRingClusterSize;
 iRingClusterLMax4 = source.iRingClusterLMax4;
 iRingClusterLMax8 = source.iRingClusterLMax8;
 

 iRingAlgorithmIndex = source.iRingAlgorithmIndex;
 for (int i=0; i<256; i++) iRingImage[i] = source.iRingImage[i];

 fX = source.fX;
 fY = source.fY;
 fTheta = source.fTheta;
 fPhi = source.fPhi;

// lEventNr = source.lEventNr;
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
  iCentX = source.iCentX;
  iCentY = source.iCentY;
  centroid = source.centroid;
  radius = source.radius;

  iRingClusterSize = source.iRingClusterSize;
  iRingClusterLMax4 = source.iRingClusterLMax4;
  iRingClusterLMax8 = source.iRingClusterLMax8;

  iRingAlgorithmIndex = source.iRingAlgorithmIndex;
  for (int i=0; i<256; i++) iRingImage[i] = source.iRingImage[i];

  fX = source.fX;
  fY = source.fY;
  fTheta = source.fTheta;
  fPhi = source.fPhi;

//  lEventNr = source.lEventNr;
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
 iCentX = 0;
 iCentY = 0;
 centroid = 0.0F;
 radius = 0.0F;

 iRingClusterSize = 0.0F;
 iRingClusterLMax4 = 0.0F;
 iRingClusterLMax8 = 0.0F;
 
 iRingClusterNr = 0; //reset of cluster counter is sufficient
 iRingAlgorithmIndex = 0;
 for (int i=0; i<256; iRingImage[i++] = 0);
 fX = 0.0F;
 fY = 0.0F;
 fTheta = 0.0F;
 fPhi = 0.0F;
// lEventNr = 0;
 nSector = 0;
}






