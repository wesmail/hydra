
#ifndef HPIDEVTINFO_H
#define HPIDEVTINFO_H

//-----------------------------------------------------------------------------
//                                                                      
// HPidEvtInfo                                                          
//                                                                     
// A container keeping characteristic information about event         
// (detector hit multiplicity, particle track multiplicity ...)      
//                                                                  
// Author : Jacek Otwinowski 18/06/2007
// Revised by Simon Lang, 22/06/2007
//
// $Id: hpidevtinfo.h,v 1.4 2007-11-26 09:59:11 kempter Exp $
//
//-----------------------------------------------------------------------------

#include "TObject.h"
#include "piddef.h"

// ----------------------------------------------------------------------------
class HPidEvtInfo : public TObject
{
   private:
    
      // hit multiplicity per detector (sector wise)
      Int_t aRichMult[MAX_SECTOR];                      // RICH rings
      Int_t aMdcClusMult[MAX_SECTOR][MAX_IOSEG];        // MDC cluster (segment wise)
      Int_t aMdcSegFittedMult[MAX_SECTOR][MAX_IOSEG];   // fitted MDC segments
      Int_t aMdcSegUnfittedMult[MAX_SECTOR][MAX_IOSEG]; // unfitted MDc segments
      Int_t aTofMult[MAX_SECTOR];    	                // TOF hit multiplicity
      Int_t aShowerMult[MAX_SECTOR];	                // Shower/TOFino hit multiplicity
      Int_t aPidTrackCandMult[MAX_SECTOR];              // Pid Track Candidate entries
      Int_t aTofinoMult[MAX_SECTOR];    	        // TOFINO cal multiplicity
      Int_t aSelectedPidTrackCandMult[MAX_SECTOR];      // selected Pid Track Candidate entries (flagged kIsUsed)

      // reaction plane information
      Int_t    nFWallMult;      // forward wall hits
      Double_t fFWallVector; 	// not yet implemented: vector size of all FW positions 
      Double_t fRPlanePhi; 	// not yet implemented: phi of reaction plane
  
   public:
  
      HPidEvtInfo(void);
      ~HPidEvtInfo(void) {}


      void setRichMult(const Int_t m[MAX_SECTOR]) {
	 for (Int_t s = 0; s < MAX_SECTOR; ++s)
	    aRichMult[s] = m[s];
      }
      void setTofMult(const Int_t m[MAX_SECTOR]) {
	 for (Int_t s = 0; s < MAX_SECTOR; ++s)
	    aTofMult[s] = m[s];
      }
      void setTofinoMult(const Int_t m[MAX_SECTOR]) {
	 for (Int_t s = 0; s < MAX_SECTOR; ++s)
	    aTofinoMult[s] = m[s];
      }
      void setShowerMult(const Int_t m[MAX_SECTOR]) {
	 for (Int_t s = 0; s < MAX_SECTOR; ++s)
	    aShowerMult[s] = m[s];
      }
      void setPidTrackCandMult(const Int_t m[MAX_SECTOR]) {
	 for (Int_t s = 0; s < MAX_SECTOR; ++s)
	    aPidTrackCandMult[s] = m[s];
      }
      void setSelectedPidTrackCandMult(const Int_t m[MAX_SECTOR]) {
	 for (Int_t s = 0; s < MAX_SECTOR; ++s)
	    aSelectedPidTrackCandMult[s] = m[s];
      }
      void setMdcClusMult(const Int_t m[MAX_SECTOR][MAX_IOSEG]) { 
	 for (Int_t s = 0; s < MAX_SECTOR; ++s)  
	    for (Int_t io = 0; io < MAX_IOSEG; ++io)
	       aMdcClusMult[s][io] = m[s][io]; 
      }
      void setMdcSegFittedMult(const Int_t m[MAX_SECTOR][MAX_IOSEG]) { 
	 for (Int_t s = 0; s < MAX_SECTOR; ++s)  
	    for (Int_t io = 0; io < MAX_IOSEG; ++io)
	       aMdcSegFittedMult[s][io] = m[s][io]; 
      }
      void setMdcSegUnfittedMult(const Int_t m[MAX_SECTOR][MAX_IOSEG]) { 
	 for (Int_t s = 0; s < MAX_SECTOR; ++s)  
	    for (Int_t io = 0; io < MAX_IOSEG; ++io)
	       aMdcSegUnfittedMult[s][io] = m[s][io]; 
      }
      void setFWallMult(Int_t m)      { nFWallMult   = m; }
      void setFWallVector(Double_t d) { fFWallVector = d; }
      void setRPlanePhi(Double_t d)   { fRPlanePhi   = d; }
	

