// File: hrichpadsignal.cc
// ***************************************************************************

//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 1999/12/04 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)


#include <iostream.h>
#include "hrichpadsignal.h"

// ***************************************************************************
//////////////////////////////////////////////////////////////////////////////
//
//  HRichPadSignal
//
//  Pad parameters used in HRichAnalysis
//
//////////////////////////////////////////////////////////////////////////////
// ***************************************************************************


ClassImp(HRichPadSignal)

//----------------------------------------------------------------------------
ostream& operator<< (ostream& output, HRichPadSignal& raw) {
 output <<"pad amplitude, label, lock: "
        << raw.fAmplitude << ", " << raw.fLabel << ", " << raw.fLock << endl;
 return output;
}
//============================================================================

//----------------------------------------------------------------------------
HRichPadSignal::~HRichPadSignal(void) {
}

void HRichPadSignal::reset() {
 fEventNr = 0;
 fAddress = 0;
 fAmplitude = 0;
 fLabel = 0;
 fLock = 0;
 fSector = 0;
 fRow = 0;
 fCol = 0;
 fPadNr = 0;
} 
//============================================================================

//----------------------------------------------------------------------------
void HRichPadSignal::clear() {
 fAmplitude = 0;
 fLabel = 0;
 fLock = 0;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichPadSignal::CalcNrtoXY(Int_t padsx) {
 fCol = fPadNr % padsx;
 fRow = fPadNr / padsx;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichPadSignal::CalcXYtoNr(Int_t padsx) {
 fPadNr = fCol + padsx*fRow;
}
//============================================================================

//----------------------------------------------------------------------------
inline Int_t HRichPadSignal::getNLocationIndex(void) {
  return 3;
}
//============================================================================

//----------------------------------------------------------------------------
inline Int_t HRichPadSignal::getLocationIndex(Int_t i) {
  switch (i) {
   case 0 : return getSector(); break;
   case 1 : return getRow(); break; // this corresponds to y
   case 2 : return getCol(); break; // this corresponds to x
  }
  return -1;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichPadSignal::calcAddress() {
 Int_t s = (fSector) ? fSector : 6;
 fAddress = 10000 * s;
 fAddress += 100 * fRow;
 fAddress += fCol;

return fAddress;
}
//============================================================================

