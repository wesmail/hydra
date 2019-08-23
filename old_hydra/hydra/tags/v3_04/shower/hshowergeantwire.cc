#include "hshowergeantwire.h"
#include <iostream.h>

//*-- Author : Manuel Sanchez Garcia
//*-- Modified : 25/03/98
//*-- Copyright : GENP

//////////////////////////////////////////////////////
// HShowerGeantWire
//
//   This class contains one hit in one cell of the rich.
//
//////////////////////////////////////////////////////

ClassImp(HShowerGeantWire)

HShowerGeantWire::HShowerGeantWire(void) {
  Clear();
}

void HShowerGeantWire::Clear(void) {
  setCharge(0.0);
  setXY(0.0, 0.0);
  setWireNr(-1);
  setSector(-1);
  setModule(-1);
  setTrack(-1);
}

void HShowerGeantWire::getXY(Float_t* pfX, Float_t* pfY) {
  *pfX = fXWire;
  *pfY = fYWire;
}

void HShowerGeantWire::setXY(Float_t fX, Float_t fY) {
  fXWire = fX;
  fYWire = fY;
}


ostream& operator<< (ostream& output, HShowerGeantWire& raw) {
        output<<"charge:"<<raw.fCharge;
        output<<"X wire:"<<raw.fXWire;
        output<<"Y wire:"<<raw.fYWire;
        return output;
}    
