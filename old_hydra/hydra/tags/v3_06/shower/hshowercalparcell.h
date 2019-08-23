#ifndef HShowerCalParCell_H
#define HShowerCalParCell_H
#pragma interface

#include "hlocateddataobject.h"

class HShowerCalParCell : public HLocatedDataObject {
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

        Int_t getAddress(void) {return m_nAddress;}
        Char_t getSector(void) {return m_nSector;}
        Char_t getModule(void) {return m_nModule;}
        Char_t getRow(void) {return m_nRow;}
        Char_t getCol(void) {return m_nCol;}

        void setSector(Char_t s) {m_nSector = s;calcAddress();}
        void setModule(Char_t m) {m_nModule = m;calcAddress();}
        void setRow(Char_t r) {m_nRow = r;calcAddress();}
        void setCol(Char_t c) {m_nCol = c;calcAddress();}

        Int_t getNLocationIndex(void);
        Int_t getLocationIndex(Int_t i);

	Int_t getAsicNr() {return m_nAsicNr;}
        void setAsicNr(Int_t nAsicNr){m_nAsicNr = nAsicNr;}

        ClassDef(HShowerCalParCell,1) //ROOT extension

private:
        Int_t calcAddress();

        Char_t m_nSector;
        Char_t m_nModule;
        Char_t m_nRow;
        Char_t m_nCol;
        Int_t m_nAddress;

	Float_t m_fOffset;
        Float_t m_fSlope;

        int m_nAsicNr;
};

#endif
