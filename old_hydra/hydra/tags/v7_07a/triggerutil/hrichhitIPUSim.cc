//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/17/2005
//*-- Version: $Id: hrichhitIPUSim.cc,v 1.3 2005-07-15 09:02:50 wuestenf Exp $

#include "hrichhitIPUSim.h"

ClassImp(HRichHitIPUSim)


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
