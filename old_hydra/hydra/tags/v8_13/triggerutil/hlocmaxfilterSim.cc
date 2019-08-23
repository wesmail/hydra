//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/19/2005

#include "hrichhitIPUSim.h"
#include "hlocmaxfilterSim.h"

//******************************************************************************
//
// HLocMaxFilterSim
//
// Local maximum filter for RICH hits from simulation.
//
//******************************************************************************

ClassImp(HLocMaxFilterSim)
//----------------------------------------------------------------------------
HLocMaxFilterSim::HLocMaxFilterSim(void)
{
  // Default constructor
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  //Return code:
  //   none
}
//============================================================================


Bool_t HLocMaxFilterSim::check(TObject *r)
{
  // Check wether the x coordinate of the RICH hit is set.
  //
  // Input parameters:
  //   TObject *r
  //     Pointer to HRichHitIPUSim object
  //
  // Output parameters:
  //   none
  //
  //Return code:
  //   Returns kTRUE if the x coordinate of the RICH hit is valid (!=999),
  //   otherwise kFALSE;
  if (  ((HRichHitIPUSim*)r)->getX() != 9999 )
    {
      return kTRUE;
    }
  else
    {
      return kFALSE;
    }
}


//----------------------------------------------------------------------------
HLocMaxFilterSim::~HLocMaxFilterSim(void)
{
  // Destructor
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  //Return code:
  //   none 
}

//============================================================================
