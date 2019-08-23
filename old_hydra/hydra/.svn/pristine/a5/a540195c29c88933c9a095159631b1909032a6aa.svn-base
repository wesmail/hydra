#ifndef HTofinoShowerMapCell_H
#define HTofinoShowerMapCell_H
#pragma interface

#include "hdataobject.h"

class HTofinoShowerMapCell : public HDataObject {
public:
	HTofinoShowerMapCell():m_nCellNumber(0){}
//	initialization could be used e.g. to clear the data element
	HTofinoShowerMapCell(Int_t nCell){m_nCellNumber = nCell;}
	~HTofinoShowerMapCell() {}

	Int_t getCellNumber() {return m_nCellNumber;}
        void setCellNumber(Int_t nCell){m_nCellNumber = nCell;}

        ClassDef(HTofinoShowerMapCell,1) //ROOT extension

private:
	Int_t m_nCellNumber; //number of tofino cell which cover 
                              // the same area as pad in shower detector	
};

#endif
