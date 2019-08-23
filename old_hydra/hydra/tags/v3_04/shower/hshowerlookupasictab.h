#ifndef HShowerLookupAsicTab_H
#define HShowerLookupAsicTab_H
#pragma interface

#include "TObject.h"
#include "TArrayI.h"

class HShowerLookupAsicTab : public TObject {
public:
	HShowerLookupAsicTab();
	HShowerLookupAsicTab(Int_t nIndex);

        Int_t reset();
        void fill(Int_t nVal);
        void setNIndex(Int_t nIndexes);

	Int_t getAsicNr(Int_t nIdx);
        void setAsicNr(Int_t nIdx, Int_t nAsicNr);

        Int_t &operator[](Int_t aIdx) ; 
        ClassDef(HShowerLookupAsicTab,1) //ROOT extension

private:
        TArrayI m_nAsicNr;
};

#endif
