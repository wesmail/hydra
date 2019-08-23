//*-- Modified : 17/02/2000 by Laura Fabbietti

///////////////////////////////////////////////////////////////////////////////
//_____________________________________________________________________________
//
// HRichCalSim                                                               
// cal class simulated data 
// contains  nTrack1, nTrack2, indexes of the first and last track numbers
// of the particles that hit one pad in the linear category catRichTrack. 
//                              
///////////////////////////////////////////////////////////////////////////////

#include "hrichcalsim.h"

ClassImp(HRichCalSim)

void HRichCalSim::clear() {
  // clears the object
  HRichCalSim::clear();
  nTrack1 = nTrack2 = 0;
}
