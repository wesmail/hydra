// File: hrichpadsignal.h
// ****************************************************************************
//
// HRichPadSignal
//
// Author: Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
// Modified: 1999/12/04 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
// ----------------------------------------------------------------------------
//
// ****************************************************************************


#ifndef HRICHPADSIGNAL_H
#define HRICHPADSIGNAL_H

#include "hlocateddataobject.h"
#include <iostream.h>

// ****************************************************************************

class HRichPadSignal : public HLocatedDataObject {

// ****************************************************************************

public:

        HRichPadSignal() : fAmplitude(0) { clear(); }
        HRichPadSignal(const Int_t q) : fAmplitude(q) {}
        virtual ~HRichPadSignal();

        void clear();
        void reset();

        Int_t getAmplitude() { return fAmplitude; }
        Int_t addAmplitude(Int_t ampl) { return (fAmplitude += ampl); };
        Int_t getLabel() { return fLabel; }
        Int_t getLock() { return fLock; }

        void setAmplitude(Int_t ampl) { fAmplitude = ampl; }
        void setLabel(Int_t label) { fLabel = label; }
        void setLock(Int_t lock) { fLock = lock; }

        void CalcNrtoXY(Int_t padsx);
        void CalcXYtoNr(Int_t padsx);


        friend ostream& operator<< (ostream& output, HRichPadSignal& element);

        Int_t getSector() { return fSector; }
        Int_t getCol() { return fCol; }
        Int_t getRow() { return fRow; }
        Int_t getPadNr() { return fPadNr; }

        void setSector(Int_t s) { fSector = s; calcAddress(); }
        void setCol(Int_t c) { fCol = c; calcAddress(); }
        void setRow(Int_t r) { fRow = r; calcAddress(); }
        void setPadNr(Int_t nr) { fPadNr = nr; }

        void setEventNr(Int_t lNr) { fEventNr = lNr; }
        Int_t getEventNr() { return fEventNr; }

        Int_t getNLocationIndex();
        Int_t getLocationIndex(Int_t i);

private:

        Int_t calcAddress();

        Int_t fEventNr;
        Int_t fAddress;

        Int_t fAmplitude; 
        Int_t fLabel;
        Int_t fLock;

        Int_t fSector;
        Int_t fCol; 
        Int_t fRow;
        Int_t fPadNr;

        ClassDef(HRichPadSignal,1) // Rich auxiliary class
};

//============================================================================


#endif // HRICHPADSIGNAL_H

