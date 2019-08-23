// $Id: hrichcalmax.cc,v 1.5 2003-11-21 14:32:51 halo Exp $
using namespace std;
#include <iostream> 
#include <iomanip>


//////////////////////////////////////////////////////
// HRichCalMax
//
//   This class contains one hit in one cell of the rich.
//
//////////////////////////////////////////////////////

#pragma implementation
#include "hrichcalmax.h"
ClassImp(HRichCalMax)

ostream& operator<< (ostream& output, HRichCalMax& raw) {
        output<<"pad fCharge:"<<raw.fCharge;
        return output;
}


