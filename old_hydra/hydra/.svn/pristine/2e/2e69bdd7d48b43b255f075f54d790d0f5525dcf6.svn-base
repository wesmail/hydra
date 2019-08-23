// @(#)$Id: hpidtrackcand.h,v 1.12 2009-07-03 12:43:18 halo Exp $
//*-- Author : Marcin Jaskula 28/06/2002
// Modified  : Marcin Jaskula 16/09/2002
//             - removed: HArrayS, OuterHits
//             - Rings array -> one Ring Id
// Modified  : Marcin Jaskula 31/10/2002
//           - buildPidTrackCategory moved from global to static function
// Modified  : Marcin Jaskula 10/11/2002
//            - get*Sim methods added
// Modified  : Marcin Jaskula 05/02/2003
//            - isBad() and isFiltered() added
// Modified  : Marcin Jaskula 25/02/2003
//            - static functions to retrive the Hits, Rings and Tracks
//            Definition of getGeantTrackSet() changed !
// Redesigned: Tassilo Christ for compliance with new tracking scheme 
//             Inherits now only from TObject
// Upgrade of documentation 05/02/2005              

#ifndef HPIDTRACKCAND_H
#define HPIDTRACKCAND_H

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// HPidTrackCand                                                        //
//                                                                      //
// A container of track candidate.                                      //
// Contains basically two other objects HPidHitData and HPidTrackData   //
// Serves as starting point for particle identification                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TObject.h"
#include "TString.h"
#include "piddef.h"
#include "hpidhitdata.h"
#include "hpidtrackdata.h"


#include <iostream>
using namespace std;
// -----------------------------------------------------------------------------

class HCategory;

// -----------------------------------------------------------------------------
class HPidTrackCand : public TObject
{
 public:
  
  HPidTrackCand(void);
  HPidTrackCand(HMetaMatch* pMetaMatch, Int_t iMatchIndex);
  
  HPidHitData itsHitData;        // The hit data object which holds all PID-relevant information for this class
  HPidTrackData itsTrackData;    // The track data object which holds all PID-relevant information for this class
  Int_t flags;                   // stored flags for Double_t hits, best hits etc (bitwise) . See enum definition for the bit masks

  // -------------------------------------------------------------------------
  // Track Cleaner flags
  enum EflagBits {kIsDoubleHitRICH       =  0,    //
                  kIsDoubleHitInnerMDC   =  1,    //
                  kIsDoubleHitOuterMDC   =  2,    //
		  kIsDoubleHitMETA       =  3,    //

		  kIsBestHitRICH         =  4,    //  (by number of pads)
		  kIsBestHitInnerMDC     =  5,    //  (by chi2)
		  kIsBestHitOuterMDC     =  6,    //  (by chi2)
		  kIsBestHitMETA         =  7,    //  (by META match quality)

		  kIsBestRKMETA          =  8,    //  (by RK META match quality)
		  kIsBestRKRICH          =  9,    //  (by RK RICH match quality)
		  kIsBestRK              = 10,    //  (by RK chi2, none fitted outer segments with lower priority)
		  kIsBestSPLINE          = 11,    //  (by chi2)
		  kIsBestKICK            = 12,    //  (by pull)
		  kIsBestRKRKMETA        = 13,    //  (by RK chi2 * RK META match quality)
		  kIsBestRKRKRICH        = 14,     // (by RK chi2 * RK RICH match quality)
                  kIsBestRKMETAQA        = 15,     // (by RK chi2 * RK RICH match quality)

		  kIsAcceptedHitRICH     = 16,    // remember if the hit was been taken into account
		  kIsAcceptedHitInnerMDC = 17,    //
		  kIsAcceptedHitOuterMDC = 18,    //
		  kIsAcceptedHitMETA     = 19,    //
		  kIsAcceptedRKMETA      = 20,    //
		  kIsAcceptedRKRICH      = 21,    //
		  kIsAcceptedRK          = 22,    //
		  kIsAcceptedSPLINE      = 23,    //
		  kIsAcceptedKICK        = 24,    //
		  kIsAcceptedRKRKMETA    = 25,    //
		  kIsAcceptedRKRKRICH    = 26,    //
                  kIsAcceptedRKMETAQA    = 27,    //
                  kIsLepton              = 29,    //  mark the selected leptons
		  kIsUsed                = 30,    //  mark the object as arleady used
		  kIsRejected            = 31     //  mark the object as rejected

  };


  void   setFlagBit   (EflagBits bit)  { flags |=  ( 0x01 << bit ); }
  void   unsetFlagBit (EflagBits bit)  { flags &= ~( 0x01 << bit ); }
  Bool_t isFlagBit    (EflagBits bit)  { return (flags >> bit ) & 0x01 ; }
  void   setFlagBit   (Int_t bit);
  void   unsetFlagBit (Int_t bit);
  Bool_t isFlagBit    (Int_t bit);
  void   setFlagBitByValue(EflagBits bit, Bool_t val)  { val ? setFlagBit(bit) : unsetFlagBit(bit); }
  void   setFlagBitByValue(Int_t     bit, Bool_t val)  { val ? setFlagBit(bit) : unsetFlagBit(bit); }
  Bool_t isFlagDoubleHit ()            { return (flags & 0xF) == 0x0 ? kFALSE : kTRUE; }
  Bool_t isFlagAllBestHit()            { return ((flags >> 4 ) & 0xF) == 0xF ? kTRUE : kFALSE; }
  Bool_t isFlagNoBestHit ()            { return ((flags >> 4 ) & 0xF) == 0x0 ? kTRUE : kFALSE; }
  Int_t  getFlagField()                { return flags;  }
  void   setFlagField(Int_t field)     { flags = field; }
  Bool_t isFlagAND(Int_t num, ...);
  Bool_t isFlagOR (Int_t num, ...);
  Int_t  getDoubleHitsLeptons(){ return (flags & 0xF);}
  Int_t  getDoubleHitsHadrons(){ return ((flags >> 1) & 0x7);}
  void   printFlags(TString comment="");
  void   printInfo();
  Bool_t select(Bool_t (*function)(HPidTrackCand* )) { return  (*function)(this); }

  // -------------------------------------------------------------------------



  // -------------------------------------------------------------------------

  //Overwrite base class function
  virtual void       Clear(Option_t * /*option*/ = "");

  // -------------------------------------------------------------------------

  //reports content of this object
  void               print(void) const;
  void               Print(Option_t *o = "") const { print();                   }

  HPidTrackData*     getTrackData(void) {return &itsTrackData;}
  HPidHitData*       getHitData(void) {return &itsHitData;}
  
  // -------------------------------------------------------------------------
  
  static HCategory*  buildPidTrackCandCategory(void);
  // -------------------------------------------------------------------------
  
  ClassDef(HPidTrackCand, 2)  // Track candidate for PID
};
    
#endif //HPIDTRACKCAND_H
