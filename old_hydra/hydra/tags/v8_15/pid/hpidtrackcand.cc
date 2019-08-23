// @(#)$Id: hpidtrackcand.cc,v 1.13 2007-08-06 14:04:57 kempter Exp $
//*-- Author : Marcin Jaskula 28/06/2002
// Modified  : Marcin Jaskula 16/09/2002
//             - removed: HArrayS, OuterHits
//             - Rings array -> one Ring Id
// Modified  : Marcin Jaskula 31/10/2002
//           - buildPidTrackCategory moved from global to static function
// Modified  : Marcin Jaskula 10/11/2002
//           - get*Sim methods added
// Modified  : Marcin Jaskula 25/02/2003
//           - static functions to retrive the Hits, Rings and Tracks
//            Definition of getGeantTrackSet() changed !
// Restructured : Tassilo Christ 11.11.2003 for compliance with new pid/tracking scheme
//                Matching functionality is now performed by HMetaMatch class.
//                Category retrieval is now done ion HPidFL::getHitObject
//                Get/Set functions removed as all data members are public
//                this might change later on
//                The real data and the sim data class will be different in the future!
//                Thus the isSim bit has been removed
//

//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////////////////////
//
//
// HPidTrackCand
//
// In this container matched hits from all detectors are stored.
// The class does not define the algorithm how the matching is done.
// This is defined in HMetaMatch (coarse matching) and HPidTrackFiller (refined 
// matching taking into account momentum values.
//
// The structure of this class allows user to store information about
// the correlated hits as well as information about all hits in neighborhood
// of hits which contribute to the track candidate. The hits in
// the neighborhood are dedicated for identification close pairs
// as it was discussed at the PID subgroup meeting.
// ATTENTION: THE HANDLING OF CLOSE BY HITS IS NOT YET IMPKLEMENTED 
// THE CLASS ONLY STORES THE DISTANCE OF THE CLOSEST HADRON AND LEPTON CANDIDATES!
//
//
// -------------------------------------------------------------------------
// Track Cleaner flags
// To clean the PidTrackCand objects from unwanted fakes
// the object carries a bitfield (32 bit Int_t). The different
// qualitiy criteria can be used to select the best reconstructed
// candidate in the case of multiple use of single detector hits.
// The single bit numbers are defined by
//
//  enum EflagBits {kIsDoubleHitRICH       =  0,    //
//                  kIsDoubleHitInnerMDC   =  1,    //
//                  kIsDoubleHitOuterMDC   =  2,    //
//	            kIsDoubleHitMETA       =  3,    //
//
//	            kIsBestHitRICH         =  4,    //  (by number of pads)
//	            kIsBestHitInnerMDC     =  5,    //  (by chi2)
//	            kIsBestHitOuterMDC     =  6,    //  (by chi2)
//	            kIsBestHitMETA         =  7,    //  (by META match quality)
//
//	            kIsBestRKMETA          =  8,    //  (by RK META match quality)
//                  kIsBestRKRICH          =  9,    //  (by RK RICH match quality)
//	            kIsBestRK              = 10,    //  (by RK chi2, none fitted outer segments with lower priority)
//		    kIsBestSPLINE          = 11,    //  (by chi2)
//	            kIsBestKICK            = 12,    //  (by pull)
//	            kIsBestRKRKMETA        = 13,    //  (by RK chi2 * RK META match quality)
//	            kIsBestRKRKRICH        = 14,     // (by RK chi2 * RK RICH match quality)
//	            kIsBestRKMETAQA        = 15,     // (by RK chi2 * META match quality)
//
//	            kIsAcceptedHitRICH     = 16,    // remember if the hit was been taken into account
//	            kIsAcceptedHitInnerMDC = 17,    //
//                  kIsAcceptedHitOuterMDC = 18,    //
//	            kIsAcceptedHitMETA     = 19,    //
//	            kIsAcceptedRKMETA      = 20,    //
//	            kIsAcceptedRKRICH      = 21,    //
//	            kIsAcceptedRK          = 22,    //
//	            kIsAcceptedSPLINE      = 23,    //
//	            kIsAcceptedKICK        = 24,    //
//	            kIsAcceptedRKRKMETA    = 25,    //
//	            kIsAcceptedRKRKRICH    = 26,    //
//	            kIsAcceptedRKMETAQA    = 27,    //
//                  kIsLepton              = 29,    //  mark the selected leptons
//	            kIsUsed                = 30,    //  mark the object as arleady used
//	            kIsRejected            = 31     //  mark the object as rejected
//
//  };
//
//  The selection of tracks and setting of the corresponding flags is performed
//  by HPidTrackCleaner and HPidTrackSorter classes (see the documentation).
//
//  For example void  setFlagBit(HPidTrackCand::kIsDoubleRICH) sets the bit flag that
//  the RICH hit has been used in more than one HPidTrackCand object.
//  Correspondingly one can ask for the flag with Bool_t isFlagBit(HPidTrackCand::kIsDoubleRICH)
//  void   setFlagBit(Int_t bit) and  Bool_t isFlagBit(Int_t bit) works for the bitnumber (0-31) specified
//  manually (if one knows what one is doing....)
//  void   setFlagBit   (EflagBits bit)
//  void   unsetFlagBit (EflagBits bit)
//  Bool_t isFlagBit    (EflagBits bit)
//  void   setFlagBit   (Int_t bit);
//  void   unsetFlagBit (Int_t bit);
//  Bool_t isFlagBit    (Int_t bit);
//  void   setFlagBitByValue(EflagBits bit, Bool_t val) sets/unsets bit depending on val
//  void   setFlagBitByValue(Int_t     bit, Bool_t val) sets/unsets bit depending on val
//  Bool_t isFlagDoubleHit ()           check all double hits flags simultanously (false if none set)
//  Bool_t isFlagAllBestHit()           check all best hit flags simultanously    (true if all are set)
//  Int_t  getDoubleHitsLeptons() returns the 4 bit double hit word as int
//  Int_t  getDoubleHitsHadrons() returns the 3 bit double hit word (excluding RICH) as int
//  Bool_t isFlagNoBestHit ()           check all best hit flags simultanously    (true if none set)
//  Int_t  getFlagField()               do what you want with flags field
//  Bool_t isFlagAND(Int_t num,Int_t fl, ...) check all flags in argument simultanously (false if none set)
//  Bool_t isFlagOR(Int_t num,Int_t fl, ...)  check all flags in argument simultanously (true if any set)
//  void   printFlags(TString comment="") prints all flags in binary representation.
//  void   printInfo()                    prints indices of detector hits + quality criteria + flags
//
//  The function
//  Bool_t select(Bool_t (*function)(HPidTrackCand* ))  { return  (*function)(this); }
//  taking a pointer to function with argument HPidTrackCand* returning a Bool_t allows
//  todo complex selection on the HPidTrackCand object as one has
//  full access to all members of HPidTrackCand inside the test function.
//  The function pointer can be a pointer to a global function or member function
//  of an Object for example:
//
//  Bool_t test(HPidTrackCand* cand){               // global function
//           if ( put all selection criteria here ) return kTRUE;
//           else                                   return kFALSE;
//  } or
//
//  static Bool_t dummy::test(HPidTrackCand* cand){   // member function of object dummy
//                                                    // needs to be declared static !
//           if ( put all selection criteria here ) return kTRUE;
//           else                                   return kFALSE;
//  }
//  would be called in the code like
//
//  dummy d;
//  HPidTrackCand* p= new HPidTrackCand() // just to get an object
//  p->select(test))        // global function
//  p->select(dummy::test)) // member function of object dummy (static call without object creation)
//  p->select(d.test))      // member function of real object dummy
//
//
////////////////////////////////////////////////////////////////////////////////

