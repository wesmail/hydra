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
// $Id: hpidevtinfo.cc,v 1.3 2007-11-26 09:59:11 kempter Exp $
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
      aTofinoMult[s]       = 0;
      aShowerMult[s]       = 0;
      aPidTrackCandMult[s] = 0;
      aSelectedPidTrackCandMult[s] = 0;

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
void HPidEvtInfo::Streamer(TBuffer &R__b)
{
   // Stream an object of class HPidEvtInfo.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b.ReadStaticArray((int*)aRichMult);
      R__b.ReadStaticArray((int*)aMdcClusMult);
      R__b.ReadStaticArray((int*)aMdcSegFittedMult);
      R__b.ReadStaticArray((int*)aMdcSegUnfittedMult);
      R__b.ReadStaticArray((int*)aTofMult);
      R__b.ReadStaticArray((int*)aShowerMult);
      R__b.ReadStaticArray((int*)aPidTrackCandMult);
      if(R__v > 1) {
	  R__b.ReadStaticArray((int*)aTofinoMult);
	  R__b.ReadStaticArray((int*)aSelectedPidTrackCandMult);
      }
      R__b >> nFWallMult;
      R__b >> fFWallVector;
      R__b >> fRPlanePhi;
      R__b.CheckByteCount(R__s, R__c, HPidEvtInfo::IsA());
   } else {
      R__c = R__b.WriteVersion(HPidEvtInfo::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b.WriteArray(aRichMult, 6);
      R__b.WriteArray((int*)aMdcClusMult, 12);
      R__b.WriteArray((int*)aMdcSegFittedMult, 12);
      R__b.WriteArray((int*)aMdcSegUnfittedMult, 12);
      R__b.WriteArray(aTofMult, 6);
      R__b.WriteArray(aShowerMult, 6);
      R__b.WriteArray(aPidTrackCandMult, 6);
      R__b.WriteArray(aTofinoMult, 6);
      R__b.WriteArray(aSelectedPidTrackCandMult, 6);
      R__b << nFWallMult;
      R__b << fFWallVector;
      R__b << fRPlanePhi;
      R__b.SetByteCount(R__c, kTRUE);
   }
}


