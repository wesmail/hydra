#include "hkicktracksimB.h"

ClassImp(HKickTrackSimB)

HKickTrackSimB::HKickTrackSimB(void)
{
  fNMdcTracks =0;     
  fNMetaTracks=0;    
  for(Int_t i=0;i<5;i++)
  {
      fMdcTracks[ i]=0; //Size is 5
  }

  for(Int_t i=0;i<2;i++)
  {
      fMetaTracks[i]=0; //Size is 2
  }
}
