#include "hrichhit.h"

ClassImp(HRichHit)


Int_t HRichHit::iRingImageSize = 0;
HRichHit::HRichHit() {
 iRingX = 0;
 iRingY = 0;
 iRingQuality = 0;
 iRingPadNr = 0;
 iRingAmplitude = 0;
 iRingLocalMax4 = 0;
 iRingLocalMax8 = 0;
 iRingClusterNr = 0;
 iRingImageSize = 0;
 iRingAlgorithmIndex = 0;
 iRingImage.Set(256);
 fLabPositionX = 0.;
 fLabPositionY = 0.;
 fLabPositionZ = 0.;
 fLocalPositionX = 0.;
 fLocalPositionY = 0.;
 fTheta = 0.;
 fPhi = 0.;

 lEventNr = 0;
 nSector = 0;
}

HRichHit::~HRichHit() {
}

HRichHit::HRichHit(const HRichHit& source) {
 iRingX = source.iRingX;
 iRingY = source.iRingY;
 iRingQuality = source.iRingQuality;
 iRingPadNr = source.iRingPadNr;
 iRingAmplitude = source.iRingAmplitude;
 iRingLocalMax4 = source.iRingLocalMax4;
 iRingLocalMax8 = source.iRingLocalMax8;
 iRingClusterNr = source.iRingClusterNr;
 iRingImageSize = source.iRingImageSize;

 iRingClusterSize = source.iRingClusterSize;
 iRingClusterLMax4 = source.iRingClusterLMax4;
 iRingClusterLMax8 = source.iRingClusterLMax8;
 iRingAlgorithmIndex = source.iRingAlgorithmIndex;
 iRingImage = source.iRingImage;

 fLabPositionX = source.fLabPositionX;
 fLabPositionY = source.fLabPositionY;
 fLabPositionZ = source.fLabPositionZ;
 fLocalPositionX = source.fLocalPositionX;
 fLocalPositionY = source.fLocalPositionY;
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
   iRingPadNr = source.iRingPadNr;
   iRingAmplitude = source.iRingAmplitude;
   iRingLocalMax4 = source.iRingLocalMax4;
   iRingLocalMax8 = source.iRingLocalMax8;
   iRingClusterNr = source.iRingClusterNr;
   iRingImageSize = source.iRingImageSize;

   iRingClusterSize = source.iRingClusterSize;
   iRingClusterLMax4 = source.iRingClusterLMax4;
   iRingClusterLMax8 = source.iRingClusterLMax8;
   iRingAlgorithmIndex = source.iRingAlgorithmIndex;
   iRingImage = source.iRingImage;

   lEventNr = source.lEventNr;
   nSector = source.nSector;
   fTheta = source.fTheta;
   fPhi = source.fPhi;
 }

return *this;
}


void HRichHit::Reset() {
 iRingX = 0;
 iRingY = 0;
 iRingQuality = 0;
 iRingPadNr = 0;
 iRingAmplitude = 0;
 iRingLocalMax4 = 0;
 iRingLocalMax8 = 0;
 iRingClusterNr = 0;
 iRingClusterSize.Reset();
 iRingClusterLMax4.Reset();
 iRingClusterLMax8.Reset();
 iRingAlgorithmIndex = 0;
 iRingImage.Reset();
 fLabPositionX = 0.;
 fLabPositionY = 0.;
 fLabPositionZ = 0.;
 fLocalPositionX = 0.;
 fLocalPositionY = 0.;
 fTheta = 0.;
 fPhi = 0.;
 lEventNr = 0;
 nSector = 0;
}
















