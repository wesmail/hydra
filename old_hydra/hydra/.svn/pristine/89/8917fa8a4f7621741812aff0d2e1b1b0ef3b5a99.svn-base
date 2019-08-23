#ifndef HShowerCalParCell_H
#define HShowerCalParCell_H
#pragma interface

#include "hdataobject.h"

class HShowerCalParCell : public HDataObject {
public:
	HShowerCalParCell();
//	initialization could be used e.g. to clear the data element
	HShowerCalParCell(Float_t fSlope, Float_t fOffset);
	~HShowerCalParCell() {}

        Int_t reset();
	Float_t getOffset() {return m_fOffset;}
	Float_t getSlope() {return m_fSlope;}
	void setOffset(Float_t fOffset){m_fOffset = fOffset;}
	void setSlope(Float_t fSlope){m_fSlope = fSlope;}
        void setParams(Float_t fSlope, Float_t fOffset);

	Int_t getAsicNr() {return m_nAsicNr;}
        void setAsicNr(Int_t nAsicNr){m_nAsicNr = nAsicNr;}

        ClassDef(HShowerCalParCell,1) //ROOT extension

private:
	Float_t m_fOffset;   //offset parameter for calibration of the pad
        Float_t m_fSlope;    //slope parameter for  calibration of the pad

        int m_nAsicNr;  //asic number connected with this pad
};

#endif
