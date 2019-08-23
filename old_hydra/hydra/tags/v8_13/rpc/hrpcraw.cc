//*-- AUTHORS :  Pablo Cabanelas / Hector Alvarez Pol
//*-- Created : 18/10/2005
//*-- Modified: 28/09/2006

/////////////////////////////////////////////////////////////
//
//  HRpcRaw
//
//  Class for the raw data of the RPC detector
//
/////////////////////////////////////////////////////////////

#include "hrpcraw.h"

ClassImp(HRpcRaw)

HRpcRaw::HRpcRaw(void) {
     leftNHits = rightNHits = 0;
     leftMult = rightMult = 0;
     rightT = leftT = -999.0;
     rightW = leftW = -999.0;
     sec  = -10;
     mod  = -10;
     cell = -10;
     chan = -1;
     flag = -1;
}

HRpcRaw::~HRpcRaw(void) {
}
