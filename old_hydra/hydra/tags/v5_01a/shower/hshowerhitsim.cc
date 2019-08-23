//*-- Modified : 17/02/00 by Laura Fabbietti

///////////////////////////////////////////////////////////////////////////////
//_____________________________________________________________________________
//                                                                           
// HShowerHitSim                                                               
// cal class simulated data                                                  
// contains  nTrack1, ntrack2, indexes of the first and last track numbers
// of the particles that hit one pad. 
//                              
///////////////////////////////////////////////////////////////////////////////
#pragma implementation
#include "hshowerhitsim.h"

ClassImp(HShowerHitSim)

void HShowerHitSim::clear() {
  // clears the object
  HShowerHitSim::clear();
  nTrack1 = nTrack2 = 0;
}
