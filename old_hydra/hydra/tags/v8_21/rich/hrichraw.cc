// File: hrichraw.cc
// ***************************************************************************

//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 1999/11/19 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified: Nov. 2000 by Wolfgang Koenig (W.Koenig@gsi.de)
using namespace std;
#include <iostream> 
#include <iomanip>
#include "hrichraw.h"

// ***************************************************************************
//_HADES_CLASS_DESCRIPTION 
//////////////////////////////////////////////////////////////////////////////
//
//  HRichRaw
//
//  This class contains one hit in one cell of the rich.
//
//////////////////////////////////////////////////////////////////////////////
// ***************************************************************************


ClassImp(HRichRaw)


//----------------------------------------------------------------------------
ostream& operator<< (ostream& output, HRichRaw& raw) {
        output<<"pad fCharge:"<<raw.fCharge;
        return output;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichRaw::getLocationIndex(Int_t i) {
 switch (i) {
  case 0 : return getSector(); break;
  case 1 : return getRow(); break;
  case 2 : return getCol(); break;
 }
return -1;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichRaw::getLocation(HLocation& loc) {
 loc.setNIndex(getNLocationIndex());
 for(Int_t i = 0; i < getNLocationIndex(); i++)
   loc.setIndex(i, getLocationIndex(i));
}
//============================================================================

void HRichRaw::dumpToStdout()
{
    cout<<"RAW: "<<"sec:"<<getSector()<<" col:"<<getCol()<<" row:"<<getRow()
	<<" chrg:"<<getCharge()<<endl;


}
