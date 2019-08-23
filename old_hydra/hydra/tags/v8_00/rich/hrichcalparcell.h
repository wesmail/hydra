
#ifndef HRICHCALPARCELL_H
#define HRICHCALPARCELL_H
#pragma interface

#include "hlocateddataobject.h"

class HRichCalParCell : public HLocatedDataObject {
private:

        Short_t fSector;
//        char fRow;
//        char fCol;
        Short_t fRow;
        Short_t fCol;

        Int_t fAddress; // not needed

	Float_t fOffset;
        Float_t fSlope;
        Float_t fSigma;

public:

        HRichCalParCell():fAddress(0) { reset(); }

        HRichCalParCell(Float_t slope, Float_t offset, Float_t sigma) {
          setParams(slope, offset, sigma);
        }
	~HRichCalParCell() {}

        Int_t reset() { setParams(1.0F,0.0F,0.0F); return 1;}
	Float_t getOffset() { return fOffset; }
	Float_t getSlope() { return fSlope; }
        Float_t getSigma() { return fSigma; }
	void setOffset(Float_t offset) { fOffset = offset; }
        void setSigma(Float_t sigma) { fSigma = sigma;}
	void setSlope(Float_t slope) { fSlope = slope; }
        void setParams(Float_t slope, Float_t offset, Float_t sigma) {
	  fOffset=offset; fSlope=slope; fSigma=sigma;
	}

        Int_t getAddress() {
	  return 10000 * (fSector ? fSector : 6) + 100 * fRow + fCol;
   	}
        Short_t getSector() { return fSector; }
        Short_t getRow() { return fRow; }
        Short_t getCol() { return fCol; }

        void setSector(Short_t s) { fSector = s; }
        void setRow(Short_t r) { fRow = r; }
        void setCol(Short_t c) { fCol = c; }

        inline Int_t getNLocationIndex(void);
        inline Int_t getLocationIndex(Int_t i);


        ClassDef(HRichCalParCell,1) //ROOT extension
};

inline Int_t HRichCalParCell::getNLocationIndex() {
  return 3;
}

inline Int_t HRichCalParCell::getLocationIndex(Int_t i) {
 switch (i) {
  case 0 : return getSector(); break;
  case 1 : return getRow(); break;
  case 2 : return getCol(); break;
 }
return -1;
}

#endif // HRICHCALPARCELL_H







