// @(#)$Id: hpidtrackfiller.h,v 1.3 2002-11-20 17:20:50 jaskula Exp $
//*-- Author  : Marcin Jaskula 29/06/2002
//*-- Modified: Marcin Jaskula 09/10/2002
//              - HPidTrackFillerPar used
//*-- Modified: Marcin Jaskula 31/10/2002
//              - getMdc* moved from global to static function

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

// -----------------------------------------------------------------------------

class HCategory;
class HIterator;
class HKickTrack;
class HPidTrackCand;
class HPidTrackFillerPar;
class HEvent;

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

protected:

    // -------------------------------------------------------------------------

    virtual Bool_t  getCategories(HEvent *pEvent);

    // -------------------------------------------------------------------------

    void            setDefault(void);

    HPidTrackCand*         getNextSlot(void);
    virtual HPidTrackCand* getNextSlot(HKickTrack *pTrack);

    void            calculateCounts(void);

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

    HLocation   lTrack;         // location used in the execute

    HIterator  *pitTrack;       // iterator: kicktrack
    HIterator  *pitMdc;         // iterator: mdc segments
    HIterator  *pitMdcN;        // iterator: mdc segments in neighborhood
    HIterator  *pitRich;        // iterator: rings
    HIterator  *pitCand;        // iterator: output category

    Bool_t      bInitOk;        // set kTRUE if last init was ok

    Bool_t      bNoMdc;         // if MDC matching should be done
    Bool_t      bNoRich;        // if RICH matching should be done

    Cat_t       cCatShowerTof;  // id of category HShowerHitTof(Sim)

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
