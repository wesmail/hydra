// @(#)$Id: hpidtrackcand.cc,v 1.6 2003-03-04 17:21:47 jaskula Exp $
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

#include "hpidgeanttrackset.h"

#include "hkicktracksim.h"
#include "hrichhitsim.h"

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
// Find HCategory if gHades->getCurrentEvent() is accessible

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

HKickTrack* HPidTrackCand::getKickTrack(Int_t iId, HCategory *pCat)
{
// Static function which returns HKickTrack object corresponding to iId.
// pCat must point on the catKickTrack category or gHades->getCurrentEvent()
// must be accessible.
// In case catKickTrack is category of HKickTrackSim then the object of this
// class is returned.

    if(iId < 0)
        return NULL;

    if((pCat == NULL) && ((pCat = getCategory(catKickTrack)) == NULL))
    {
        ::Error("HPidTrackCand::getKickTrack", "No catKickTrack category");
        return NULL;
    }

    return (HKickTrack *)pCat->getObject(iId);
}
// -----------------------------------------------------------------------------

HKickTrack* HPidTrackCand::getKickTrack(HCategory *pCat) const
{
// Returns HKickTrack object corresponding to nKickTrackId (if exists)
// pCat must point on the catKickTrack category or gHades->getCurrentEvent()
// must be accessible.
// In case catKickTrack is category of HKickTrackSim then the object of this
// class is returned.

    if(nKickTrackId < 0)
        return NULL;

    return getKickTrack(nKickTrackId, pCat);
}

// -----------------------------------------------------------------------------

HKickTrackSim* HPidTrackCand::getKickTrackSim(Int_t iId, HCategory *pCat)
{
// Static function which returns HKickTrackSim object corresponding to iId.
// Returns HKickTrackSim object corresponding to nKickTrackId (if exists)
// pCat must point on the catKickTrack category or gHades->getCurrentEvent()
// must be accessible.

    return (HKickTrackSim *)getKickTrack(iId, pCat);
}
// -----------------------------------------------------------------------------

HKickTrackSim* HPidTrackCand::getKickTrackSim(HCategory *pCat) const
{
// Returns HKickTrackSim object corresponding to nKickTrackId (if exists)
// pCat must point on the catKickTrack category or gHades->getCurrentEvent()
// must be accessible.
// In case the HPidTrackCand is not Sim then an error is reported and
// the method returns NULL.

    if( ! isSim())
    {
        Error("getKickTrackSim", "HPidTrackCand is not sim");
        return NULL;
    }

    if(nKickTrackId < 0)
        return NULL;

    return getKickTrackSim(nKickTrackId, pCat);
}

// -----------------------------------------------------------------------------

HMdcSeg* HPidTrackCand::getMdcSeg(Int_t iSector, Int_t iId, HCategory *pCat)
{
// Static function which returns HMdcSeg object corresponding to iId
// in sector iSector.
// pCat must point on the catMdcSeg category or gHades->getCurrentEvent()
// must be accessible.

HLocation  loc;

    if((iSector < 0) || (iSector > 5) || (iId < 0))
    {
        return NULL;
    }

    if((pCat == NULL) && ((pCat = getCategory(catMdcSeg)) == NULL))
    {
        ::Error("HPidTrackCand::getMdcSeg", "No catMdcSeg category");
        return NULL;
    }

    loc.setNIndex(3);
    loc[0] = iSector;
    loc[1] = 0;
    loc[2] = iId;

    return (HMdcSeg *)pCat->getObject(loc);
}
// -----------------------------------------------------------------------------

HMdcSeg* HPidTrackCand::getMdcSeg(Int_t iPos, HCategory *pCat) const
{
// Returns HMdcSeg object corresponding to aInnerMdcIds[iPos] (if exists)
// pCat must point on the catMdcSeg category or gHades->getCurrentEvent()
// must be accessible.
// In case HPidTrackCand isSim() then HMdcSegSim is returned.

Int_t      iId;

    if((iPos < 0) || (iPos >= unInnerMdcNumber)
            || ((iId = aInnerMdcIds[iPos]) < 0))
    {
        return NULL;
    }

    return getMdcSeg(nSector, iId, pCat);
}

