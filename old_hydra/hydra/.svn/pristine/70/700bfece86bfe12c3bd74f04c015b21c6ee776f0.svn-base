#ifndef HRICHCalMax_H
#define HRICHCalMax_H
#pragma interface

#include "hlocateddataobject.h"




// modified by Wolfgang Koenig Nov. 2000

#include <iostream.h>

class HRichCalMax : public HLocatedDataObject {

public:

        HRichCalMax() : fCharge(0.0F),fSector(-1),fRow(-1),fCol(-1){}
        HRichCalMax(const Float_t q) :
        fCharge(q),fSector(-1),fRow(-1),fCol(-1) {}

        virtual ~HRichCalMax() {}

        Int_t clear() {
          fCharge = 0.0F;
          fSector = -1;
          fRow = fCol = -1;
          return 1;
	}

        Float_t getCharge() const { return fCharge; }
        Float_t addCharge(Float_t q) { return fCharge += q; }
	void setCharge(Float_t q) { fCharge = q; }
        friend ostream& operator<< (ostream& output, HRichCalMax& element);

        Int_t getSector() { return fSector; }
        Int_t getRow() { return fRow; }
        Int_t getCol() { return fCol; }
        Int_t getAddress() {
	return 10000 * (fSector ? fSector : 6) + 100 * fRow + fCol; }

        void setSector(Int_t s) { fSector = s; }
        void setRow(Int_t r) { fRow = r; }
        void setCol(Int_t c) { fCol = c; }

        void setEventNr(Int_t lNr) { fEventNr = lNr; }
        Int_t getEventNr() { return fEventNr; }

        Int_t getNLocationIndex() { return 3; }
        Int_t getLocationIndex(Int_t i);

private:

//        Int_t calcAddress();

        Int_t fEventNr;
//        Int_t fAddress;
        Float_t fCharge; 

        short fSector;
        char fRow;
        char fCol; 

        ClassDef(HRichCalMax,1) //Rich cal data
};

#endif

