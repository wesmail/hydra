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
#include "hshowerlookupasictab.h"
ClassImp(HShowerLookupAsicTab)

////////// **********ShowerLookupAsicTab defintion ****************/////////////77
HShowerLookupAsicTab::HShowerLookupAsicTab() {
   reset();
}

HShowerLookupAsicTab::HShowerLookupAsicTab(Int_t nIndexes) {
  setNIndex(nIndexes);
  fill(-1);
}

int HShowerLookupAsicTab::reset()
{
   m_nAsicNr.Reset();

   return 1;
}

void HShowerLookupAsicTab::setNIndex(Int_t nIndexes)
{
  m_nAsicNr.Set(nIndexes);
}

void HShowerLookupAsicTab::fill(Int_t nVal)
{
   for (int i = 0; i < m_nAsicNr.fN; i++)
        m_nAsicNr[i] = nVal;
}


Int_t HShowerLookupAsicTab::getAsicNr(Int_t nIdx) {
  return m_nAsicNr[nIdx];
}

void HShowerLookupAsicTab::setAsicNr(Int_t nIdx, Int_t nAsicNr) {
  m_nAsicNr[nIdx] = nAsicNr;
}

Int_t &HShowerLookupAsicTab::operator[](Int_t nIdx) {
  return m_nAsicNr[nIdx];
}
