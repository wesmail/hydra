// @(#)$Id: hpidtrackcand.cc,v 1.3 2002-11-20 17:18:32 jaskula Exp $
//*-- Author : Marcin Jaskula 28/06/2002
// Modified  : Marcin Jaskula 16/09/2002
//             - removed: HArrayS, OuterHits
//             - Rings array -> one Ring Id
// Modified  : Marcin Jaskula 31/10/2002
//           - buildPidTrackCategory moved from global to static function
// Modified  : Marcin Jaskula 10/11/2002
//           - get*Sim methods added

////////////////////////////////////////////////////////////////////////////////
//
// HPidTrackCand
//
// In this container matched hits from all detectors are stored.
// The class do not define the algorithm how the mathcing is done.
// It should be defined in a correlator which fills the container.
//
// The structure of this class allow user to store information about
// the correlated hits as well as information about all hits in neighborhood
// to hits which construct the root of the track candidate. The hits in
// the neighborhood are dedicated for identification close pairs
// as it was discussed at the PID subgroup meeting.
//
// The structure of the container is similar to the HKickTrack category.
// There is information about the sector and META hit like in the HKickTrack.
// The nKickTrackId keeps id of the kick track in the category. To get the
// HKickTrack from this information a call similar for getting HMdcSeg must
// be done:
// HLocation loc;
// loc.set(3,0,0,0);
// loc[0] = nSector;
// loc[1] = 0;
// loc[2] = nKickTrackId;
// category->getObject(loc);
//
// For Mdc's segs array of ids is dedicated. The idea was
// that the first id in the arrays ([0]) is used for building the track
// candidate. The rest of the ids in the arrays are used only as information
// for the close pairs and similar tasks.
//
// Methods getXXX are dedicate to make access to hits/rings/tracks in
// an easy way. They work only in gHades loop.
//
// Marker Sim (setSim(), getSim()) is chosen when the ids in the category
// stores ids for simulation objects. A proper getXXXSim should be used
// in this case.
//
// For every id of hit/ring/track a counter for this object is dedicated.
// They should store information in how many HPidTrackCand the hit/ring/track
// was used and make this information available for cleaning the tracks
// candidates.
// This is arbitrary if the track fillers (correlators) fill these members.
//
////////////////////////////////////////////////////////////////////////////////

#pragma implementation

#include "hpidtrackcand.h"

#include "hades.h"
#include "hevent.h"
#include "hlinearcategory.h"

#include "kickdef.h"
#include "hmdcdef.h"
#include "richdef.h"
#include "tofdef.h"
#include "showertofinodef.h"

#include "hshowerhittof.h"
#include "htofhit.h"

#include <TError.h>
#include <TTree.h>

// -----------------------------------------------------------------------------

#define ADD_COUNT(A)    { if(A < 0) A = 1; else ++A; }

// -----------------------------------------------------------------------------

ClassImp(HPidTrackCand)

// -----------------------------------------------------------------------------

HPidTrackCand::HPidTrackCand(void)
{
// Default HPidTrackCand ctor.

    Clear();
}

// -----------------------------------------------------------------------------

void HPidTrackCand::Clear(Option_t *)
{
// Clear all members
// Overwrite TObject::Clear(Option *)

    setSim(kFALSE);

    nSector         = -1;
    nKickTrackId    = -1;
    nKickTrackCount = -1;
    nSystem         = -1;
    nMetaId         = -1;
    nMetaCount      = -1;

    nRingId         = -1;
    nRingCount      = -1;

    unInnerMdcNumber = 0;
    for(Int_t i = 0; i < MAX_MDC_SEG; i++)
    {
        aInnerMdcIds[i]    = -1;
        aInnerMdcCounts[i] = -1;
    }
}

// -----------------------------------------------------------------------------

void HPidTrackCand::addKickTrackCount(void)
{
// Increase nKickTrackCount by 1 or set it to 1 when not set

    ADD_COUNT(nKickTrackCount);
}

