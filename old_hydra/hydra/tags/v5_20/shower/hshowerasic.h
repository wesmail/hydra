#ifndef HShowerAsic_H
#define HShowerAsic_H
#pragma interface

#include "TObject.h"
#include "TArrayF.h"

class HShowerAsic : public TObject {
public:
	HShowerAsic();
//	initialization could be used e.g. to clear the data element
	HShowerAsic(Int_t nAsicNr, Float_t fSlope = 1.0, Float_t fOffset = 0.0);
	HShowerAsic(Int_t nAsicNr, Float_t *fSlope, Float_t *fOffset);
	~HShowerAsic() {}

        Int_t reset();
	Float_t getOffset(Int_t nIdx) {return m_fOffset[nIdx];}
	Float_t getSlope(Int_t nIdx) {return m_fSlope[nIdx];}
	void setOffset(Int_t nIdx, Float_t fOffset){m_fOffset[nIdx] = fOffset;}
	void setSlope(Int_t nIdx, Float_t fSlope){m_fSlope[nIdx] = fSlope;}
        void setParams(Float_t fSlope, Float_t fOffset);
        void setParams(Int_t nIdx, Float_t fSlope, Float_t fOffset);

	Int_t getAsicNr() {return m_nAsicNr;}
        void setAsicNr(Int_t nAsicNr){m_nAsicNr = nAsicNr;}

        ClassDef(HShowerAsic,1) //ROOT extension

private:
	TArrayF m_fOffset;
        TArrayF m_fSlope;

        int m_nAsicNr;
};

#endif
