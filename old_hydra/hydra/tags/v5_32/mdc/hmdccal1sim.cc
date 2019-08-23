//*-- AUTHOR : Ilse Koenig
//*-- Modified : 18/06/99 by Ilse Koenig

///////////////////////////////////////////////////////////////////////////////
//
// HMdcCal1Sim
//
// Data object for a Mdc cell containing digitized drift times
// from simulation
// This class inherits from the class HMdcCal1 used for real data.
//
// As additional data elements the track numbers of the two hits are stored
// and a status number set by the digitizer.
// The track number of the hits can be accessed via the inline functions    
//     void setNTrack1(const Int_t nTrack1)
//     void setNTrack2(const Int_t nTrack2)
//     Int_t getNTrack1(void) const
//     Int_t getNTrack2(void) const
// and the status via the inline functions 
//     void setStatus(const Int_t flag)
//     Int_t getStatus() const
//
///////////////////////////////////////////////////////////////////////////////

#include "hmdccal1sim.h"

ClassImp(HMdcCal1Sim)

void HMdcCal1Sim::clear(void) {
  // clears the object
  HMdcCal1::clear();
  nTrack1=nTrack2=-99;
  status=0;
}
