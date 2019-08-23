using namespace std;
#include <iostream> 
#include <iomanip>


//////////////////////////////////////////////////////
// HRichCal
//
//   This class contains one hit in one cell of the rich.
//
//////////////////////////////////////////////////////

#pragma implementation
#include "hrichcal.h"
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


