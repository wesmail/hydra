#include "hrecobject.h"
//*-- Author : Manuel Sanchez 
//*-- Modified : 24/03/98 by Manuel Sanchez
//*-- Copyright : GENP (Univ. Santiago de Compostela)

////////////////////////////////////
//HRecObject
//
//  This class stands for the data objects reconstructed using other data
// objects of which you want to keep track.
// 
//  A HRecObject is a HDataObject plus a list of identifiers of the objects
// used to reconstruct this particular data object.
////////////////////////////////

ClassImp(HRecObject)

HRecObject::HRecObject(void) {
  //Constructor
}

HRecObject::HRecObject(HRecObject &aObj) {
  //Copy constructor
}

HRecObject::~HRecObject(void) {
  //Cleaning
}