#pragma implementation

#include "hpidtrackcand.h"
#include <iostream>
#include <iomanip>

#include "hmetamatch.h"
#include "hbasetrack.h"
#include "hpidfl.h"

#include "hades.h"
#include "hevent.h"
#include "hlinearcategory.h"

#include "hmdctrackgdef.h"
#include "kickdef.h"

#include "TError.h"
#include "TTree.h"
#include "hpidhitdata.h"
#include "hbasetrack.h"

// -----------------------------------------------------------------------------

ClassImp(HPidTrackCand)
  
  // -----------------------------------------------------------------------------

HPidTrackCand::HPidTrackCand(void)
{
  // Default HPidTrackCand ctor.
  
  Clear();
}

//Constructor
HPidTrackCand::HPidTrackCand(HMetaMatch* pMetaMatch, Int_t iMatchIndex)
{
  Clear();
  if(pMetaMatch->getSector()>5 || pMetaMatch->getSector()<0)
    {
      Error("HPidTrackCand::HPidTrackCand(HMetaMatch* p,Int_t i)","Nonsense sector in constructor of PidTrackCand");
    }
  itsHitData.fillFromMetaMatch(pMetaMatch, iMatchIndex);
  itsTrackData.fillFromMetaMatch(pMetaMatch);
}