// -----------------------------------------------------------------------------

void HPidTrackCand::addRingCount(void)
{
// Increase nRingCount by 1 or set it to 1 when not set

    ADD_COUNT(nRingCount);
}

// -----------------------------------------------------------------------------

void HPidTrackCand::addInnerMdcId(Short_t n)
{
// Set Mdc seg id in a specified position

    if(unInnerMdcNumber >= MAX_MDC_SEG)
    {
        Error("setInnerMdcId", "No free space in aInnerMdcIds array");
        return;
    }

    aInnerMdcIds[unInnerMdcNumber++] = n;
}

// -----------------------------------------------------------------------------

void HPidTrackCand::addInnerMdcCount(Int_t iP)
{
// Increase aInnerMdcCounts[iP] by 1 or set it to 1 when not set

    ADD_COUNT(aInnerMdcCounts[iP]);
}

// -----------------------------------------------------------------------------

void HPidTrackCand::addInnerMdcCountForId(Short_t nId)
{
// Increase aInnerMdcCounts for a specified id by 1 or set it to 1 when not set

    for(Int_t i = 0; i < MAX_MDC_SEG; i++)
    {
        if(aInnerMdcIds[i] == nId)
        {
            ADD_COUNT(aInnerMdcCounts[i]);
            return;
        }
    }

    Error("addInnerMdcCountForId", "No InnerMdc with id: %d", nId);
}

// -----------------------------------------------------------------------------

void HPidTrackCand::addMetaCount(void)
{
// Increase nMetaCount by 1 or set it to 1 when not set

    ADD_COUNT(nMetaCount);
}

// -----------------------------------------------------------------------------

void HPidTrackCand::print(void) const
{
// Print all fields of the track candidate

UInt_t i;

    printf("Type           : %s\n", (isSim()) ? "Sim" : "Real");
    printf("Sector         : %4d\n", nSector);
    printf("KickTrackId    : %4d (%4d)\n", nKickTrackId, nKickTrackCount);
    printf("RingId         : %4d (%4d)\n", nRingId, nRingCount);

    printf("InnerMdcIds(%2d):", unInnerMdcNumber);
    for(i = 0; i < unInnerMdcNumber; i++)
        printf(" %4d (%4d)", aInnerMdcIds[i], aInnerMdcCounts[i]);

    printf("\nMeta system    : %4d\n     id        : %4d (%4d)\n",
            nSystem, nMetaId, nMetaCount);
}

// -----------------------------------------------------------------------------

HCategory* HPidTrackCand::getCategory(Cat_t cat, Bool_t bWarning)
{
// Find HCategory if gHades->getCurrentEvent() is accessable

HEvent    *pEvent;
HCategory *pCat;

    if((gHades == NULL) || ((pEvent = gHades->getCurrentEvent()) == NULL))
    {
        if(bWarning)
        {
            ::Error("HPidTrackCand::getCategory",
                            "Cannot access current event");
        }

        return NULL;
    }

    if((pCat = pEvent->getCategory(cat)) == NULL)
    {
        if(bWarning)
            ::Error("HPidTrackCand::getCategory", "No category: %d", cat);
    }

    return pCat;
}

// -----------------------------------------------------------------------------

HKickTrack* HPidTrackCand::getKickTrack(HCategory *pCat) const
{
// Returns HKickTrack object corresponding to nKickTrackId (if exists)
// pCat must point on the catKickTrack category or gHades->getCurrentEvent()
// must be accessable
// In case catKickTrack is category of HKickTrackSim then the object of this
// class is returned

    if(nKickTrackId < 0)
        return NULL;

    if((pCat == NULL) && ((pCat = getCategory(catKickTrack)) == NULL))
    {
        Error("getKickTrack", "No catKickTrack category");
        return NULL;
    }

    return (HKickTrack *)pCat->getObject(nKickTrackId);
}

