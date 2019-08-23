#include "hrichhitsim.h"
#include "TArrayI.h"
ClassImp(HRichHitSim)

HRichHitSim::HRichHitSim() {
    for(Int_t i =0; i<256;i++){
       iRingPadNTrack1[i] = 0;
       iRingPadNTrack2[i] = 0;
        iRingTrack[i] = 0;
        iRingFlag[i] = 0;  
    }
    track1 = track2 =track3 =0;  
    weigTrack1 = weigTrack2 =weigTrack3 =0;  
    flag1 =  flag2 = flag3 = -1;
}

HRichHitSim& HRichHitSim::operator=(const HRichHitSim& source) {
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
   Int_t i =0;
   while ((source.iRingTrack[i])) {
     iRingTrack[i] = source.iRingTrack[i];
     iRingFlag[i] = source.iRingFlag[i];
    
     i++;
   }
  
 }
return *this;
}
 





