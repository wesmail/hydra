// @(#)$Id: hpidtrackfillersim.cc,v 1.3 2004-05-25 11:52:50 wojcik Exp $
//*-- Author  : Marcin Jaskula 11/11/2002

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// HPidTrackFillerSim                                                   //
//                                                                      //
// A simple track filler of HPidTrackCand category for sim data         //
// For more info see HPidTrackFiller                                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "hpidtrackfillersim.h"

#include "richdef.h"
#include "hpidtrackcand.h"
#include "hevent.h"

#include "showertofinodef.h"

// -----------------------------------------------------------------------------

ClassImp(HPidTrackFillerSim)

// -----------------------------------------------------------------------------

HPidTrackFillerSim::HPidTrackFillerSim(Option_t pOpt[])
    : HPidTrackFiller("PidTrackFillerSim",
            "PID track candidate filler for SIM", pOpt)
{
// Default constructor.

    setDefault();
}

// -----------------------------------------------------------------------------

HPidTrackFillerSim::HPidTrackFillerSim(Text_t name[], Text_t title[],
                        Option_t pOpt[])
    : HPidTrackFiller(name, title, pOpt)
{
// Constructor with name

    setDefault();
}

// -----------------------------------------------------------------------------

void HPidTrackFillerSim::setDefault(void)
{
    cCatShowerTof = catShowerHitTofTrack;
}

// -----------------------------------------------------------------------------

HPidTrackFillerSim::~HPidTrackFillerSim(void)
{
}

// -----------------------------------------------------------------------------

Bool_t HPidTrackFillerSim::getCategories(HEvent *pEvent)
{
// Internal function for getting proper input cats.

    // get/build catRichHit
    if(bNoRich)
        pCatRich = NULL;
    else
    {
        if((pCatRich = pEvent->getCategory(catRichHitSim)) != NULL)
            return kTRUE;

        Warning("getCategories",
                "Cannot get catRichHitSim category. Trying catRichHit");

        if((pCatRich = pEvent->getCategory(catRichHit)) == NULL)
        {
            Error("getCategories", "Cannot get catRichHit category");
            return kFALSE;
        }
    }

    return kTRUE;
}

// -----------------------------------------------------------------------------

HPidTrackCand* HPidTrackFillerSim::getNextSlot() {
// Get next slot from the output category 
// This method set Sim to the output object

HPidTrackCand *pOut;

    if((pOut = HPidTrackFiller::getNextSlot()) != NULL){
        pOut->setSim();
	}

    return pOut;
}

// -----------------------------------------------------------------------------

HPidTrackCand* HPidTrackFillerSim::getNextSlot(HKickTrack *pTrack)
{
// Get next slot from the output category and fills it with data from HKickTrack
// This method set Sim to the output object

HPidTrackCand *pOut;

    if((pOut = HPidTrackFiller::getNextSlot(pTrack)) != NULL)
        pOut->setSim();

    return pOut;
}
