// @(#)$Id: hpidsimtrigger.cc,v 1.2 2003-04-03 13:31:03 jaskula Exp $
//*-- Author : Marcin Jaskula 13/01/2003

////////////////////////////////////////////////////////////////////////////////
//
// HPidSimTrigger
//
////////////////////////////////////////////////////////////////////////////////

#include "hpidtrackcand.h"
#include "hcategory.h"
#include "hiterator.h"

#include "hpidsimtrigger.h"

#include "tofdef.h"
#include "showertofinodef.h"

#include "hshowerhittoftrack.h"

#include <TMath.h>

// -----------------------------------------------------------------------------

ClassImp(HPidSimTrigger)

// -----------------------------------------------------------------------------

HPidSimTrigger::HPidSimTrigger(Int_t i)
    : HReconstructor("PidSimTrigger", "Check TOF + TOFno mult.")
{
// Default constructor.

    setDefault(i);
}

// -----------------------------------------------------------------------------

HPidSimTrigger::HPidSimTrigger(Text_t name[], Text_t title[], Int_t i)
    : HReconstructor(name, title)
{
// Constructor with names

    setDefault(i);
}

// -----------------------------------------------------------------------------

HPidSimTrigger::~HPidSimTrigger(void)
{
    if(pItTofino != NULL)
    {
        delete pItTofino;
        pItTofino = NULL;
    }
}

// -----------------------------------------------------------------------------

void HPidSimTrigger::setDefault(Int_t k)
{
// Called in constructors to set default valuest to the members

    iMinMult   = k;
    pTofCat    = NULL;
    pTofinoCat = NULL;
    pItTofino  = NULL;

    uiEvents   = 0;
    uiEventsOk = 0;
}

// -----------------------------------------------------------------------------

Bool_t HPidSimTrigger::init(void)
{
// Initialize the reconstructor

    uiEvents   = 0;
    uiEventsOk = 0;

    if((pTofCat = HPidTrackCand::getCategory(catTofHit)) == NULL)
    {
        Error("reinit", "Cannot get catTofHit");
        return kFALSE;
    }

    if((pTofinoCat = HPidTrackCand::getCategory(catShowerHitTofTrack)) == NULL)
    {
        Error("reinit", "Cannot get catShowerHitTofTrack");
        return kFALSE;
    }

    if((pItTofino = (HIterator *) pTofinoCat->MakeIterator()) == NULL)
    {
        Error("init",
                "Cannot make an iterator for HShowerHitTofTrack category");

        return kFALSE;
    }

    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidSimTrigger::reinit(void)
{
// Empty method

    if(pItTofino == NULL)
    {
        Error("reinit", "No iterator for HShowerHitTofTrack category");
        return kFALSE;
    }

    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidSimTrigger::finalize(void)
{
// Empty method

    print();

    return kTRUE;
}

// -----------------------------------------------------------------------------

Int_t HPidSimTrigger::execute(void)
{
// Check whether the TOF+TOFino mult. >= iMinMult

    uiEvents++;

Int_t iMult = getMult();

    if((iMult = getMult()) < 0)
        return 1;

    if(iMult >= iMinMult)
    {
        uiEventsOk++;

        return 0;
    }

    return kSkipEvent;
}

// -----------------------------------------------------------------------------

Int_t HPidSimTrigger::getMult(void)
{
// Check whether the TOF+TOFino mult. >= iMinMult

    if(pItTofino == NULL)
    {
        Error("getMult", "No iterator for HShowerHitTofTrack category");
        return -1;
    }

Int_t               iTofinoMult = 0;
static Bool_t       aTofino[6][3];
HShowerHitTofTrack *pST;
Int_t               iSec, iMod;

    memset(aTofino, 0, sizeof(aTofino));

    pItTofino->Reset();
    while((pST = (HShowerHitTofTrack *)pItTofino->Next()) != NULL)
    {
        iSec = pST->getSector();
        iMod = pST->getTofinoCell();

        if( ! aTofino[iSec][iMod])
        {
            aTofino[iSec][iMod] = kTRUE;
            iTofinoMult++;
        }
    }

    return pTofCat->getEntries() + iTofinoMult;
}

// -----------------------------------------------------------------------------

Double_t HPidSimTrigger::getRatio(void) const
{
// Returns ratio uiEventsOk/uiEvents

    return (uiEvents > 0) ? (Double_t(uiEventsOk) / uiEvents) : 0.0;
}

// -----------------------------------------------------------------------------

Double_t HPidSimTrigger::getRatioError(void) const
{
// Statistic error of the ratio uiEventsOk/uiEvents

Double_t dR;

    dR = (uiEventsOk > 0)
                ? (TMath::Sqrt(Double_t(uiEventsOk)) / uiEventsOk) : 0.0;
    dR += (uiEvents > 0)
                ? (TMath::Sqrt(Double_t(uiEvents)) / uiEvents) : 0.0;

    return (dR * getRatio());
}

// -----------------------------------------------------------------------------

void HPidSimTrigger::print(void) const
{
    printf("HPidSimTrigger\n");
    printf("Min mult. : %d\n", iMinMult);
    printf("All events: %d\n", uiEvents);
    printf("Events OK : %d\n", uiEventsOk);
    printf("Ratio     : %8.4f%% +/- %8.4f%%\n", 100. * getRatio(),
                        100. * getRatioError());
}
