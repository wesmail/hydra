//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/17/2005
//*-- Version: $Id: hrichhitIPUSim.cc,v 1.4 2005-10-10 11:32:26 kempter Exp $

#include "hrichhitIPUSim.h"

ClassImp(HRichHitIPUSim)
Bool_t HRichHitIPUSimCont::sortIncreasing=kTRUE;


HRichHitIPUSim::HRichHitIPUSim(void)
  : HRichHitIPU()
{
  // Default constructor
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none

  track1 = track2 =track3 =0;
  weigTrack1 = weigTrack2 =weigTrack3 =0;
  flag1 =  flag2 = flag3 = -1;
}

void HRichHitIPUSim::Reset(void)
{
  // Resets all variables to the defaults.
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  iRingX = 0;
  iRingY = 0;
  iRingQuality = 0;
  iVetoQuality = 0;
  nSector = 0;
  nPads = 0;
  fTheta =0.;
  fPhi =0.;
  DataWord=0;
  track1 = track2 =track3 =0;
  weigTrack1 = weigTrack2 =weigTrack3 =0;
  flag1 =  flag2 = flag3 = -1;

}
