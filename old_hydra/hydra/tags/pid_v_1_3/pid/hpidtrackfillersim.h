// @(#)$Id: hpidtrackfillersim.h,v 1.2 2003-03-04 17:18:23 jaskula Exp $
//*-- Author  : Marcin Jaskula 11/11/2002

#ifndef HPIDTRACKFILLERSIM_H
#define HPIDTRACKFILLERSIM_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidTrackFillerSim                                                         //
//                                                                            //
// A simple track filler of HPidTrackCand category for sim data               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hpidtrackfiller.h"

// -----------------------------------------------------------------------------

class HPidTrackFillerSim : public HPidTrackFiller
{
public:
                HPidTrackFillerSim(Option_t pOpt[] = "");
                HPidTrackFillerSim(Text_t name[], Text_t title[],
                                Option_t pOpt[] = "");
               ~HPidTrackFillerSim(void);

protected:

    void                   setDefault(void);

    // -------------------------------------------------------------------------

    virtual Bool_t         getCategories(HEvent *pEvent);

    // -------------------------------------------------------------------------

    virtual HPidTrackCand* getNextSlot(HKickTrack *pTrack);

    // -------------------------------------------------------------------------

    ClassDef(HPidTrackFillerSim, 0)  // PID track candidate filler for sim
};

#endif //HPIDTRACKFILLERSIM_H
