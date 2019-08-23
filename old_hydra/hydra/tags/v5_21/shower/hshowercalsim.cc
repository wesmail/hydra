//*-- Modified : 17/02/00 by Laura Fabbietti

///////////////////////////////////////////////////////////////////////////////
//_____________________________________________________________________________
//
// HShowerCalSim                                                               
// cal class simulated data 
// contains  nTrack1, ntrack2, indexes of the first and last track numbers
// of the particles that hit one pad. 
//                              
///////////////////////////////////////////////////////////////////////////////

#include "hshowercalsim.h"

ClassImp(HShowerCalSim)

void HShowerCalSim::clear() {
  // clears the object
  HShowerCalSim::clear();
  nTrack1 = nTrack2 = 0;
}
