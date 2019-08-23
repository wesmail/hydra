// $Id: hrichcalmax.cc,v 1.4 2003-01-24 18:24:18 halo Exp $
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

inline Int_t HRichCalMax::getLocationIndex(Int_t i) {
  switch (i) {
   case 0 : return getSector(); break;
   case 1 : return getRow(); break;
   case 2 : return getCol(); break;
  }
  return -1;
}

