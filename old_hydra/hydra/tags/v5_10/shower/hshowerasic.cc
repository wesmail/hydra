#pragma implementation
#include "hshowerasic.h"
ClassImp(HShowerAsic)

////////// **********ShowerAsic defintion ****************/////////////77
HShowerAsic::HShowerAsic() {
   m_fSlope.Set(32);
   m_fOffset.Set(32);
   reset();
}

HShowerAsic::HShowerAsic(Int_t nAsicNr, Float_t fSlope, Float_t fOffset) {
   m_fSlope.Set(32);
   m_fOffset.Set(32);
   m_nAsicNr = nAsicNr;
   setParams(fSlope, fOffset);
}

HShowerAsic::HShowerAsic(Int_t nAsicNr, Float_t* fSlope, Float_t* fOffset) {
   m_fSlope.Set(32);
   m_fOffset.Set(32);
   m_nAsicNr = nAsicNr;
   for (int i = 0; i < 32; i++)
       setParams(i, fSlope[i], fOffset[i]);
}


int HShowerAsic::reset()
{
   setParams(1.0, 0.0);

   m_nAsicNr = -1; 
   return 1;
}

void HShowerAsic::setParams(Float_t fSlope, Float_t fOffset) {
 for (int i = 0; i < 32; i++)
    setParams(i, fSlope, fOffset);
}

void HShowerAsic::setParams(Int_t nIdx, Float_t fSlope, Float_t fOffset) {
    setSlope(nIdx, fSlope);
    setOffset(nIdx, fOffset);
}

