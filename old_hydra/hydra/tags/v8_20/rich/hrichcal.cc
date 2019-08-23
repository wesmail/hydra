//_HADES_CLASS_DESCRIPTION 
//////////////////////////////////////////////////////
// HRichCal
//
//   This class contains one hit in one cell of the rich.
//
//////////////////////////////////////////////////////

#pragma implementation

#include "hrichcal.h"

#include <iostream> 
#include <iomanip>

using namespace std;

ClassImp(HRichCal)

ostream& operator<< (ostream& output, HRichCal& raw) {
        output<<"pad fCharge:"<<raw.fCharge;
        return output;
}
void HRichCal::dumpToStdout()
{
    cout<<"CAL: "<<"sec:"<<getSector()<<" col:"<<getCol()<<" row:"<<getRow()
	<<" chrg:"<<getCharge()<<endl;


}


