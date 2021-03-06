// File: hrichraw.cc
// ***************************************************************************

//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 1999/11/19 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)


#include <iostream.h>
#include "hrichraw.h"

// ***************************************************************************
//////////////////////////////////////////////////////////////////////////////
//
//  HRichRaw
//
//  This class contains one hit in one cell of the rich.
//
//////////////////////////////////////////////////////////////////////////////
// ***************************************************************************


ClassImp(HRichRaw)

//----------------------------------------------------------------------------
HRichRaw::HRichRaw() {
 clear();
}
//============================================================================

//----------------------------------------------------------------------------
HRichRaw::HRichRaw(const Float_t charge) {
 clear();
 fCharge = charge;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichRaw::clear() {
 fEventNr = 0;
 fAddress = -1;
 fCharge = 0.;
 fSector = -1;
 fRow = -1;
 fCol = -1;
return 1;
}
//============================================================================

//----------------------------------------------------------------------------
ostream& operator<< (ostream& output, HRichRaw& raw) {
        output<<"pad fCharge:"<<raw.fCharge;
        return output;
}
//============================================================================

//----------------------------------------------------------------------------
Float_t HRichRaw::addCharge(Float_t q) {
 fCharge += q;
 // if (fCharge > 255.) fCharge = 255.;

return fCharge;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichRaw::getNLocationIndex(void) {
 return 3;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichRaw::getLocationIndex(Int_t i) {
 switch (i) {
  case 0 : return getSector(); break;
  case 1 : return getRow(); break;
  case 2 : return getCol(); break;
 }
return -1;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichRaw::getLocation(HLocation& loc) {
 loc.setNIndex(getNLocationIndex());
 for(Int_t i = 0; i < getNLocationIndex(); i++)
   loc.setIndex(i, getLocationIndex(i));
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichRaw::calcAddress() {
  Int_t s = (fSector) ? fSector : 6;
  fAddress = 10000 * s;
  fAddress += 100 * fRow;
  fAddress += fCol;

return fAddress;
}
//============================================================================


