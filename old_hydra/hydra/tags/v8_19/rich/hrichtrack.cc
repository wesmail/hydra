//*-- Author : Laura Fabbietti
//*-- Modified : 22/02/2000 L.Fabbietti
//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////
//______________________________________________________
//  HRichTrack
//  This class stores the track numbers of the particles
//  that hit the RICH. 
//  For each particle it contains the corresponding track number,
//  the address of the fired pad and a flag that distinguishes 
//  photons and charged particles.
//  In the categories catRichGeantRaw and catRichGeantRaw+1
//  the particle track number is stored in the case that a direct
//  hit has occurred; if a photon hits the RICH, its parent track number
//  is stored, therefore we need a flag to distinguish the two cases.
//  The class is sortable by the pad address. 
// 
//////////////////////////////////////////////////////

#pragma implementation
#include "hrichtrack.h"

ClassImp(HRichTrack)


Int_t HRichTrack::clear()
{
   m_lEventNr = -1;
   m_nTrack = -1;
   m_Address = -1;
   m_Flag = -1; 
   return 1;
}