// -----------------------------------------------------------------------------

HMdcSegSim* HPidTrackCand::getMdcSegSim(Int_t iSector, Int_t iId,
                HCategory *pCat)
{
// Static function which returns HMdcSegSim object corresponding to iId
// in sector iSector.
// pCat must point on the catMdcSeg category or gHades->getCurrentEvent()
// must be accessible.

    return (HMdcSegSim *) getMdcSeg(iSector, iId, pCat);
}

// -----------------------------------------------------------------------------

HMdcSegSim* HPidTrackCand::getMdcSegSim(Int_t iPos, HCategory *pCat) const
{
// Returns HMdcSegSim object corresponding to aInnerMdcIds[iPos] (if exists)
// pCat must point on the catMdcSeg category or gHades->getCurrentEvent()
// must be accessible.
// In case the HPidTrackCand is not Sim then an error is reported and
// the method returns NULL

    if( ! isSim())
    {
        Error("getMdcSegSim", "HPidTrackCand is not sim");
        return NULL;
    }

Int_t      iId;

    if((iPos < 0) || (iPos >= unInnerMdcNumber)
            || ((iId = aInnerMdcIds[iPos]) < 0))
    {
        return NULL;
    }

    return getMdcSegSim(nSector, iId, pCat);
}

// -----------------------------------------------------------------------------

HRichHit* HPidTrackCand::getRichHit(Int_t iId, HCategory *pCat)
{
// Static function which returns HRichHit object corresponding to iId index.
// pCat must point on the catRichHit category or gHades->getCurrentEvent()
// must be accessible.

    if(iId < 0)
        return NULL;

    if((pCat == NULL) && ((pCat = getCategory(catRichHit)) == NULL))
    {
        ::Error("HPidTrackCand::getRichHit", "No catRichHit category");
        return NULL;
    }

    return (HRichHit *)pCat->getObject(iId);
}

// -----------------------------------------------------------------------------

HRichHit* HPidTrackCand::getRichHit(HCategory *pCat) const
{
// Returns HRichHit object corresponding to nRingId (if exists)
// pCat must point on the catRichHit category or gHades->getCurrentEvent()
// must be accessible.

    return getRichHit(nRingId, pCat);
}

// -----------------------------------------------------------------------------

HRichHitSim* HPidTrackCand::getRichHitSim(Int_t iId, HCategory *pCat)
{
// Static function which returns HRichHitSim object corresponding to iId index.
// pCat must point on the catRichHitSim category or gHades->getCurrentEvent()
// must be accessible.
// In the second case catRichHit is used instead of catRichHitSim because
// some inaccurate in RICH code.

    if(iId < 0)
        return NULL;

    if((pCat == NULL) && ((pCat = getCategory(catRichHit)) == NULL))
    {
        ::Error("HPidTrackCand::getRichHit", "No catRichHit category");
        return NULL;
    }

    return (HRichHitSim *)pCat->getObject(iId);
}
// -----------------------------------------------------------------------------

HRichHitSim* HPidTrackCand::getRichHitSim(HCategory *pCat) const
{
// Returns HRichHitSim object corresponding to nRingId (if exists)
// pCat must point on the catRichHitSim category or gHades->getCurrentEvent()
// must be accessible.
// In case the HPidTrackCand is not Sim then an error is reported and
// the method returns NULL

    if( ! isSim())
    {
        Error("getRichHitSim", "HPidTrackCand is not sim");
        return NULL;
    }

    return getRichHitSim(nRingId, pCat);
}

// -----------------------------------------------------------------------------

TObject* HPidTrackCand::getMetaHit(HCategory *pShwCat, HCategory *pTofCat) const
{
// Returns META hit object corresponding to META parameters
// in the container (if exists)
// If the system is SHOWER then pShwCat must point on the catShowerHitTof
// category or if system is TOF then pTofCat must point on the catTofHit.
// If the pointer is NULL then gHades->getCurrentEvent() must be accessible.
// In a case HPidTrackCand isSim() then proper sim obects are returned.

    if(nMetaId < 0)
        return NULL;

    switch(nSystem)
    {
        case 0:
            return (isSim()) ? getShowerHitTofTrack(pShwCat)
                             : getShowerHitTof(pShwCat);

        case 1:
            return (isSim()) ? getTofHitSim(pTofCat)
                             : getTofHit(pTofCat);

        case -1:
            break;

        default:
            Error("getMetaHit", "Unknown system: %d", nSystem);
            break;
    }

    return NULL;
}

