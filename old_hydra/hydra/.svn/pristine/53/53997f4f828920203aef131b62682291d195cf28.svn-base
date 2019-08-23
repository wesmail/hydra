// @(#)$Id: hpidtrackcand.cc,v 1.1 2002-10-30 10:55:33 halo Exp $
//*-- Author : Marcin Jaskula 28/06/2002
// Modified  : Marcin Jaskula 16/09/2002
//             - removed: HArrayS, OuterHits
//             - Rings array -> one Ring Id

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidTrackCand                                                              //
//                                                                            //
// In this container correlated hits from all detectors are stored.           //
// The class do not define the algorithm how the correlation is done.         //
// It should be defined in a correlator which fills the container.            //
//                                                                            //
// The structure of this class allow user to store information about          //
// the correlated hits as well as information about all hits in neighborhood  //
// to hits which construct the root of the track candidate. The hits in       //
// the neighborhood are dedicated for identification close pair and ring      //
// fakes as it was discussed at the PID subgroup meeting.                     //
//                                                                            //
// The structure of the container is similar to the HKickTrack category.      //
// There is information about the sector and META hit like in the HKickTrack. //
// The nKickTrackId keeps id of the kick track in the category. To get the    //
// HKickTrack from this information a call similar for getting HMdcSeg must   //
// be done:                                                                   //
// HLocation loc;                                                             //
// loc.set(3,0,0,0);                                                          //
// loc[0] = nSector;                                                          //
// loc[1] = 0;                                                                //
// loc[2] = nKickTrackId;                                                     //
// category->getObject(loc);                                                  //
//                                                                            //
// For Mdc's segs and rich rings arrays of ids are dedicated. The idea was    //
// that the first id in the arrays ([0]) is used for building the track       //
// candidate. The rest of the ids in the arrays are used only as information  //
// for the close pairs and similar tasks.                                     //
// Usage of HArrayS class lets to store any number of ids of one type.        //
// To get a number of hits in one array call: aXXX.getSize()                  //
// Because the HArrayS returns -1 for any element which was not set, so to    //
// check if there was a ring on can check (aRingIds[0] != -1) condition.      //
//                                                                            //
// Methods getXXX are dedicate to make access to hits/rings/tracks in         //
// an easy way. They work only in gHades loop. This has NOT been checked yet! //
// The idea is to let these functions to work not only in the Hades loop.     //
// It lets to use them in the T.Draw() as well as during some analysis        //
// made outside of the normal Hades loop.                                     //
//                                                                            //
// For every id of hit/ring/track a counter for this object is dedicated.     //
// They should store information in how many HPidTrackCand the hit/ring/track //
// was used and make this information available for cleaning the tracks       //
// candidates.                                                                //
// This is arbitrary if the track fillers (correlators) use these members.    //
//                                                                            //
// It is not decided what should be stored in OuterMdcIds fields but this     //
// member is put here for the future use.                                     //
//                                                                            //
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

Int_t HPidTrackCand::print(void) const
{
// Print all fields of the track candidate

UInt_t i;

    printf("Sector         : %4d\n", nSector);
    printf("KickTrackId    : %4d (%4d)\n", nKickTrackId, nKickTrackCount);
    printf("RingId         : %4d (%4d)\n", nRingId, nRingCount);

    printf("InnerMdcIds(%2d):", unInnerMdcNumber);
    for(i = 0; i < unInnerMdcNumber; i++)
        printf(" %4d (%4d)", aInnerMdcIds[i], aInnerMdcCounts[i]);

    printf("\nMeta system    : %4d\n     id        : %4d (%4d)\n",
            nSystem, nMetaId, nMetaCount);

    return 0;
}

// -----------------------------------------------------------------------------

HCategory* HPidTrackCand::getCategory(Cat_t cat) const
{
// Find HCategory if gHades->getCurrentEvent() is accessable

HEvent    *pEvent;
HCategory *pCat;

    if((gHades == NULL) || ((pEvent = gHades->getCurrentEvent()) == NULL))
    {
        Error("getCategory", "Cannot access current event");
        return NULL;
    }

    if((pCat = pEvent->getCategory(cat)) == NULL)
        Error("getCategory", "No category: %d", cat);

    return pCat;
}

