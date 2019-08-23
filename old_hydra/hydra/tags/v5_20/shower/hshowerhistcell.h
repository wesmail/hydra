#ifndef HShowerHistCell_H
#define HShowerHistCell_H
#pragma interface

#include "hdataobject.h"

class TH3S;
class TH2F;

class HShowerHistCell : public HDataObject {
public:
	HShowerHistCell();
	HShowerHistCell(Int_t nSect, Int_t nMod, Int_t nRow, Int_t nCol);
//	initialization could be used e.g. to clear the data element
	~HShowerHistCell();

        Bool_t reset();
        Bool_t fill(Int_t nRow, Int_t nCol, Int_t nVal);
        Bool_t draw(Int_t nRow, Int_t nCol, Option_t* opt="");
        Bool_t book();
        Bool_t book(Int_t nRows, Int_t nCols);
        void deleteHist();

        void calculate1(Int_t nRow, Int_t nCol, 
                                  Float_t *pfSlope, Float_t *pfOffset);
        void calculate2(Int_t nRow, Int_t nCol, 
                                  Float_t *pfSlope, Float_t *pfOffset);
        TH3S* getHistogram(){return m_pHist;}
        void writeHistogram();

        Char_t getSector(void) {return m_nSector;}
        Char_t getModule(void) {return m_nModule;}
        Char_t getRows(void) {return m_nRows;}
        Char_t getCols(void) {return m_nCols;}

        void setSector(Char_t s) {m_nSector = s;}
        void setModule(Char_t m) {m_nModule = m;}
        void setRows(Char_t r) {m_nRows = r;}
        void setCols(Char_t c) {m_nCols = c;}

        ClassDef(HShowerHistCell,1) //ROOT extension

private:
        void getName(Char_t* name, Int_t nInt);
        void getTitle(Char_t* title, Int_t nInt);

        void fillData(Int_t nRow, Int_t nCol, Int_t nBins, Int_t * pData);
        Int_t getMax(Int_t nBins, Int_t * pData);
        Float_t getMean(Int_t nBins, Int_t * pData, Int_t nMin,  
                     Int_t nMax,  Int_t nThreshold);

        TH3S *m_pHist;  //!
        TH2F *m_pSlopeHist;  //!
        TH2F *m_pOffsetHist;  //!

        Char_t m_nSector;
        Char_t m_nModule;

        Char_t m_nRows;
	Char_t m_nCols;

        Int_t m_nDataSize; //!
        Int_t* m_pData; //!

        Float_t m_fChannel10pC;
};

#endif
