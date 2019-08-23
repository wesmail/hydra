#ifndef HRICHCAL_H
#define HRICHCAL_H
#pragma interface

#include "hlocateddataobject.h"
#include <iostream.h>

class HRichCal : public HLocatedDataObject {

public:

        HRichCal() : fCharge(0) { clear(); }
        HRichCal(const Int_t q) : fCharge(q) {}
        virtual ~HRichCal() {}

        Int_t clear();

        Float_t getCharge() const { return fCharge; }
        Float_t addCharge(Float_t q);
	void setCharge(Float_t q) { fCharge = q; }
        friend ostream& operator<< (ostream& output, HRichCal& element);

        Int_t getSector() { return fSector; }
        Int_t getRow() { return fRow; }
        Int_t getCol() { return fCol; }

        void setSector(Int_t s) { fSector = s; calcAddress(); }
        void setRow(Int_t r) { fRow = r; calcAddress(); }
        void setCol(Int_t c) { fCol = c; calcAddress(); }

        void setEventNr(Int_t lNr) { fEventNr = lNr; }
        Int_t getEventNr() { return fEventNr; }

        Int_t getNLocationIndex();
        Int_t getLocationIndex(Int_t i);

private:

        Int_t calcAddress();

        Int_t fEventNr;
        Int_t fAddress;
        Float_t fCharge; 

        Int_t fSector;
        Int_t fRow;
        Int_t fCol; 

        ClassDef(HRichCal,1) //Rich cal data
};

#endif

