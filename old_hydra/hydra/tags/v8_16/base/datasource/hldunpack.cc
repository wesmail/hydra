// File: hldunpack.cc
//
//*-- Author : Walter Karig <W.Karig@gsi.de>
//*-- Modified : 25/03/98 by Manuel Sanchez 
//*-- Modified : 97/11/13 15:23:49 by Walter Karig
//

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////
//HldUnpack
//
//  ABC for the different unpackers.
//
//  The unpackers are used to unpack the data from Lmd files and put them
//into the HEvent structure.
//
//  The job of an unpacker starts when its execute function is called within
//  HLmdSource::getNextEvent(), in this function the unpacker reads data from
//  an HldEvt (corresponding to an event as stored by the data adquisition 
//  system) and extracts the info to the category pRawCat.
///////////////////////////////////////
using namespace std;
#include <iostream>
#include <iomanip>
#include "hldunpack.h"
#include "hldsubevt.h"

HldUnpack::HldUnpack(void) {
  //Constructor por defecto para un Unpacker generico.
  pSubEvt=NULL;
  pRawCat=NULL;
}

HldUnpack::~HldUnpack(void) {
  //Destructor 
}

HldUnpack::HPP const HldUnpack::getpSubEvt(void) {
  //Return a pointer to the subevent read by this unpacker
  return &pSubEvt;
}

int HldUnpack::execute(void) {
  //Default execute function. It just gives some info about the read subevent
  // This function is overriden by derived classes.
   if (pSubEvt)
      cout << "subEvtId = " << pSubEvt->getId() << endl;
    return kTRUE;
}

void HldUnpack::setCategory(HCategory *aCat) {
  //Sets the category where the unpacked data go to.
  pRawCat=aCat;
}

ClassImp(HldUnpack)      