// -----------------------------------------------------------------------------

HTofHit* HPidTrackCand::getTofHit(Int_t iId, HCategory *pCat)
{
// Static function which returns HTofHit object corresponding to iId.
// pCat must point on the catTofHit category or gHades->getCurrentEvent()
// must be accessible.
// In case HPidTrackCand isSim() then HTofHitSim is returned.

    if(iId < 0)
        return NULL;

    if((pCat == NULL) && ((pCat = getCategory(catTofHit)) == NULL))
    {
        ::Error("HPidTrackCand::getTofHit", "No catTofHit category");
        return NULL;
    }

HLocation loc;

    loc.setNIndex(3);
    loc[0] = iId / 1000;
    loc[1] = (iId % 1000) / 100;
    loc[2] = iId % 100;

    return (HTofHit *)pCat->getObject(loc);
}

// -----------------------------------------------------------------------------

HTofHit* HPidTrackCand::getTofHit(HCategory *pCat) const
{
// Returns HTofHit object corresponding to nMetaId (if exists)
// pCat must point on the catTofHit category or gHades->getCurrentEvent()
// must be accessible.
// In case HPidTrackCand isSim() then HTofHitSim is returned.

    if((nSystem != 1) || (nMetaId < 0))
        return NULL;

    return getTofHit(nMetaId);
}

// -----------------------------------------------------------------------------

HTofHitSim* HPidTrackCand::getTofHitSim(Int_t iId, HCategory *pCat)
{
// Static function which returns HTofHitTrack object corresponding to iId index.
// pCat must point on the catTofHit category or gHades->getCurrentEvent()
// must be accessible.

    return (HTofHitSim *)getTofHit(iId, pCat);
}

// -----------------------------------------------------------------------------

HTofHitSim* HPidTrackCand::getTofHitSim(HCategory *pCat) const
{
// Returns HTofHitTrack object corresponding to nMetaId (if exists)
// pCat must point on the catTofHit category or gHades->getCurrentEvent()
// must be accessible.
// In case the HPidTrackCand is not Sim then an error is reported and
// the method returns NULL.

    if( ! isSim())
    {
        Error("getTofHitSim", "HPidTrackCand is not sim");
        return NULL;
    }

    return (HTofHitSim *)getTofHit(pCat);
}

// -----------------------------------------------------------------------------

HShowerHitTof* HPidTrackCand::getShowerHitTof(Int_t iId, HCategory *pCat)
{
// Static function which returns HShowerHitTof object corresponding to iId.
// pCat must point on the catShowerHitTof category or gHades->getCurrentEvent()
// must be accessible.

    if(iId < 0)
        return NULL;

    if((pCat == NULL) && ((pCat = getCategory(catShowerHitTof)) == NULL))
    {
        ::Error("HPidTrackCand::getShowerHitTof",
                "No catShowerHitTof category");
        return NULL;
    }

    return (HShowerHitTof *)pCat->getObject(iId);
}

// -----------------------------------------------------------------------------

HShowerHitTof* HPidTrackCand::getShowerHitTof(HCategory *pCat) const
{
// Returns HShowerHitTof object corresponding to nMetaId (if exists)
// pCat must point on the catShowerHitTof category or gHades->getCurrentEvent()
// must be accessible.
// In case HPidTrackCand isSim() then HShowerHitTofTrack is returned
// from catShowerHitTrack.

    if((nSystem != 0) || (nMetaId < 0))
        return NULL;

    return getShowerHitTof(nMetaId, pCat);
}

// -----------------------------------------------------------------------------

