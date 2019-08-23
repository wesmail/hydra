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
ClassImp(HShowerRawMatr)

////////// **********ShowerRaw defintion ****************/////////////77

int HShowerRaw::clear()
{
   m_fCharge = 0.0F;
   m_lEventNr = -1;

   m_nSector = -1;
   m_nModule = -1;
   m_nRow = -1;
   m_nCol = -1;

//   m_nAddress = -1;
 
   return 1;
}

inline Float_t HShowerRaw::addCharge(Float_t q) {
  m_fCharge += q;
  if (m_fCharge > 255.0)
      m_fCharge = 255.0;

  return m_fCharge;
}

Int_t HShowerRaw::calcAddress() {
  Int_t nAddress;
  Char_t s = (m_nSector) ? m_nSector : 6;
  nAddress = 100000 * s;
  nAddress += 10000 * m_nModule;
  nAddress += 100 * m_nRow;
  nAddress += m_nCol;

  return nAddress;
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

void HShowerRaw::getLocation(HLocation& loc) {
  loc.setNIndex(getNLocationIndex());
  for(int i = 0; i < getNLocationIndex(); i++)
    loc.setIndex(i, getLocationIndex(i));  
}

