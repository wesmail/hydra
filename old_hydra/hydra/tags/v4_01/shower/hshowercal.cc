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
#include "hshowercal.h"
ClassImp(HShowerCal)

////////// **********ShowerCal defintion ****************/////////////77

HShowerCal::HShowerCal() {
  clear();
}

HShowerCal::HShowerCal(const Float_t fCharge) {
  clear();
  m_fCharge = fCharge;
}


int HShowerCal::clear()
{
   m_fCharge=0.0F;
   m_nChannel=0;
   m_nSector = -1;
   m_lEventNr = -1;
   m_nModule = -1;
   m_nRow = -1;
   m_nCol = -1;
   m_nAddress = -1;

   resetLocalMax(); 
   return 1;
}

inline Float_t HShowerCal::addCharge(Float_t q) {
  m_fCharge += q;
  if (m_fCharge > 255.0)
      m_fCharge = 255.0;

  m_nChannel = (Int_t) m_fCharge;

  return m_fCharge;
}

inline void HShowerCal::setCharge(Float_t q) {
  m_fCharge = q;
  m_nChannel = (Int_t) m_fCharge;
}

Int_t HShowerCal::calcAddress() {
  Char_t s = (m_nSector) ? m_nSector : 6;
  m_nAddress = 100000 * s;
  m_nAddress += 10000 * m_nModule;
  m_nAddress += 100 * m_nRow;
  m_nAddress += m_nCol;

  return m_nAddress;
}

inline Int_t HShowerCal::getNLocationIndex(void) {
  return 4;
}

inline Int_t HShowerCal::getLocationIndex(Int_t i) {
  switch (i) {
  case 0 : return getSector(); break;
  case 1 : return getModule(); break;
  case 2 : return getRow(); break;
  case 3 : return getCol(); break;
  }
  return -1;
}



