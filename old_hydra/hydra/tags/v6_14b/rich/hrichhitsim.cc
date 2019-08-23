// modified by Wolfgang Koenig Oct. 2000.
// End of track list via 0, -1 is generated in HRichRingFindSim.
// No need to initialize track array in Constructor.

#include "hrichhitsim.h"

ClassImp(HRichHitSim)

HRichHitSim::HRichHitSim() {
    track1 = track2 =track3 =0;  
    weigTrack1 = weigTrack2 =weigTrack3 =0;  
    flag1 =  flag2 = flag3 = -1;
}

 





