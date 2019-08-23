
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// HTofHitSim                                                                //
// hit class simulated data                                                  //
// contains up to 2 track numbers ofcurrent hit                              //
///////////////////////////////////////////////////////////////////////////////

#include "htofhitsim2.h"

ClassImp(HTofHitSim2)

 void HTofHitSim2::clear() {
  // clears the object
  HTofHitSim::clear();
  nTrack1 = nTrack2 = 0;
}
