// @(#)$Id: hpidtrackfillersim.h,v 1.1 2002-11-20 17:21:29 jaskula Exp $
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
                HPidTrackFillerSim(void);
                HPidTrackFillerSim(Text_t name[], Text_t title[]);
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