// -----------------------------------------------------------------------------

HKickTrackSim* HPidTrackCand::getKickTrackSim(HCategory *pCat) const
{
// Returns HKickTrackSim object corresponding to nKickTrackId (if exists)
// pCat must point on the catKickTrack category or gHades->getCurrentEvent()
// must be accessable.
// In case the HPidTrackCand is not Sim then an error is reported and
// the method returns NULL

    if( ! isSim())
    {
        Error("getKickTrackSim", "HPidTrackCand is not sim");
        return NULL;
    }

    return (HKickTrackSim *)getKickTrack(pCat);
}

// -----------------------------------------------------------------------------

HMdcSeg* HPidTrackCand::getMdcSeg(Int_t iPos, HCategory *pCat) const
{
// Returns HMdcSeg object corresponding to aInnerMdcIds[iPos] (if exists)
// pCat must point on the catMdcSeg category or gHades->getCurrentEvent()
// must be accessable.
// In case HPidTrackCand isSim() then HMdcSegSim is returned.

HLocation  loc;
Int_t      iId;

    if((iPos < 0) || (iPos >= unInnerMdcNumber)
            || ((iId = aInnerMdcIds[iPos]) < 0))
    {
        return NULL;
    }

    if((pCat == NULL) && ((pCat = getCategory(catMdcSeg)) == NULL))
    {
        Error("getMdcSeg", "No catMdcSeg category");
        return NULL;
    }

    loc.setNIndex(3);
    loc[0] = nSector;
    loc[1] = 0;
    loc[2] = iId;

    return (HMdcSeg *)pCat->getObject(loc);
}

// -----------------------------------------------------------------------------

HMdcSegSim* HPidTrackCand::getMdcSegSim(Int_t iPos, HCategory *pCat) const
{
// Returns HMdcSegSim object corresponding to aInnerMdcIds[iPos] (if exists)
// pCat must point on the catMdcSeg category or gHades->getCurrentEvent()
// must be accessable.
// In case the HPidTrackCand is not Sim then an error is reported and
// the method returns NULL

    if( ! isSim())
    {
        Error("getMdcSegSim", "HPidTrackCand is not sim");
        return NULL;
    }

    return (HMdcSegSim *)getMdcSeg(iPos, pCat);
}

// -----------------------------------------------------------------------------

HRichHit* HPidTrackCand::getRichHit(HCategory *pCat) const
{
// Returns HRichHit object corresponding to nRingId (if exists)
// pCat must point on the catRichHit category or gHades->getCurrentEvent()
// must be accessable
// In case HPidTrackCand isSim() then HRichHitSim is returned from catRichHitSim

    if((pCat == NULL) && ((pCat = getCategory(
                        catRichHit)) == NULL))
                        //(isSim()) ? catRichHitSim : catRichHit)) == NULL))
    {
        Error("getRichHit", "No %s category",
                        (isSim()) ? "catRichHitSim" : "catRichHit");

        return NULL;
    }

    return (HRichHit *)pCat->getObject((Int_t)nRingId);
}

// -----------------------------------------------------------------------------

HRichHitSim* HPidTrackCand::getRichHitSim(HCategory *pCat) const
{
// Returns HRichHitSim object corresponding to nRingId (if exists)
// pCat must point on the catRichHitSim category or gHades->getCurrentEvent()
// must be accessable.
// In case the HPidTrackCand is not Sim then an error is reported and
// the method returns NULL

    if( ! isSim())
    {
        Error("getRichHitSim", "HPidTrackCand is not sim");
        return NULL;
    }

    return (HRichHitSim *)getRichHit(pCat);
}

// -----------------------------------------------------------------------------

