// File: hrichpadsignal.h
// ****************************************************************************
//
// HRichPadSignal
//
// Author: Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
// Modified: 1999/12/04 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
// ----------------------------------------------------------------------------
// Modified Oct. 2000 by W.Koenig (w.koenig@gsi.de)
//
// ****************************************************************************


#ifndef HRICHPADSIGNAL_H
#define HRICHPADSIGNAL_H
using namespace std;
#include <TObject.h>
#include <iostream> 
#include <iomanip>

// ****************************************************************************

class HRichPadSignal : public TObject{

// ****************************************************************************

public:

        HRichPadSignal() : fAmplitude(0),fLabel(0),fLock(0) {}
        HRichPadSignal(const Int_t q) : fAmplitude(q) {}
        virtual ~HRichPadSignal() {}

        void clear() { fAmplitude = 0; fLabel = fLock = 0; }

        Int_t getAmplitude() { return fAmplitude; }
        Int_t addAmplitude(Int_t ampl) { return (fAmplitude += ampl); };
        Int_t getLabel() { return fLabel; }
        Int_t getLock() { return fLock; }

        void setAmplitude(Int_t ampl) { fAmplitude = (short)ampl; }
        void setLabel(Int_t label) { fLabel = (char)label; }
        void setLock(Int_t lock) { fLock = (char)lock; }

        friend ostream& operator<< (ostream& output, HRichPadSignal& element);

private:

        Short_t fAmplitude;
        Char_t fLabel;
        Char_t fLock;

        ClassDef(HRichPadSignal,1) // Rich auxiliary class
};

//============================================================================


#endif // HRICHPADSIGNAL_H

