//*-- Author: Joern Wuestenfeld 07/07/2005
//*-- Modified
//*-- Version: $Id: hmuemueventfilter.cc,v 1.2 2009-07-15 11:34:06 halo Exp $

#include "hmuemueventfilter.h"
#include "heventheader.h"
#include "hevent.h"
#include "hades.h"

//
// HMuEmuEventFilter
//
// Task for filtering events based on the decision that the MU emulation has done.
//

HMUEmuEventFilter::HMUEmuEventFilter(const Text_t *name,const Text_t *title)
  : HReconstructor(name,title)
{
  // Constructor.
  //
  // Input parameters:
  //  Text_t *name
  //    Name of the task
  //  Text_t *title
  //    Title of the task
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //  none
  eventHeader = NULL;
}

HMUEmuEventFilter::HMUEmuEventFilter(void)
  : HReconstructor("mu.evtFilter","mu.evtFilter")
{
  // Default constructor.
  //
  // Input parameters:
  //  none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //  none
  eventHeader = NULL;
}

HMUEmuEventFilter::~HMUEmuEventFilter(void)
{
  // Destructor.
  //
  // Input parameters:
  //  none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //  none
}

Bool_t HMUEmuEventFilter::init(void)
{
  // Initialisation function of task.
  // acquires pointer to the event header.
  //
  // Input parameters:
  //  none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //  Returns kTRUE if event header found, otherwise kFALSE.
  eventHeader = gHades->getCurrentEvent()->getHeader();
  if(!eventHeader)
  {
    return kFALSE;
  }
  return kTRUE;
}

Bool_t HMUEmuEventFilter::finalize(void)
{
  // Finalization function of task.
  // Does nothing.
  //
  // Input parameters:
  //  none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //  Allways kTRUE.
  return kTRUE;
}

Bool_t HMUEmuEventFilter::reinit(void)
{
  // Reinitialization function of task.
  // Does nothing.
  //
  // Input parameters:
  //  none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //  Allways kTRUE.
  return kTRUE;
}

Int_t HMUEmuEventFilter::execute(void)
{
  // Execute function of task, called for each event
  // Checks, wether the trigger decision done by the MU emulation
  // is positiv or negativ.
  //
  // Input parameters:
  //  none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //  Returns kSkipEvent, if the triggerdecision of the MU emulation
  //  is negativ, 0 otherwise.
  Int_t triggerDecision;

  triggerDecision = eventHeader->getMuEmuTriggerDecision();
  if(triggerDecision==0)
  {
    return kSkipEvent;
  }
  return 0;
}

ClassImp(HMUEmuEventFilter)
