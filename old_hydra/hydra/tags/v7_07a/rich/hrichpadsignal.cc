// File: hrichpadsignal.cc
// ***************************************************************************

//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 1999/12/04 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified Oct 2000 by W. Koenig (W.Koenig@gsi.de)
using namespace std;
#include <iostream> 
#include <iomanip>
#include "hrichpadsignal.h"

// ***************************************************************************
//////////////////////////////////////////////////////////////////////////////
//
//  HRichPadSignal
//
//  Pad parameters used in HRichAnalysis
//
//////////////////////////////////////////////////////////////////////////////
// ***************************************************************************
          

ClassImp(HRichPadSignal)

//----------------------------------------------------------------------------
ostream& operator<< (ostream& output, HRichPadSignal& raw) {
 output <<"pad amplitude, label, lock: "
        << raw.fAmplitude << ", " << raw.fLabel << ", " << raw.fLock << endl;
 return output;
}
//============================================================================