// -----------------------------------------------------------------------------

HKickTrack* HPidTrackCand::getKickTrack(HCategory *pCat) const
{
// Returns HKickTrack object corresponding to nKickTrackId (if exists)
// pCat must point on the catKickTrack category or gHades->getCurrentEvent()
// must be accessable

    if(nKickTrackId < 0)
        return NULL;

    if((pCat == NULL) && ((pCat = getCategory(catKickTrack)) == NULL))
        return NULL;

    return (HKickTrack *)pCat->getObject(nKickTrackId);
}

// -----------------------------------------------------------------------------

HMdcSeg* HPidTrackCand::getMdcSeg(Int_t iPos, HCategory *pCat) const
{
// Returns HMdcSeg object corresponding to aInnerMdcIds[iPos] (if exists)
// pCat must point on the catMdcSeg category or gHades->getCurrentEvent()
// must be accessable

HLocation  loc;
Int_t      iId;

    if((iPos < 0) || (iPos >= unInnerMdcNumber)
            || ((iId = aInnerMdcIds[iPos]) < 0))
    {
        return NULL;
    }

    if((pCat == NULL) && ((pCat = getCategory(catMdcSeg)) == NULL))
        return NULL;

    loc.setNIndex(3);
    loc[0] = nSector;
    loc[1] = 0;
    loc[2] = iId;

    return (HMdcSeg *)pCat->getObject(loc);
}

// -----------------------------------------------------------------------------

HRichHit* HPidTrackCand::getRichHit(HCategory *pCat) const
{
// Returns HRichHit object corresponding to nRingId (if exists)
// pCat must point on the catRichHit category or gHades->getCurrentEvent()
// must be accessable

    if((pCat == NULL) && ((pCat = getCategory(catRichHit)) == NULL))
        return NULL;

    return (HRichHit *)pCat->getObject((Int_t)nRingId);
}

// -----------------------------------------------------------------------------

TObject* HPidTrackCand::getMetaHit(HCategory *pShwCat, HCategory *pTofCat) const
{
// Returns META hit object corresponding to META parameters
// in the container (if exists)
// If the system is SHOWER then pShwCat must point on the catShowerHitTof
// category or if system is TOF then pTofCat must point on the catTofHit.
// If the pointer is NULL then gHades->getCurrentEvent() must be accessable

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
// must be accessable

    if((nSystem != 1) || (nMetaId < 0))
        return NULL;

    if((pCat == NULL) && ((pCat = getCategory(catTofHit)) == NULL))
        return NULL;

    return (HTofHit *)pCat->getObject(nMetaId);
}

// -----------------------------------------------------------------------------

HShowerHitTof* HPidTrackCand::getShowerHitTof(HCategory *pCat) const
{
// Returns HShowerHitTof object corresponding to nMetaId (if exists)
// pCat must point on the catShowerHitTof category or gHades->getCurrentEvent()
// must be accessable

    if((nSystem != 0) || (nMetaId < 0))
        return NULL;

    if((pCat == NULL) && ((pCat = getCategory(catShowerHitTof)) == NULL))
        return NULL;

    return (HShowerHitTof *)pCat->getObject(nMetaId);
}

// -----------------------------------------------------------------------------

HCategory* buildPidTrackCategory(void)
{
// global function for making the category

HCategory  *pCat;
static char sCatName[] = "HPidTrackCand";
HEvent     *pEvent;

    if((gHades == NULL) || ((pEvent = gHades->getCurrentEvent()) == NULL))
    {
        ::Error("buildPidTrackCategory", "Cannot access current event");
        return NULL;
    }

    if((pCat = pEvent->getCategory(catPidTrackCand)) != NULL)
        return pCat;

    if((pCat = new HLinearCategory(sCatName, 1000)) == NULL)
    {
        ::Error("buildPidTrackCategory", "Cannot create new category");
        return NULL;
    }

    pEvent->addCategory(catPidTrackCand, pCat, "Pid");

    return pCat;
}

