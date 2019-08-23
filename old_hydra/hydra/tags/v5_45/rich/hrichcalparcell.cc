//###################################################
//# modified by W. Koenig Nov 2000
// ###################################################

#pragma implementation
#include "hrichcalparcell.h"

ClassImp(HRichCalParCell)


inline Int_t HRichCalParCell::getNLocationIndex() {
  return 3;
}

inline Int_t HRichCalParCell::getLocationIndex(Int_t i) {
 switch (i) {
  case 0 : return getSector(); break;
  case 1 : return getRow(); break;
  case 2 : return getCol(); break;
 }
return -1;
}




