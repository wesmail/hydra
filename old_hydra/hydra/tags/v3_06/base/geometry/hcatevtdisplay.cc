//*-- Author : Ilse Koenig
//*-- Modified : 11/07/99 by Ilse Koenig

///////////////////////////////////////////////////////////////////////////////
//
// HCatEvtDisplay
// 
// Base class for the event displays of categories
//
///////////////////////////////////////////////////////////////////////////////

#include "hcatevtdisplay.h"

ClassImp(HCatEvtDisplay)

HCatEvtDisplay::HCatEvtDisplay(void) {
  // default constructor
  cat=0;
  isActive=kFALSE;
}

void HCatEvtDisplay::setActive(Bool_t f) {
  // sets the isActive flag
  // if this flag is kFALSE the execute function is not called
  isActive=f;
}