HShowerHitTofTrack* HPidTrackCand::getShowerHitTofTrack(Int_t iId,
                    HCategory *pCat)
{
// Static function which returns HShowerHitTofTrack object corresponding to iId.
// pCat must point on the catShowerHitTof category or gHades->getCurrentEvent()
// must be accessible.

    if(iId < 0)
        return NULL;

    if((pCat == NULL) && ((pCat = getCategory(catShowerHitTofTrack)) == NULL))
    {
        ::Error("HPidTrackCand::getShowerHitTofTrack",
                "No catShowerHitTofTrack category");
        return NULL;
    }

    return (HShowerHitTofTrack *)pCat->getObject(iId);
}

// -----------------------------------------------------------------------------

HShowerHitTofTrack* HPidTrackCand::getShowerHitTofTrack(HCategory *pCat) const
{
// Returns HShowerHitTofTrack object corresponding to nMetaId (if exists)
// pCat must point on the catShowerHitTrack category or
// gHades->getCurrentEvent() must be accessible.
// In case the HPidTrackCand is not Sim then an error is reported and
// the method returns NULL.

    if( ! isSim())
    {
        Error("getShowerHitTofTrack", "HPidTrackCand is not sim");
        return NULL;
    }

    if((nSystem != 0) || (nMetaId < 0))
        return NULL;

    return getShowerHitTofTrack(nMetaId);
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

// -----------------------------------------------------------------------------

HPidTrackCand* HPidTrackCand::getTrackCand(Int_t iIdx, HCategory *pCat)
{
// Static function which returns HPidTrackCand object corresponding to
// iIdx from pCat.
// If the pCat is not set (NULL) catPidTrackCand category from gHades is used.

    if(iIdx < 0)
        return NULL;

    if((pCat == NULL) && ((pCat = getCategory(
                                    catPidTrackCand, kFALSE)) == NULL))
    {
        ::Error("HPidTrackCand::getTrackCand", "No catPidTrackCand category");

        return NULL;
    }

    return (HPidTrackCand *) pCat->getObject(iIdx);
}

// -----------------------------------------------------------------------------

const HPidGeantTrackSet* HPidTrackCand::getGeantTrackSet(void) const
{
// Return HPidGeantTrackSet from tracks stored in the corresponding
// HKickTrackSim and HRichHitSim (if exist and are accessible).
// If the HPidTrackCand is NOT sim then error is printed and NULL is returned.
//
// The returned pointer points to ONE COMMON STATIC HPidGeantTrackSet for
// all HPidTrackCand, so one needs to store the output they needs to copy it.
//

    if( ! isSim())
    {
        Error("getGeantTrackSet", "Is not a sim object");
        return NULL;
    }

static HPidGeantTrackSet set;

    set.reset();

HKickTrackSim* pKT = getKickTrackSim();
Int_t          i;

    if((pKT = getKickTrackSim()) == NULL)
    {
        Warning("getGeantTrackSet", "no kick track: tracks should be taken"
                    " from HMdcSegs, but it is not implemented");

        for(i = 0; i < HPidGeantTrackSet::kMaxMdcTracks; i++)
            set.setMdcTrack(i, HPidGeantTrackSet::kNoRefObj);

        for(i = 0; i < HPidGeantTrackSet::kMaxMetaTracks; i++)
            set.setMetaTrack(i, HPidGeantTrackSet::kNoRefObj);
    }
    else
    {
        for(i = 0; i < pKT->getNMdcTracks(); i++)
            set.setMdcTrack(i, (Short_t) pKT->getMdcTrack(i));

        for(i = 0; i < pKT->getNMetaTracks(); i++)
            set.setMetaTrack(i, (Short_t) pKT->getMetaTrack(i));
    }

HRichHitSim* pSimRich;

    if((pSimRich = getRichHitSim()) == NULL)
    {
        for(i = 0; i < HPidGeantTrackSet::kMaxRichTracks; i++)
            set.setRichTrack(i, HPidGeantTrackSet::kNoRefObj);
    }
    else
    {
        i = 0;

        if((pSimRich->weigTrack1 > 0) && (pSimRich->track1 > 0))
            set.setRichTrack(i++, pSimRich->track1);

        if((pSimRich->weigTrack2 > 0) && (pSimRich->track2 > 0))
            set.setRichTrack(i++, pSimRich->track2);

        if((pSimRich->weigTrack3 > 0) && (pSimRich->track3 > 0))
            set.setRichTrack(i++, pSimRich->track3);
    }

    return &set;
}
