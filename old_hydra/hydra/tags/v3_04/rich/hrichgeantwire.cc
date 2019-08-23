#include "hrichgeantwire.h"
#include <iostream.h>

//*-- Author : Manuel Sanchez Garcia
//*-- Modified : 25/03/98
//*-- Copyright : GENP

//////////////////////////////////////////////////////
// HRichGeantWire
//
//   This class contains one hit in one cell of the rich.
//
//////////////////////////////////////////////////////

ClassImp(HRichGeantWire)

HRichGeantWire::HRichGeantWire(void) {
  Clear();
}

void HRichGeantWire::Clear(void) {
  setCharge(0.0);
  setXY(0.0, 0.0);
  setSrcQ(0);
  setWireNr(-1);
}

void HRichGeantWire::getXY(Float_t* pfX, Float_t* pfY) {
  *pfX = fXWire;
  *pfY = fYWire;
}

void HRichGeantWire::setXY(Float_t fX, Float_t fY) {
  fXWire = fX;
  fYWire = fY;
}


ostream& operator<< (ostream& output, HRichGeantWire& raw) {
        output<<"charge:"<<raw.fCharge;
        output<<"X wire:"<<raw.fXWire;
        output<<"Y wire:"<<raw.fYWire;
        return output;
}    
