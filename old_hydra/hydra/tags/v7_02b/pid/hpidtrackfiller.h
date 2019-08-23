// @(#)$Id: hpidtrackfiller.h,v 1.12 2004-05-25 11:51:23 wojcik Exp $
//*-- Author  : Marcin Jaskula 29/06/2002
//*-- Modified: Marcin Jaskula 09/10/2002
//              - HPidTrackFillerPar used
//*-- Modified: Marcin Jaskula 31/10/2002
//              - getMdc* moved from global to static function
//*-- Modified: Marcin Jaskula and Jacek Otwinowski 05/02/2003
//              - checkTofinoMult() removed bacause of HPidTofinoTrackFilter
//*-- Modified: Marcin Jaskula 14/03/2003
//              - meta overlap added

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

#include <TMath.h>

// -----------------------------------------------------------------------------

#define MAX_SECTOR 6
#define DEF_ARRAY_SIZE 10000
#define MAX_OVER_SIZE  50

// -----------------------------------------------------------------------------

class HCategory;
class HIterator;
class HKickTrack;
class HPidTrackCand;
class HPidTrackFillerPar;
class HEvent;
class HShowerHitTof;

// -----------------------------------------------------------------------------

class HPidMetaOverlap
{
public:
    Int_t   iMdcSeg;
    Float_t fTofTheta;
    Float_t fTofPhi;
};

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

    Bool_t          checkTofinoMult(HKickTrack *pTrack);

    // -------------------------------------------------------------------------

    void            setParameters(Option_t *pParams);

    void            setNoMdc(Bool_t b = kTRUE)      { bNoMdc = b;           }
    Bool_t          getNoMdc(void) const            { return bNoMdc;        }

    void            setNoRich(Bool_t b = kTRUE)     { bNoRich = b;          }
    Bool_t          getNoRich(void) const           { return bNoRich;       }

    void            setSkipNoLepton(Bool_t b = kTRUE)   { bSkipNoLepton = b;   }
    Bool_t          getSkipNoLepton(void) const         { return bSkipNoLepton;}

    void            setSkipTofinoMult(Bool_t b = kTRUE) { bSkipTofinoMult = b; }
    Bool_t          getSkipTofinoMult(void) const   { return bSkipTofinoMult;}

    void            setMetaOverlap(Bool_t b = kTRUE)    { bMetaOverlap = b;    }
    Bool_t          getMetaOverlap(void) const          { return bMetaOverlap; }

    // -------------------------------------------------------------------------

    virtual void    Print(Option_t *o = "") const   { print();              }
    void            print(void) const;

protected:

    // -------------------------------------------------------------------------

    virtual Bool_t  getCategories(HEvent *pEvent);

    // -------------------------------------------------------------------------

    void            setDefault(void);

    virtual HPidTrackCand*         getNextSlot(void);
    virtual HPidTrackCand* getNextSlot(HKickTrack *pTrack);

    void            calculateCounts(void);

    // -------------------------------------------------------------------------

    HShowerHitTof*  getShowerHitTof(Int_t iId) const;

    void            fillMetaOverlaps(void);
    Bool_t          checkMetaOverlap(HKickTrack *pTrack);

    // -------------------------------------------------------------------------

    HPidTrackFillerPar* pParams;

    Short_t     aTrackCounts[MAX_SECTOR][DEF_ARRAY_SIZE]; // counter of tracks
    Short_t     aMetaCounts[2][DEF_ARRAY_SIZE];           // counter of META
    Short_t     aMdcCounts[MAX_SECTOR][DEF_ARRAY_SIZE];   // counter of MDC
    Short_t     aRichCounts[DEF_ARRAY_SIZE];              // counter of rings

    HCategory  *pCatTrack;      // KickTrack category
    HCategory  *pCatMdc;        // MdcSeg category
    HCategory  *pCatRich;       // Rich category

    HCategory  *pCatCand;       // output category

    HLocation   lTrack;         // location used in the execute for HKickTrack
    HLocation   lMdc;           // location used in the execute for HMdcSeg

    HIterator  *pitTrack;       // iterator: kicktrack
    HIterator  *pitMdc;         // iterator: mdc segments
    HIterator  *pitMdcN;        // iterator: mdc segments in neighborhood
    HIterator  *pitRich;        // iterator: rings
    HIterator  *pitCand;        // iterator: output category

    Bool_t      bInitOk;        // set kTRUE if last init was ok

    Bool_t      bNoMdc;         // if MDC matching should be done
    Bool_t      bNoRich;        // if RICH matching should be done
    Bool_t      bSkipNoLepton;  // skip event if no lepton track
    Bool_t      bSkipTofinoMult;// skip track if tofino mult. > 1
    Bool_t      bMetaOverlap;   // skip hits from META which overlap

    Cat_t       cCatShowerTof;  // id of category HShowerHitTof(Sim)

    // -------------------------------------------------------------------------

    HPidMetaOverlap acOverlap[MAX_SECTOR][MAX_OVER_SIZE];
    Int_t           aiOverlapCount[MAX_SECTOR];

    // -------------------------------------------------------------------------

public:

    static Float_t getMdcPhi(Int_t iSector, Float_t fPhiMdc);
    static Float_t getMdcTheta(Float_t fThetaMdc);

    // -------------------------------------------------------------------------

    ClassDef(HPidTrackFiller, 0)  // PID track candidate filler
};

// -----------------------------------------------------------------------------

inline Float_t HPidTrackFiller::getMdcPhi(Int_t iSector, Float_t fPhiMdc)
{
// Convert MDC's phi angle to the coordinate system used in other detectors

Float_t fPhi = TMath::RadToDeg() * fPhiMdc;

    switch(iSector)
    {
        case 0:
            break;

        case 1:
        case 2:
        case 3:
        case 4:
            fPhi += 60.0f * iSector;
            break;

        default:
            fPhi -= 60.0f;
            break;
    }

    return fPhi;
}

// -----------------------------------------------------------------------------

inline Float_t HPidTrackFiller::getMdcTheta(Float_t fThetaMdc)
{
// Convert MDC's theta angle to the coordinate system used in other detectors

    return  TMath::RadToDeg() * fThetaMdc;
}

// -----------------------------------------------------------------------------

#endif //HPIDTRACKFILLER_H
