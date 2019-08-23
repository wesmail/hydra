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
#include "hshowerraw.h"
ClassImp(HShowerRaw)

////////// **********ShowerRaw defintion ****************/////////////77

int HShowerRaw::clear()
{
   m_fCharge = 0.0F;
   m_lEventNr = -1;
   m_nSector = -1;
   m_nModule = -1;
   m_nRow = -1;
   m_nCol = -1;
   m_nAddress = -1;
   
   return 1;
}

inline Float_t HShowerRaw::addCharge(Float_t q) {
  m_fCharge += q;
  return m_fCharge;
}

Int_t HShowerRaw::calcAddress() {
  Char_t s = (m_nSector) ? m_nSector : 6;
  m_nAddress = 100000 * s;
  m_nAddress += 10000 * m_nModule;
  m_nAddress += 100 * m_nRow;
  m_nAddress += m_nCol;

  return m_nAddress;
}

inline Int_t HShowerRaw::getNLocationIndex(void) {
  return 4;
}

inline Int_t HShowerRaw::getLocationIndex(Int_t i) {
  switch (i) {
  case 0 : return getSector(); break;
  case 1 : return getModule(); break;
  case 2 : return getRow(); break;
  case 3 : return getCol(); break;
  }
  return -1;
}

HLocation& HShowerRaw::getLocation() {
  m_curLoc.set(4, m_nSector, m_nModule, m_nRow, m_nCol);

  return m_curLoc;
} 

