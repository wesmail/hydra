#ifndef HRICHCALPARCELL_H
#define HRICHCALPARCELL_H
#pragma interface

#include "hlocateddataobject.h"

class HRichCalParCell : public HLocatedDataObject {

public:

	HRichCalParCell();
	HRichCalParCell(Float_t fSlope, Float_t fOffset, Float_t fSigma);
	~HRichCalParCell() {}

        Int_t reset();
	Float_t getOffset() { return fOffset; }
	Float_t getSlope() { return fSlope; }
        Float_t getSigma() { return fSigma; }
	void setOffset(Float_t offset) { fOffset = offset; }
        void setSigma(Float_t sigma) { fSigma = sigma;}
	void setSlope(Float_t slope) { fSlope = slope; }
        void setParams(Float_t slope, Float_t offset, Float_t sigma);

        Int_t getAddress() { return fAddress; }
        Short_t getSector() { return fSector; }
        Short_t getRow() { return fRow; }
        Short_t getCol() { return fCol; }

        void setSector(Short_t s) { fSector = s; calcAddress(); }
        void setRow(Short_t r) { fRow = r; calcAddress(); }
        void setCol(Short_t c) { fCol = c; calcAddress(); }

        Int_t getNLocationIndex(void);
        Int_t getLocationIndex(Int_t i);


        ClassDef(HRichCalParCell,1) //ROOT extension

private:

        Int_t calcAddress();

        Short_t fSector;
        Short_t fRow;
        Short_t fCol;

        Int_t fAddress;

	Float_t fOffset;
        Float_t fSlope;
        Float_t fSigma;

};

#endif // HRICHCALPARCELL_H
