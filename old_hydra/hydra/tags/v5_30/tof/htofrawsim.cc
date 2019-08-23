//*-- AUTHOR : Jaroslav Bielcik
//*-- Modified : 18/12/99

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// HTofRawSim                                                                //
// this is raw data level for simulated data                                 //
// it stores 2 track numbers                                                 //
///////////////////////////////////////////////////////////////////////////////

#include "htofrawsim.h"

ClassImp(HTofRawSim)

void HTofRawSim::clear(void) {
  // clears the object
  HTofRawSim::clear();
  nTrack1 = nTrack2 = 0;

}

void HTofRawSim::setNTrack( Int_t nTrack) {
  // set the track number
  // up to 2 hits per cell can be stored now
   if(nHit==1) setNTrack1(nTrack);
   if(nHit==2) setNTrack2(nTrack);
 }
