//###################################################
//#
// ###################################################

#pragma implementation
#include "hrichcalparcell.h"

ClassImp(HRichCalParCell)

HRichCalParCell::HRichCalParCell() {
 reset();
}

HRichCalParCell::HRichCalParCell(Float_t slope, Float_t offset) {
 setParams(slope, offset);
}


int HRichCalParCell::reset()
{
 setParams(1.0, 0.0);

 return 1;
}

void HRichCalParCell::setParams(Float_t slope, Float_t offset) {
 setSlope(slope);
 setOffset(offset);
}

Int_t HRichCalParCell::calcAddress() {
  Char_t s = (fSector) ? fSector : 6;                               
  fAddress = 10000 * s;
  fAddress += 100 * fRow;
  fAddress += fCol;
   
return fAddress;
}

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



