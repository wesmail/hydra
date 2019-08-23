// File: hrichglobal.cc
//
// Author: Soenke Schroeder <soenke.schroeder@physik.tu-muenchen.de>
// Last update by Soenke Schroeder: 00/08/09 11:15:25
//
using namespace std;
#include <iostream> 
#include <iomanip>
#pragma implementation
#include "hrichglobal.h"

ClassImp(HRichGlobal)

Int_t HRichGlobal::clear() {
    fSector = -1;
    fEventNr = 0;
    fGlobalEventNr = 0;
    fGlobalTheta1 = 0.;
    fGlobalTheta2 = 0.;
    fGlobalPhi1 =0.;
    fGlobalPhi2 =0.;
    fGlobalN1 =0;
    fGlobalN2 =0;
    return 1;
}
