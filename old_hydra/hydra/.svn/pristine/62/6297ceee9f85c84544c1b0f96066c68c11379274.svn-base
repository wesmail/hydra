//###################################################
//#
//#       
//#                      FRAMEWORK
//#
//#       Authors:                           W.Koenig
//#       adoption to framework & ROOT       W.Schoen
//#    
//#       last mod. W.Schoen 19.1.98
// ###################################################

#pragma implementation
#include "hshowercalparcell.h"
ClassImp(HShowerCalParCell)

////////// **********ShowerCalParCell defintion ****************/////////////77
HShowerCalParCell::HShowerCalParCell() {
   reset();
}

HShowerCalParCell::HShowerCalParCell(Float_t fSlope, Float_t fOffset) {
   setParams(fSlope, fOffset);
}


int HShowerCalParCell::reset()
{
   setParams(1.0, 0.0);
 
   return 1;
}

void HShowerCalParCell::setParams(Float_t fSlope, Float_t fOffset) {
 setSlope(fSlope);
 setOffset(fOffset);
}

Int_t HShowerCalParCell::calcAddress() {
  Char_t s = (m_nSector) ? m_nSector : 6;                               
  m_nAddress = 100000 * s;
  m_nAddress += 10000 * m_nModule;
  m_nAddress += 100 * m_nRow;
  m_nAddress += m_nCol;
   
  return m_nAddress;
}

inline Int_t HShowerCalParCell::getNLocationIndex(void) {
  return 4;
}

inline Int_t HShowerCalParCell::getLocationIndex(Int_t i) {
  switch (i) {
  case 0 : return getSector(); break;
  case 1 : return getModule(); break;
  case 2 : return getRow(); break;
  case 3 : return getCol(); break;
  }
  return -1;
}



