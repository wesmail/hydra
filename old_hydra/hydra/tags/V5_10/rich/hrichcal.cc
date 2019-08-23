#include <iostream.h>


//////////////////////////////////////////////////////
// HRichCal
//
//   This class contains one hit in one cell of the rich.
//
//////////////////////////////////////////////////////

#pragma implementation
#include "hrichcal.h"
ClassImp(HRichCal)

ostream& operator<< (ostream& output, HRichCal& raw) {
        output<<"pad fCharge:"<<raw.fCharge;
        return output;
}

Int_t HRichCal::clear() {
 fEventNr = 0;
 fAddress = -1;
 fCharge = 0.;
 fSector = -1;
 fRow = -1;
 fCol = -1;

return 1;
} 


Float_t HRichCal::addCharge(Float_t q) {
  fCharge += q;
  // if (fCharge > 255.) fCharge = 255.;

  return fCharge;
}


inline Int_t HRichCal::getNLocationIndex(void) {
  return 3;
}


inline Int_t HRichCal::getLocationIndex(Int_t i) {
  switch (i) {
   case 0 : return getSector(); break;
   case 1 : return getRow(); break;
   case 2 : return getCol(); break;
  }
  return -1;
}


Int_t HRichCal::calcAddress() {
 Int_t s = (fSector) ? fSector : 6;
 fAddress = 10000 * s;
 fAddress += 100 * fRow;
 fAddress += fCol;

return fAddress;
}
