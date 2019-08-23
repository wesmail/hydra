#include "hrichhitsim.h"
#include "TArrayI.h"
ClassImp(HRichHitSim)

HRichHitSim::HRichHitSim() {
    for(Int_t i =0; i<256;i++){
       iRingPadNTrack1[i] = -10;
       iRingPadNTrack2[i] = -10;
        iRingTrack[i] = 0;
        iRingFlag[i] = 0;  
    }
    track1 = track2 =track3 =0;  
    weigTrack1 = weigTrack2 =weigTrack3 =0;  
    flag1 =  flag2 = flag3 = -1;
}

HRichHitSim& HRichHitSim::operator=(const HRichHitSim& source) {
 
 HRichHit::operator=(source);

 if (this != &source) {
   Int_t i =0;
   while ((source.iRingTrack[i])) {
     iRingTrack[i] = source.iRingTrack[i];
     iRingFlag[i] = source.iRingFlag[i];
     i++;
   }
 }
 return *this;
}
 





