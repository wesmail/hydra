//-----------------------------------------------------------------------------
//                                                                      
// HPidEvtInfo                                                          
//                                                                     
// A container keeping information about event                        
// (detector hit multiplicity, particle track multiplicity,          
//  reaction plane)                                                 
//                                                                 
// Author : Jacek Otwinowski 18/06/2007                        
// Revised by Simon Lang, 22/06/2007
//
// $Id: hpidevtinfo.cc,v 1.2 2007-06-22 17:59:26 slang Exp $
//
//-----------------------------------------------------------------------------

#pragma implementation

#include "hpidevtinfo.h"

// ----------------------------------------------------------------------------

ClassImp(HPidEvtInfo)
  
// ----------------------------------------------------------------------------
HPidEvtInfo::HPidEvtInfo(void)
{
   // Default HPidEvtInfo constructor

   clearMembers();
}

// ----------------------------------------------------------------------------
void HPidEvtInfo::clearMembers(void)
{
   // Just set all member variables to 0.

   for (Int_t s = 0; s < MAX_SECTOR; ++s)
   {
      aRichMult[s]         = 0;
      aTofMult[s]          = 0;
      aShowerMult[s]       = 0;
      aPidTrackCandMult[s] = 0;

      for (Int_t io = 0; io < MAX_IOSEG; ++io)  
      {
	 aMdcClusMult[s][io]        = 0;
	 aMdcSegFittedMult[s][io]   = 0;
	 aMdcSegUnfittedMult[s][io] = 0;
      }
   }

   nFWallMult   = 0;
   fFWallVector = 0.0;    
   fRPlanePhi   = 0.0;
}

// ----------------------------------------------------------------------------


