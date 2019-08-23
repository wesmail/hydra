// @(#)$Id: hpidtrackfiller.h,v 1.23 2009-07-15 11:40:30 halo Exp $
//*-- Author  : Marcin Jaskula 29/06/2002
//*-- Modified: Marcin Jaskula 09/10/2002
//              - HPidTrackFillerPar used
//*-- Modified: Marcin Jaskula 31/10/2002
//              - getMdc* moved from global to static function
//*-- Modified: Marcin Jaskula and Jacek Otwinowski 05/02/2003
//              - checkTofinoMult() removed bacause of HPidTofinoTrackFilter
//*-- Modified: Marcin Jaskula 14/03/2003
//              - meta overlap added
//Restructured: Tassilo Christ 11.11.03 to comply with new pid/tracking scheme
//              based on HBaseTrack class
//Documentation upgrade 05/02/2005 by Tassilo Christ

using namespace std;
#ifndef HPIDTRACKFILLER_H
#define HPIDTRACKFILLER_H


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// HPidTrackFiller                                                      //
//                                                                      //
// This reconstructor builds the starter-category for particle ID       //
// It reads objects of type HMetaMatch and creates the hit/track data   //
// collection required for particle ID.                                 //
// Further it checks for MOMENTUM-dependent correlations between MDC    //
// RICH and shifts the ring-position according to the computed          //
// interaction vertex.                                                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "hlocation.h"
#include "hreconstructor.h"
#include "piddef.h"
#include "TMath.h"
#include "TArrayI.h"
// -----------------------------------------------------------------------------


//#define DEF_ARRAY_SIZE 10000
//#define MAX_OVER_SIZE  50

// -----------------------------------------------------------------------------
class HCategory;
class HIterator;
class HMetaMatch;
class HPidTrackCand;
class HEvent;
class HPidTrackFillerPar;

// -----------------------------------------------------------------------------

class HPidTrackFiller : public HReconstructor
{
public:
  HPidTrackFiller(const Option_t pOpt[] = "");
  HPidTrackFiller(const Text_t name[],const Text_t title[],const Option_t pOpt[] = "");
  ~HPidTrackFiller(void);
  
  // -------------------------------------------------------------------------

  //all tasks must have this
  virtual Int_t   execute(void); 
  virtual Bool_t  init(void);    
  virtual Bool_t  reinit(void);  
  virtual Bool_t  finalize(void);

    // -------------------------------------------------------------------------

  //Parse the option string for the condition to apply to a HMetaMatch before creating
  //a HPidTrackCand from it
  void            setMatchingConditions(const Option_t *pParams);

  //getters and setters won't be documented unless required
  void            setSkipNoLepton(Bool_t b = kTRUE)   { bSkipNoLepton = b;   }
  Bool_t          getSkipNoLepton(void) const         { return bSkipNoLepton;}

  void            setSkipTofinoMult(Bool_t b = kTRUE) { bSkipTofinoMult = b; }
  Bool_t          getSkipTofinoMult(void) const   { return bSkipTofinoMult;}
  
  void            setChi2Selection(Int_t nSeg,Bool_t b = kTRUE) { bNoChi2Selection[nSeg] = b; }
  Bool_t          getChi2Selection(Int_t nSeg) const   { return bNoChi2Selection[nSeg];}

  void            setSkipNoOuterSegAndNoMeta(Bool_t b = kTRUE) { bSkipNoOuterSegAndNoMeta = b; }
  Bool_t          getSkipNoOuterSegAndNoMeta(void) const       { return bSkipNoOuterSegAndNoMeta; }
  // -------------------------------------------------------------------------
  
  virtual void    Print(const Option_t *o = "") const   { print();              }
  void            print(void) const;
  
 protected:
  
  // -------------------------------------------------------------------------

  //Set default behaviour of the track filler and set pointer to 0
  //Defaults of task are: All options kFALSE
  void            setDefault(void);
  

  //Get next slot in output category to store new objects
  HPidTrackCand*         getNextSlot(void);
  virtual HPidTrackCand* getNextSlot(HMetaMatch *pMatch);
  
