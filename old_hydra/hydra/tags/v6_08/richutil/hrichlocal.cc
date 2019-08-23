// File: hrichlocal.cc
//
// Author: Soenke Schroeder <soenke.schroeder@physik.tu-muenchen.de>
// Last update by Soenke Schroeder: 01/05/14 15:05:24
//
#include <iostream.h>
#pragma implementation
#include "hrichlocal.h"

ClassImp(HRichLocal)

Int_t HRichLocal::clear() {
    fRow = -1;
    fCol = -1;
    fSector = -1;
    fAddress = -1;
    fEventNr = 0;
    fLocalEventNr = 0;
    fLocalCharge = 0.;
    fLocalTheta = 0.;
    fLocalPhi = 0.;
    fLocalEnergy = 0.;
    fRingEventNr = 0;
    fRing = 0;
    return 1;
}
