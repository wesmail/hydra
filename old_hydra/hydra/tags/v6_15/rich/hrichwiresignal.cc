// File: hrichwiresignal.cc
// ***************************************************************************

//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 1999/12/04 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 13/05/2001 by Laura Fabbietti
using namespace std;
#include <iostream> 
#include <iomanip>
#include "hrichwiresignal.h"

// ***************************************************************************
//////////////////////////////////////////////////////////////////////////////
//
//  HRichWireSignal
//
//  Wire signal (charge calculated by Pola function).
//
//////////////////////////////////////////////////////////////////////////////
// ***************************************************************************


ClassImp(HRichWireSignal)

//----------------------------------------------------------------------------
ostream& operator<< (ostream& output, HRichWireSignal& raw) {
 output <<"sector, wire number, charge, xpos, ypos: "
        << raw.fSector << ", " << raw.fWireNr << ", " 
        << raw.fCharge << ", " << raw.fXpos << ", " << raw.fYpos << endl;
 return output;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichWireSignal::clear() {
 fSector = 0;
 fWireNr = 0;
 fCharge = 0.;
 fXpos = 0.;
 fYpos = 0.;
} 
//============================================================================

