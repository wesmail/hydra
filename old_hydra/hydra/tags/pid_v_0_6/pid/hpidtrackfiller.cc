// @(#)$Id: hpidtrackfiller.cc,v 1.3 2002-11-20 17:20:50 jaskula Exp $
//*-- Author  : Marcin Jaskula 29/06/2002
//*-- Modified: Marcin Jaskula 09/10/2002
//              - HPidTrackFillerPar used
//*-- Modified: Marcin Jaskula 31/10/2002
//              - getMdc* moved from global to static function
//*-- Modified: Marcin Jaskula 10/11/2002
//              - input categories must be created before init() !!!

////////////////////////////////////////////////////////////////////////////////
//
// HPidTrackFiller
//
// A simple track filler of HPidTrackCand category.
// The data from HKickTrack are used and matching with RICH is done.
// For the matched ring correlated HMdcSegs are found for the close pair
// rejection. For all not HMdcSegs not matched in any HKickTrack the searching
// for corresponding rings is done as well.
//
// The options set in the constructor may modify this behaviour: see
// setParameters() description.
//
////////////////////////////////////////////////////////////////////////////////

#include "hpidtrackfiller.h"

#include "hades.h"
#include "hevent.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hdetector.h"
#include "hspectrometer.h"

#include "hkicktrack.h"
#include "hrichhit.h"
#include "hmdcseg.h"
#include "hpidtrackcand.h"

#include "kickdef.h"
#include "richdef.h"
#include "hmdcdef.h"

#include "hruntimedb.h"
#include "hpidtrackfillerpar.h"

#include "showertofinodef.h"
#include "hshowerhittof.h"

// -----------------------------------------------------------------------------

#define DEF_ARRAY_SIZE 10000
#define SAFE_DELETE(A) { if(A) { delete (A); A = NULL; }}

// -----------------------------------------------------------------------------

ClassImp(HPidTrackFiller)

// -----------------------------------------------------------------------------

HPidTrackFiller::HPidTrackFiller(Option_t par[])
    : HReconstructor("PidTrackFiller", "PID track candidate filler")
{
// Default constructor.
// For options see setParameters() description.

    setDefault();
    setParameters(par);
}

// -----------------------------------------------------------------------------

HPidTrackFiller::HPidTrackFiller(Text_t name[], Text_t title[], Option_t par[])
    : HReconstructor(name, title)
{
// Default constructor.
// For options see setParameters() description.

    setDefault();
    setParameters(par);
}

// -----------------------------------------------------------------------------

HPidTrackFiller::~HPidTrackFiller(void)
{
// What's this ???

    SAFE_DELETE(pitTrack);
    SAFE_DELETE(pitMdc);
    SAFE_DELETE(pitMdcN);
    SAFE_DELETE(pitRich);
    SAFE_DELETE(pitCand);
}

// -----------------------------------------------------------------------------

void HPidTrackFiller::setParameters(Option_t par[])
{
// Set parameters by names. Options (may be separated by comma or blank chars):
// NOMDC  - turn off matching of MdcSegs not matched with any KP
// NORICH - turn off RICH matching with KP (MdcSegs is turn of as well)

TString s = par;

    s.ToUpper();

    bNoMdc  = (strstr(s.Data(), "NOMDC") != NULL);
    bNoRich = (strstr(s.Data(), "NORICH") != NULL);
}

// -----------------------------------------------------------------------------

