//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/19/2005

#include "hrichhitIPU.h"
#include "hlocmaxfilter.h"

//******************************************************************************
//
// HLocMaxFilter
//
// Local maximum filter for RICH hits.
//
//******************************************************************************

ClassImp(HLocMaxFilter)
//----------------------------------------------------------------------------
HLocMaxFilter::HLocMaxFilter(void)
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


Bool_t HLocMaxFilter::check(TObject *r)
{
  // Check wether the x coordinate of the RICH hit is set.
  //
  // Input parameters:
  //   TObject *r
  //     Pointer to HRichHitIPU object
  //
  // Output parameters:
  //   none
  //
  //Return code:
  //   Returns kTRUE if the x coordinate of the RICH hit is valid (!=999),
  //   otherwise kFALSE;
  if (  ((HRichHitIPU*)r)->getX() != 9999 )
    {
      return kTRUE;
    }
  else
    {
      return kFALSE;
    }
}


//----------------------------------------------------------------------------
HLocMaxFilter::~HLocMaxFilter(void)
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