void   HPidTrackCand::setFlagBit   (Int_t bit)
{
    // set given bit in flag (0-32)
    if(bit >= 0 && bit < 32 ) flags |=  ( 0x01 << bit );
    else {
	Error("HPidTrackCand::setFlagBit(Int_t)","Bit number out of range : %i!",bit);
    }
}

void   HPidTrackCand::unsetFlagBit (Int_t bit)
{
    // unset given bit in flag (0-32)
    if(bit >= 0 && bit < 32 ) flags &= ~( 0x01 << bit );
    else {
	Error("HPidTrackCand::unsetFlagBit(Int_t)","Bit number out of range : %i!",bit);
    }
}

Bool_t HPidTrackCand::isFlagBit    (Int_t bit)
{
    // check given bit in flag (0-32)
    // return kTRUE if bit is set
    if (bit >= 0 && bit < 32 )  return (flags >> bit ) & 0x01;
    else {
	Error("HPidTrackCand::isFlagBit(Int_t)","Bit number out of range : %i!",bit);
        return kFALSE;
    }
}

Bool_t HPidTrackCand::isFlagOR(Int_t num,...)
{
    // check given number of bits in argument in flag (0-32)
    // return kTRUE if any bit is set
    va_list ap;
    va_start(ap,num);

    for(Int_t i=0;i<num;i++){

	Int_t bit=va_arg(ap,Int_t);
	if(bit < 0 || bit > 31)
	{
	    Error("HPidTrackCand::isFlagOR()","Bit number out of range : i=%i ,%i!",i,bit);
	    va_end(ap);
	    return kFALSE;
	}
	if(isFlagBit(bit)) {
	    va_end(ap);
	    return kTRUE;
	}
    }
    va_end(ap);
    return kFALSE;
}
Bool_t HPidTrackCand::isFlagAND(Int_t num, ...)
{
    // check given number of bits in argument in flag (0-32)
    // return kTRUE if all bits are set
    va_list ap;
    va_start(ap,num);

    for(Int_t i=0;i<num;i++){
        Int_t bit=va_arg(ap,Int_t);
	if(bit < 0 || bit > 31)
	{
	    Error("HPidTrackCand::isFlagAND()","Bit number out of range : i=%i ,%i!",i,bit);
	    va_end(ap);
	    return kFALSE;
	}
	if(!isFlagBit(bit)) {
	    va_end(ap);
	    return kFALSE;
	}
    }
    va_end(ap);
    return kTRUE;
}

