// $Id: hrichcalmax.cc,v 1.6 2006-08-12 12:53:41 halo Exp $
using namespace std;
#include <iostream> 
#include <iomanip>


//_HADES_CLASS_DESCRIPTION 
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


