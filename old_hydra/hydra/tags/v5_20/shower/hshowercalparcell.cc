#pragma implementation
#include "hshowercalparcell.h"
ClassImp(HShowerCalParCell)

//*-- Author : Leszek Kidon
//*-- Created: 1998

///////////////////////////////////////////////////////////////////////
// HShowerCalParCell
//
// HShowerCalParCell contains slope and offset parameters 
// for calibration of one pad 
//
//////////////////////////////////////////////////////////////////////

HShowerCalParCell::HShowerCalParCell() {
   reset();
}

HShowerCalParCell::HShowerCalParCell(Float_t fSlope, Float_t fOffset) {
   setParams(fSlope, fOffset);
}


int HShowerCalParCell::reset()
{
//reset of all data
   setParams(1.0, 0.0);

   m_nAsicNr = -1; 
   return 1;
}

void HShowerCalParCell::setParams(Float_t fSlope, Float_t fOffset) {
//set slope and offset
 setSlope(fSlope);
 setOffset(fOffset);
}


