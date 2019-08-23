//*-- Modified : 13/05/2001 by Laura Fabbietti

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
//_____________________________________________________________________________
//
// HRichCalSim                                                               
// cal class simulated data 
// contains  nTrack1, nTrack2, indexes of the first and last track numbers
// of the particles that hit one pad in the linear category catRichTrack and
// fEnergy, average energy of the photon hits on each pad.
//                              
///////////////////////////////////////////////////////////////////////////////

#include "hrichcalsim.h"

ClassImp(HRichCalSim)

void
HRichCalSim::clear()
{
   // clears the object
   HRichCal::clear();
   nTrack1 = 0;
   nTrack2 = 0;
   fEnergy = 0;
}