TObject* HPidTrackCand::getMetaHit(HCategory *pShwCat, HCategory *pTofCat) const
{
// Returns META hit object corresponding to META parameters
// in the container (if exists)
// If the system is SHOWER then pShwCat must point on the catShowerHitTof
// category or if system is TOF then pTofCat must point on the catTofHit.
// If the pointer is NULL then gHades->getCurrentEvent() must be accessable.
// In a case HPidTrackCand isSim() then proper sim obects are returned.

    if(nMetaId < 0)
        return NULL;

    switch(nSystem)
    {
        case 0:
            return getShowerHitTof(pShwCat);

        case 1:
            return getTofHit(pTofCat);

        case -1:
            break;

        default:
            Error("getMetaHit", "Unknown system: %d", nSystem);
            break;
    }

    return NULL;
}

// -----------------------------------------------------------------------------

HTofHit* HPidTrackCand::getTofHit(HCategory *pCat) const
{
// Returns HTofHit object corresponding to nMetaId (if exists)
// pCat must point on the catTofHit category or gHades->getCurrentEvent()
// must be accessable.
// In case HPidTrackCand isSim() then HTofHitSim is returned.

    if((nSystem != 1) || (nMetaId < 0))
        return NULL;

    if((pCat == NULL) && ((pCat = getCategory(catTofHit)) == NULL))
    {
        Error("getTofHit", "No catTofHit category");
        return NULL;
    }

    return (HTofHit *)pCat->getObject(nMetaId);
}

// -----------------------------------------------------------------------------

HTofHitSim* HPidTrackCand::getTofHitSim(HCategory *pCat) const
{
// Returns HTofHitTrack object corresponding to nMetaId (if exists)
// pCat must point on the catTofHit category or gHades->getCurrentEvent()
// must be accessable.
// In case the HPidTrackCand is not Sim then an error is reported and
// the method returns NULL

    if( ! isSim())
    {
        Error("getTofHitSim", "HPidTrackCand is not sim");
        return NULL;
    }

    return (HTofHitSim *)getTofHit(pCat);
}

// -----------------------------------------------------------------------------

HShowerHitTof* HPidTrackCand::getShowerHitTof(HCategory *pCat) const
{
// Returns HShowerHitTof object corresponding to nMetaId (if exists)
// pCat must point on the catShowerHitTof category or gHades->getCurrentEvent()
// must be accessable.
// In case HPidTrackCand isSim() then HShowerHitTofTrack is returned
// from catShowerHitTrack.

    if((nSystem != 0) || (nMetaId < 0))
        return NULL;

    if((pCat == NULL) && ((pCat = getCategory(
                        (isSim()) ? catShowerHitTofTrack : catShowerHitTof))
                        == NULL))
    {
        Error("getShowerHitTof", "No %s category",
                        (isSim()) ? "catShowerHitTofTrack" : "catShowerHitTof");

        return NULL;
    }

    return (HShowerHitTof *)pCat->getObject(nMetaId);
}

// -----------------------------------------------------------------------------

HShowerHitTofTrack* HPidTrackCand::getShowerHitTofTrack(HCategory *pCat) const
{
// Returns HShowerHitTofTrack object corresponding to nMetaId (if exists)
// pCat must point on the catShowerHitTrack category or
// gHades->getCurrentEvent() must be accessable.
// In case the HPidTrackCand is not Sim then an error is reported and
// the method returns NULL

    if( ! isSim())
    {
        Error("getShowerHitTofTrack", "HPidTrackCand is not sim");
        return NULL;
    }

    return (HShowerHitTofTrack *)getShowerHitTof(pCat);
}

// -----------------------------------------------------------------------------

HCategory* HPidTrackCand::buildPidTrackCategory(void)
{
// global function for making the category

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
        return pCat;

    if((pCat = new HLinearCategory(sCatName, 1000)) == NULL)
    {
        ::Error("HPidTrackCand::buildPidTrackCategory",
                    "Cannot create new category");

        return NULL;
    }

    pEvent->addCategory(catPidTrackCand, pCat, "Pid");

    return pCat;
}

