// File: hrichraw.h
// ****************************************************************************
//
// HRichRaw
//
// Author: Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
// Modified: 1999/11/19 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
// Modified by Wolfgang Koenig Nov. 2000
// ----------------------------------------------------------------------------
//
// ****************************************************************************


#ifndef HRICHRAW_H
#define HRICHRAW_H


#include "hlocateddataobject.h"
#include <iostream.h>


// ****************************************************************************

class HRichRaw : public HLocatedDataObject {

// ****************************************************************************


public:

        HRichRaw():fCharge(0.0F),fSector(-1),fRow(-1),fCol(-1){}; 
        HRichRaw(const Float_t charge):
        fCharge(charge),fSector(-1),fRow(-1),fCol(-1){};

        virtual ~HRichRaw() {}

        Int_t clear() {
          fCharge = 0.0F;
          fSector = -1;
          fRow = fCol = -1;
          return 1;
	}

        Float_t getCharge() { return fCharge; }
        Float_t addCharge(Float_t q) { return fCharge += q; }
	void setCharge(Float_t q) { fCharge = q; }

        friend ostream& operator<< (ostream& output, HRichRaw& element);

        Int_t getSector() { return fSector; }
        Int_t getRow() { return fRow; }
        Int_t getCol() { return fCol; }

        void setSector(Int_t s) { fSector = s; }
        void setRow(Int_t r) { fRow = r; }
        void setCol(Int_t c) { fCol = c; }

        void setEventNr(Int_t nr) { fEventNr = nr; }
        Int_t getEventNr() { return fEventNr; }

        Int_t getNLocationIndex() { return 3; }
        Int_t getLocationIndex(Int_t i);

        void getLocation(HLocation& loc);

    void dumpToStdout();
private:

//        Int_t calcAddress();

        Int_t fEventNr;
//        Int_t fAddress;

        Float_t fCharge; 

        short fSector;
        char fRow;
        char fCol; 

        ClassDef(HRichRaw,2) // Rich raw data
};

//============================================================================


#endif // HRICHRAW_H