void HPidTrackCand::printFlags(TString comment)
{
    // print the flag field in binary representation
    // Comment will be printed at the end of line

    TString out="";
    for(Int_t i=32;i>0;i--){
	if(i%4==0) out+=" ";
	out+= ( (flags>>i-1) & 0x1 );
    }
    cout<<"bin "<<out.Data()<<" "<<comment.Data()<<endl;
}
void HPidTrackCand::printInfo()
{
    // print one candidate object and the flags which have been
    // already set to HPidTrackCand
    cout<<"    "<<" --------------------------------------------"<<endl;
    cout<<"    "
	<<"sec: "  <<itsHitData.nSector
	<<", R: "  <<itsHitData.getIndRICH()
	<<", iM: " <<itsHitData.getIndInnerSeg()
	<<", oM: " <<itsHitData.getIndOuterSeg()
	<<", T: "  <<itsHitData.getIndTOF()
	<<", S: "  <<itsHitData.getIndShower()
	<<endl;
    cout<<"    "
	<<"Ring "     <<itsHitData.getRingPadNr()
	<<", ichi2: " <<itsHitData.getInnerMdcChiSquare()
	<<", ochi2: " <<itsHitData.getOuterMdcChiSquare()
	<<", MQA: "   <<itsTrackData.getMetaMatchingQuality()<<endl;

    cout<<"    "
	<<"RKM: "       <<itsTrackData.getRKMetaMatchingQuality()
	<<", RKR: "     <<itsTrackData.getRKRichMatchingQuality()
        <<", RK: "      <<itsTrackData.getRKChiSquare()
	<<", SPL: "     <<itsTrackData.getSplineChiSquare()
	<<", KICK: "    <<itsTrackData.getPull()
	<<", isRingRK: "<<(Int_t)itsHitData.getRingCorrelation(ALG_RUNGEKUTTA)
	<<endl;

    cout<<"    "<<flush;
    printFlags();
}


// -----------------------------------------------------------------------------
// Clear all members - Overwrite TObject::Clear(Option *)
void HPidTrackCand::Clear(Option_t * popt)
{
  itsTrackData.reset();
  itsHitData.reset();
  flags = 0;
}

// -----------------------------------------------------------------------------

void HPidTrackCand::print(void) const
{
  itsHitData.print();
  itsTrackData.print();
}

// -----------------------------------------------------------------------------
// global function for making the category
HCategory* HPidTrackCand::buildPidTrackCandCategory(void)
{
  HCategory  *pCat;
  static char sCatName[] = "HPidTrackCand";
  HEvent     *pEvent;
  
  if((gHades == NULL) || ((pEvent = gHades->getCurrentEvent()) == NULL))
    {
      ::Error("HPidTrackCand::buildPidTrackCategory",
	      "Cannot access current event");
      
      return NULL;
    }
  
  if((pCat = pEvent->getCategory(catPidTrackCand)) != NULL)
    {
      //This category has objects of always the same type but sometimes different size!
      //This is indicated by the "dynamic objects flag"
      ((HLinearCategory*)pCat)->setDynamicObjects(kTRUE);
      return pCat;
    }
  
  if((pCat = new HLinearCategory(sCatName, 1000)) == NULL)
    {
      ::Error("HPidTrackCand::buildPidTrackCategory",
	      "Cannot create new category");
      
      return NULL;
    }
  ((HLinearCategory*)pCat)->setDynamicObjects(kTRUE);
  pEvent->addCategory(catPidTrackCand, pCat, "Pid");
  
  return pCat;
}

void HPidTrackCand::Streamer(TBuffer &R__b)
{
   // Stream an object of class HPidTrackCand.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      itsHitData.Streamer(R__b);
      itsTrackData.Streamer(R__b);
      if (R__v >= 2)
      {
	  R__b >> flags;
      } else flags = 0;
      R__b.CheckByteCount(R__s, R__c, HPidTrackCand::IsA());
   } else {
      R__c = R__b.WriteVersion(HPidTrackCand::IsA(), kTRUE);
      TObject::Streamer(R__b);
      itsHitData.Streamer(R__b);
      itsTrackData.Streamer(R__b);
      R__b << flags;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

// -----------------------------------------------------------------------------