Bool_t HPidTrackFiller::reinit(void)
{
    if(bInitOk == kFALSE)
        Warning("reinit", "init() didn't succeed");
    else
        bInitOk = kTRUE;

    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidTrackFiller::init(void)
{
// get/build all used categories and iterators

HEvent *pEvent;

    bInitOk = kFALSE;

    if(gHades == NULL)
    {
        Error("init", "gHades == NULL");
        return kFALSE;
    }

    if((pEvent = gHades->getCurrentEvent()) == NULL)
    {
        Error("init", "gHades->getCurrentEvent() == NULL");
        return kFALSE;
    }

    // delete iterators (if exist)
    SAFE_DELETE(pitTrack);
    SAFE_DELETE(pitMdc);
    SAFE_DELETE(pitMdcN);
    SAFE_DELETE(pitRich);
    SAFE_DELETE(pitCand);

    // get HPidTrackFillerPar container
    if((pParams = (HPidTrackFillerPar *)gHades->getRuntimeDb()
                    ->getContainer(HPIDTRACKFILLERPAR_NAME)) == NULL)
    {
        Error("init", "Cannot get parameters: %s", HPIDTRACKFILLERPAR_NAME);
        return kFALSE;
    }

    // input categories
    if(getCategories(pEvent) == kFALSE)
        return kFALSE;

    if(bNoMdc)
    {
        pCatMdc = NULL;
        pitMdc  = NULL;
        pitMdcN = NULL;
    }
    else
    {
        // get/build catMdcSeg
        if((pCatMdc = pEvent->getCategory(catMdcSeg)) == NULL)
        {
            Error("init", "Cannot get catMdcSeg category");
            return kFALSE;
        }

        // get Mdc iterators
        if(((pitMdc  = (HIterator *)pCatMdc->MakeIterator()) == NULL)
                || ((pitMdcN = (HIterator *)pCatMdc->MakeIterator()) == NULL))
        {
            Error("init", "Cannot make catMdcSeg category iterators");
            return kTRUE;
        }
    }

    // get Rich iterator
    if((pCatRich != NULL)
            && (pitRich  = (HIterator *)pCatRich->MakeIterator()) == NULL)
    {
        Error("init", "Cannot make rich iterator");

        return kFALSE;
    }

    // get catKickTrack
    if((pCatTrack = pEvent->getCategory(catKickTrack)) == NULL)
    {
        Error("init", "No catKickTrack");
        return kFALSE;
    }

    if((pitTrack = (HIterator *)pCatTrack->MakeIterator()) == NULL)
    {
        Error("init", "Cannot make catKickTrack category iterator");
        return kFALSE;
    }

    // get output category
    if((pCatCand = HPidTrackCand::buildPidTrackCategory()) == NULL)
    {
        Error("init", "Cannot build PidTrack Category");
        return kFALSE;
    }

    if((pitCand = (HIterator *)pCatCand->MakeIterator()) == NULL)
    {
        Error("init", "Cannot make an iterator for PidTrack Category");
        return kFALSE;
    }

    bInitOk = kTRUE;

    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidTrackFiller::getCategories(HEvent *pEvent)
{
// Internal function for getting proper input cats.

    // get/build catRichHit
    if(bNoRich)
        pCatRich = NULL;
    else
    {
        if((pCatRich = pEvent->getCategory(catRichHit)) == NULL)
        {
            Error("init", "Cannot get catRichHit category");

            return kFALSE;
        }
    }

    return kTRUE;
}

// -----------------------------------------------------------------------------

Int_t HPidTrackFiller::execute(void)
{
// find all track candidates and fill the output category

Int_t       iSector;
Int_t       iR, iRN, i;
HKickTrack *pTrack;
HRichHit   *pRing;
HMdcSeg    *pMdcSeg;
Float_t     fKickPhi;
Float_t     fKickTheta;
Float_t     fMdcPhi;
Float_t     fMdcTheta;
Float_t     fSinTheta;

HPidTrackCand *pCand;

    if(bInitOk == kFALSE)
    {
        Error("execute", "Class not initialized");
        return -1;
    }

    // clear the counters
    memset(aTrackCounts, 0, sizeof(aTrackCounts));
    memset(aMetaCounts, 0, sizeof(aMetaCounts));
    memset(aMdcCounts, 0, sizeof(aMdcCounts));
    memset(aRichCounts, 0, sizeof(aRichCounts));

    // loop over sectors
    for(iSector = 0; iSector < MAX_SECTOR; iSector++)
    {
        lTrack[0] = iSector;
        lTrack[1] = 0;

        pitTrack->Reset();
        pitTrack->gotoLocation(lTrack);

        // loop over tracks
        while((pTrack = (HKickTrack *)pitTrack->Next()) != NULL)
        {
            if((pParams->getTofinoMult() != 0) && ( ! checkTofinoMult(pTrack)))
                continue;

            pCand      = NULL;
            fKickPhi   = getMdcPhi(iSector, pTrack->getPhi());
            fKickTheta = getMdcTheta(pTrack->getTheta());

            fSinTheta  = TMath::Sin(pTrack->getTheta());

            // loop over rings (if exist)
            if(pitRich != NULL)
            {
                pitRich->Reset();
                iR = -1;
                while((pRing = (HRichHit *) pitRich->Next()) != NULL)
                {
                    iR++;

                    if((pRing->getSector() != iSector)
                            || (fabs(pRing->getTheta() - fKickTheta)
                                    > pParams->getWindowKickRichTheta())
                            || (fabs(pRing->getPhi() - fKickPhi) * fSinTheta
                                    > pParams->getWindowKickRichPhi()))
                    {
                        continue;
                    }

                    if((pCand = getNextSlot(pTrack)) == NULL)
                        return -2;

                    pCand->setRingId(iR);
                    aRichCounts[iR]++;

                    // loop over MDC segs
                    if(pitMdc == NULL)
                        continue;

                    pitMdc->Reset();
                    pitMdc->gotoLocation(lTrack);

                    i = -1;
                    while((pMdcSeg = (HMdcSeg *) pitMdc->Next()) != NULL)
                    {
                        i++;
                        if(i == pCand->getInnerMdcId(0))
                            continue;

                        fMdcPhi   = getMdcPhi(iSector, pMdcSeg->getPhi());
                        fMdcTheta = getMdcTheta(pMdcSeg->getTheta());

                        if((fabs(pRing->getTheta() - fMdcTheta)
                                    > pParams->getWindowMdcRichTheta())
                            || (fabs(pRing->getPhi() - fMdcPhi) * fSinTheta
                                    > pParams->getWindowMdcRichPhi()))
                        {
                            continue;
                        }

                        pCand->addInnerMdcId(i);
                        aMdcCounts[iSector][i]++;
                    }
                }
            }

            // if not candidate has been created - no correlation with RICH
            if(pCand == NULL)
                pCand = getNextSlot(pTrack);
        }
    }

    // loop over rings not correlated with KickTrack
    if((pitRich != NULL) && (pitMdc != NULL) && (pitMdcN != NULL))
    {
        pitRich->Reset();
        iR = -1;
        while((pRing = (HRichHit *) pitRich->Next()) != NULL)
        {
            iR++;

            // ring has been already used in the root of the correlation with KT
            if(aRichCounts[iR] > 0)
                continue;

            iSector = pRing->getSector();
            lTrack[0] = iSector;
            fSinTheta = TMath::Sin(TMath::Pi() * pRing->getTheta() / 180.0);

            // loop over MDC segs
            pitMdc->Reset();
            pitMdc->gotoLocation(lTrack);
            i = -1;
            while((pMdcSeg = (HMdcSeg *) pitMdc->Next()) != NULL)
            {
                i++;
                fMdcPhi   = getMdcPhi(iSector, pMdcSeg->getPhi());
                fMdcTheta = getMdcTheta(pMdcSeg->getTheta());

                if((fabs(pRing->getTheta() - fMdcTheta)
                            > pParams->getWindowMdcRichTheta())
                    || (fabs(pRing->getPhi() - fMdcPhi) * fSinTheta
                            > pParams->getWindowMdcRichPhi()))
                {
                    continue;
                }

                // create candidate
                if((pCand = getNextSlot()) == NULL)
                    return -2;
                pCand->setSector(iSector);

                pCand->setRingId(iR);
                aRichCounts[iR]++;

                pCand->addInnerMdcId(i);
                aMdcCounts[iSector][i]++;

                // loop over mdc segs around
                pitMdcN->Reset();
                pitMdcN->gotoLocation(lTrack);

                iRN = -1;
                while((pMdcSeg = (HMdcSeg *) pitMdc->Next()) != NULL)
                {
                    iRN++;
                    fMdcPhi   = getMdcPhi(iSector, pMdcSeg->getPhi());
                    fMdcTheta = getMdcTheta(pMdcSeg->getTheta());

                    if((iRN == i)
                            || (fabs(pRing->getTheta() - fMdcTheta)
                                    > pParams->getWindowMdcRichTheta())
                            || (fabs(pRing->getPhi() - fMdcPhi) * fSinTheta
                                    > pParams->getWindowMdcRichPhi()))
                    {
                        continue;
                    }

                    pCand->addInnerMdcId(iRN);
                    aMdcCounts[iSector][iRN]++;
                }
            }
        }
    }

    calculateCounts();

    return 0;
}

// -----------------------------------------------------------------------------

Bool_t HPidTrackFiller::finalize(void)
{
// empty method

    return kTRUE;
}

// -----------------------------------------------------------------------------

void HPidTrackFiller::setDefault(void)
{
// set default values inside the construtors

    pCatTrack = NULL;
    pCatMdc   = NULL;
    pCatRich  = NULL;
    pCatCand  = NULL;

    lTrack.set(2, 0, 0);

    pitTrack  = NULL;
    pitMdc    = NULL;
    pitMdcN   = NULL;
    pitRich   = NULL;
    pitCand   = NULL;

    pParams   = NULL;

    bNoMdc    = kFALSE;
    bNoRich   = kFALSE;

    bInitOk   = kFALSE;

    cCatShowerTof = catShowerHitTof;
}

// -----------------------------------------------------------------------------

HPidTrackCand* HPidTrackFiller::getNextSlot(void)
{
// Get next slot from the output category

HPidTrackCand    *pOut = NULL;
static HLocation  locDummy;

    if(pCatCand == NULL)
    {
        Error("getNextSlot", "Output category not set: use init/reinit");
        return NULL;
    }

    if((pOut = (HPidTrackCand *) pCatCand->getNewSlot(locDummy)) == NULL)
    {
        Error("getNextSlot", "No new slot");
        return NULL;
    }

    return new(pOut) HPidTrackCand;
}

// -----------------------------------------------------------------------------

HPidTrackCand* HPidTrackFiller::getNextSlot(HKickTrack *pTrack)
{
// get next slot from the output category and fills it with data from HKickTrack

HPidTrackCand *pOut;
Short_t        nTrackId;

    if((pOut = getNextSlot()) == NULL)
        return NULL;

    pOut->setSector(pTrack->getSector());

    nTrackId = pCatTrack->getIndex(pTrack);
    pOut->setKickTrackId(nTrackId);
    aTrackCounts[pTrack->getSector()][nTrackId]++;

    pOut->setMetaId(pTrack->getSystem(), pTrack->getOuterHitId());
    aMetaCounts[pTrack->getSystem()][pTrack->getOuterHitId()]++;

    pOut->addInnerMdcId(pTrack->getSegment());
    aMdcCounts[pTrack->getSector()][pTrack->getSegment()]++;

    return pOut;
}

// -----------------------------------------------------------------------------

void HPidTrackFiller::calculateCounts(void)
{
// fill all candidates counts using the aXXXCounts filled during
// execute loops

HPidTrackCand *pCand;
Int_t          i, iMax;
Short_t        n;

    pitCand->Reset();
    while((pCand = (HPidTrackCand *) pitCand->Next()) != NULL)
    {
        if(pCand->getKickTrackId() >= 0)
        {
            if((n = aTrackCounts[pCand->getSector()][pCand->getKickTrackId()])
                    <= 0)
            {
                Error("calculateCounts", "Track count <= 0");
            }

            pCand->setKickTrackCount(n);

            if((n = aMetaCounts[pCand->getSystem()][pCand->getMetaId()]) <= 0)
                Error("calculateCounts", "Meta count <= 0");

            pCand->setMetaCount(n);
        }

        iMax = pCand->getInnerMdcNumber();
        for(i = 0; i < iMax; i++)
        {
            if((n = aMdcCounts[pCand->getSector()][pCand->getInnerMdcId(i)])
                    <= 0)
            {
                Error("calculateCounts", "Mdc Counts <= 0");
            }

            pCand->setInnerMdcCount(i, n);
        }

        if(pCand->getRingId() >= 0)
        {
            if((n = aRichCounts[pCand->getRingId()]) <= 0)
                Error("calculateCounts", "Rich count <= 0");

            pCand->setRingCount(n);
        }
    }
}

// -----------------------------------------------------------------------------

Bool_t HPidTrackFiller::checkTofinoMult(HKickTrack *pTrack)
{
// Check the condition for the tofino multiplicity > 1
// This version returns true only for mult==1

HCategory     *pCat;
HShowerHitTof *pShwTof;

    if(pTrack->getSystem() != 0)
        return kTRUE;

    if(pTrack->getOuterHitId() < 0)
        return kFALSE;

    if((pCat = gHades->getCurrentEvent()->getCategory(cCatShowerTof)) == NULL)
    {
        Error("checkTofinoMult", "Cannot get catShowerHitTof");
        return kFALSE;
    }

    if((pShwTof = (HShowerHitTof *) pCat->getObject(pTrack->getOuterHitId()))
                == NULL)
    {
        Error("checkTofinoMult", "Cannot get HShowerHitTof %d" ,
                    pTrack->getOuterHitId());

        return kFALSE;

    }

    if(pShwTof->getTofinoMult() <= 1)
        return kTRUE;

    // FIXME: Jacek's algorithm should work here

    return kFALSE;
}
