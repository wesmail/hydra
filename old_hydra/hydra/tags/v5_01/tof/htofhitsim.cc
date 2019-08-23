//*-- AUTHOR : Jaroslav Bielcik
//*-- Modified : 17/12/99 by Jaroslav Bielcik

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// HTofHitSim                                                                //
// hit class simulated data                                                  //
// contains up to 2 track numbers ofcurrent hit                              //
///////////////////////////////////////////////////////////////////////////////

#include "htofhitsim.h"

ClassImp(HTofHitSim)

 void HTofHitSim::clear() {
  // clears the object
  HTofHitSim::clear();
  nTrack1 = nTrack2 = 0;
}
