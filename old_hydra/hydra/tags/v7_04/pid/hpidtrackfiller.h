// @(#)$Id: hpidtrackfiller.h,v 1.14 2005-01-26 11:30:12 wojcik Exp $
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

using namespace std;
#ifndef HPIDTRACKFILLER_H
#define HPIDTRACKFILLER_H


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// HPidTrackFiller                                                      //
//                                                                      //
// A simple track filler of HPidTrackCand category                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "hlocation.h"
#include "hreconstructor.h"
#include "piddef.h"
#include <TMath.h>
#include "TArrayI.h"
// -----------------------------------------------------------------------------


#define DEF_ARRAY_SIZE 10000
#define MAX_OVER_SIZE  50

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
                HPidTrackFiller(Option_t pOpt[] = "");
                HPidTrackFiller(Text_t name[], Text_t title[],
                                    Option_t pOpt[] = "");
               ~HPidTrackFiller(void);

    // -------------------------------------------------------------------------

    virtual Int_t   execute(void);
    virtual Bool_t  init(void);
    virtual Bool_t  reinit(void);
    virtual Bool_t  finalize(void);

    // -------------------------------------------------------------------------

    void            setMatchingConditions(Option_t *pParams);

    void            setSkipNoLepton(Bool_t b = kTRUE)   { bSkipNoLepton = b;   }
    Bool_t          getSkipNoLepton(void) const         { return bSkipNoLepton;}

    void            setSkipTofinoMult(Bool_t b = kTRUE) { bSkipTofinoMult = b; }
    Bool_t          getSkipTofinoMult(void) const   { return bSkipTofinoMult;}

    void            setChi2Selection(Int_t nSeg,Bool_t b = kTRUE) { bNoChi2Selection[nSeg] = b; }
    Bool_t          getChi2Selection(Int_t nSeg) const   { return bNoChi2Selection[nSeg];}

    // -------------------------------------------------------------------------

    virtual void    Print(Option_t *o = "") const   { print();              }
    void            print(void) const;

protected:

    // -------------------------------------------------------------------------

    void            setDefault(void);

    HPidTrackCand*         getNextSlot(void);
    virtual HPidTrackCand* getNextSlot(HMetaMatch *pMatch);

    // -------------------------------------------------------------------------

    Bool_t          checkTofinoMult(HMetaMatch *pMatch);
    Bool_t          checkForLepton(HMetaMatch *pMatch);
	Bool_t			checkChi2(HMetaMatch *pMatch);

    void            assignRingCorrelations(HPidTrackCand* pCand);

    HCategory*       buildOutputCategory(void);

    // -------------------------------------------------------------------------
    
    HCategory  *pCatMatch;      // HMetaMatch category
    HCategory  *pCatTrkCand;    // HMdcTrkCand category
    HCategory  *pCatMdcSeg;     // HdcSeg category
	HCategory  *pCatTof;        // HTofHit category
	HCategory  *pCatShower;     // HShowerTofinoHit category
    HCategory  *pCatCand;       // output category

    HLocation   lTrack;         // location used in execute for HBaseTrack

    HIterator  *pitCand;        // iterator: output category
    HIterator  *pitMatch;       // iterator: input category

    HPidTrackFillerPar* pTrackFillerParam;

    Bool_t      bInitOk;        // set kTRUE if last init was ok

    Bool_t      bSkipNoLepton;      // skip event if no lepton track
    Bool_t      bSkipTofinoMult;   // skip track if tofino mult. > 1
    Bool_t      bMakeSimCategory;  //create HPidTrackCandSIim category instead of HPidTrackCand
	Bool_t		bNoChi2Selection[2];  // if kTRUE, do not remove track with chi^2<0, nSeg=0 inner,nSeg=1 outer segment


    TArrayI     trkcandindices;
    TArrayI     metahitindices;
    TArrayI     systems;
    TArrayI     sectors;
    TArrayI     multiplicities;
    Int_t       nTriples;
	Int_t       nAccTrack;
	Int_t       nRejTrack;
	Int_t       nTotalTrack;

    // -------------------------------------------------------------------------

public:

    // -------------------------------------------------------------------------

    ClassDef(HPidTrackFiller, 0)  // PID track candidate filler
};

// -----------------------------------------------------------------------------



#endif //HPIDTRACKFILLER_H
