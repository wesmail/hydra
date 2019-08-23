//*-- AUTHOR : Jaroslav Bielcik
//*-- Modified : 18/12/99
//*-- Modified : 19/4/2005  Filip Krizek

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// HWallRawSim                                                               //
// this is raw data level for simulated data                                 //
// it stores 2 track numbers                                                 //
///////////////////////////////////////////////////////////////////////////////

#include "hwallrawsim.h"

ClassImp(HWallRawSim)

void HWallRawSim::clear(void) {
  // clears the object
  HWallRawSim::clear();
  nTrack1 = nTrack2 = -1;

}

void HWallRawSim::setNTrack( Int_t nTrack) {
  // set the track number
  // up to 2 hits per cell can be stored now
   if(nHit==1) setNTrack1(nTrack);
   if(nHit==2) setNTrack2(nTrack);
 }