  // -------------------------------------------------------------------------

  //True if whether tofino multiplicity is <= 1
  Bool_t          checkTofinoMult(HMetaMatch *pMatch);

  //Checks whether a HMetaMatch has a ring or not - required to decide about skipping events
  Bool_t          checkForLepton(HMetaMatch *pMatch);

  //Returns true if either all chi^2 are >=0 or the checking is disabled
  Bool_t	  checkChi2(HMetaMatch *pMatch);
  
  //Returns the set of polynomial coefficients required to compute the analytically parametrized ring shift
  //in polar angle which is due to a deviation of the true vertex from the target center for which the
  //mapping angle<->pad is parametrized
  void            getPolynomialCoefficients(Float_t *coefficients, Int_t sector);

  //Applies the polynomial correction to the ring position. This is NOT done on the HRichHit object but
  //on the HPidHitData object!
  Float_t         correctRingShift(HPidTrackCand *pCand);

  //Whether or nor a ring is correlated to a segment depends on the chosen tracking algorithm and the
  //momentum delivered by it, because the correlation windows are momentum dependent
  void            assignRingCorrelations(HPidTrackCand* pCand);
  
  //Depending on the sophistication of the tracking we might or might not accept the meta/mdc correlation
  void            assignMetaCorrelations(HPidTrackCand* pCand);
  
  //Compute the angular distance of an inner segment in pMetaMatch and pCand
  Float_t         getAngularDistance(HMetaMatch* pMetaMatch, HPidTrackCand* pCand);

  //required to make HPidTrackCand output category if not yet present in the HEvent structure
  HCategory*       buildOutputCategory(void);

  // -------------------------------------------------------------------------
  
  //Pointers to input and output categories
  HCategory  *pCatMatch;      // HMetaMatch category
  HCategory  *pCatTrkCand;    // HMdcTrkCand category
  HCategory  *pCatMdcSeg;     // HdcSeg category
  HCategory  *pCatTof;        // HTofHit category
  HCategory  *pCatShower;     // HShowerTofinoHit category
  HCategory  *pCatCand;       // output category
  
  HLocation   lTrack;         // location used in execute for HBaseTrack
  
  HIterator  *pitCand;        // iterator: output category
  HIterator  *pitMatch;       // iterator: input category
  
  //parameter container with momentum dependent correlation windows and ring-shift parametrization
  HPidTrackFillerPar* pTrackFillerParam;
  
  Bool_t      bInitOk;        // set kTRUE if last init was ok

  //Boolean flags configuring the behaviour of the task
  Bool_t      bSkipNoLepton;      // skip event if no lepton candidate (ring+segment) was found

  Bool_t      bSkipTofinoMult;    // skip track if tofino mult. > 1

  Bool_t      bMakeSimCategory;   //create HPidTrackCandSIim category instead of HPidTrackCand

  Bool_t      bNoChi2Selection[2];  // if kTRUE, do not remove track with chi^2<0, nSeg=0 inner,nSeg=1 outer segment

  Bool_t      bSkipNoOuterSegAndNoMeta;  // if kTRUE, remove track candidates which have no outer MDC seg and no Meta hit
  
  //Bool_t      bUsePlutoGenerator; //Use pluto parent/grandparent info when searching for ancestors of particle
  /*
  TArrayI     trkcandindices;
  TArrayI     metahitindices;
  TArrayI     systems;
  TArrayI     sectors;
  TArrayI     multiplicities;
  Int_t       nTriples;
  */
  
  Int_t       nAccTrack;      //Number of accepted tracks
  Int_t       nRejTrack;      //Number of rejected tracks
  Int_t       nTotalTrack;    //Total number of analyzed tracks
  Float_t     meanDevChange;  //Mean change in deviation of ring and track
  Int_t       nShifts;        //Number of rings shifted
  // -------------------------------------------------------------------------
  
 public:
  
  // -------------------------------------------------------------------------
  
  ClassDef(HPidTrackFiller, 0)  // PID track candidate filler
    };
    
    // -----------------------------------------------------------------------------
    
    
    
#endif //HPIDTRACKFILLER_H