      Int_t getRichMult(Int_t s) const {
	 return
	    s < MAX_SECTOR  ?  aRichMult[s]  :  -1;
      }
      Int_t getTofMult(Int_t s) const {
	 return
	    s < MAX_SECTOR  ?  aTofMult[s]  :  -1;
      }
      Int_t getTofinoMult(Int_t s) const {
	 return
	    s < MAX_SECTOR  ?  aTofinoMult[s]  :  -1;
      }
      Int_t getShowerMult(Int_t s) const {
	 return 
	    s < MAX_SECTOR  ?  aShowerMult[s]  :  -1;
      }
      Int_t getPidTrackCandMult(Int_t s) const {
	 return
	    s < MAX_SECTOR  ?  aPidTrackCandMult[s]  :  -1;
      }
      Int_t getSelectedPidTrackCandMult(Int_t s) const {
	 return
	    s < MAX_SECTOR  ?  aSelectedPidTrackCandMult[s]  :  -1;
      }
      Int_t getMdcClusMult(Int_t s, Int_t io) const { 
	 return
	    (s < MAX_SECTOR  &&  io < MAX_IOSEG)  ?  aMdcClusMult[s][io]  :  -1;
      }
      Int_t getMdcSegFittedMult(Int_t s, Int_t io) const { 
	 return
	    (s < MAX_SECTOR  &&  io < MAX_IOSEG)  ?
	    aMdcSegFittedMult[s][io]  :  -1;
      }
      Int_t getMdcSegUnfittedMult(Int_t s, Int_t io) const { 
	 return
	    (s < MAX_SECTOR  &&  io < MAX_IOSEG)  ?
	    aMdcSegUnfittedMult[s][io]  :  -1;
      }

      Int_t getSumRichMult() const {
	  Int_t sum = 0;
	  for (Int_t s = 0; s < MAX_SECTOR; ++s ) sum += aRichMult[s];
	  return sum;
      }
      Int_t getSumTofMult() const {
	  Int_t sum = 0;
	  for (Int_t s = 0; s < MAX_SECTOR; ++s ) sum += aTofMult[s];
	  return sum;
      }
      Int_t getSumTofinoMult() const {
	  Int_t sum = 0;
	  for (Int_t s = 0; s < MAX_SECTOR; ++s ) sum += aTofinoMult[s];
	  return sum;
      }
      Int_t getSumShowerMult() const {
	  Int_t sum = 0;
	  for (Int_t s = 0; s < MAX_SECTOR; ++s ) sum += aShowerMult[s];
	  return sum;
      }
      Int_t getSumPidTrackCandMult() const {
	  Int_t sum = 0;
	  for (Int_t s = 0; s < MAX_SECTOR; ++s ) sum += aPidTrackCandMult[s];
	  return sum;
      }
      Int_t getSumSelectedPidTrackCandMult() const {
	  Int_t sum = 0;
	  for (Int_t s = 0; s < MAX_SECTOR; ++s ) sum += aSelectedPidTrackCandMult[s];
	  return sum;
      }
      Int_t getSumMdcClusMult(Int_t io) const {
	  Int_t sum = 0;
	  for (Int_t s = 0; s < MAX_SECTOR; ++s ) sum += aMdcClusMult[s][io];
	  return sum;
      }
      Int_t getSumMdcSegFittedMult(Int_t io) const {
	  Int_t sum = 0;
	  for (Int_t s = 0; s < MAX_SECTOR; ++s ) sum += aMdcSegFittedMult[s][io];
	  return sum;
      }
      Int_t getSumMdcSegUnfittedMult(Int_t io) const {
	  Int_t sum = 0;
	  for (Int_t s = 0; s < MAX_SECTOR; ++s ) sum += aMdcSegUnfittedMult[s][io];
	  return sum;
      }



      Int_t    getFWallMult(void)   const { return nFWallMult;   }
      Double_t getFWallVector(void) const { return fFWallVector; }
      Double_t getRPlanePhi(void)   const { return fRPlanePhi;   }




      void clearMembers(void);
   ClassDef(HPidEvtInfo, 2) // Event characteristic info
};
 
#endif //HPIDEVTINFO_H
