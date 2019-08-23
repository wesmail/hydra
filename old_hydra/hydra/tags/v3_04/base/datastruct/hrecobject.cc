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
  fNParts=0;
}

HRecObject::HRecObject(HRecObject &aObj) {
  //Copy constructor
  fParts.AddAll(&aObj.fParts); 
  fNParts=aObj.fNParts;
}

HRecObject::~HRecObject(void) {
  //Cleaning
  fParts.Delete(); 
  fNParts=0;
}

void HRecObject::setNParts(Int_t aNParts) {
  //Sets the number of parts used to build this HRecObject
  fParts.Expand(aNParts);
  fNParts=aNParts;
}


Int_t HRecObject::getNParts(void) {
  //Returns the number of parts used to build this HRecObject
  return(fNParts);
}

void HRecObject::setPart(Int_t aPartId, Cat_t cat,HLocation &loc) {
  //Sets the category and location of the object corresponding to the part
  //aPartId.
  HDataObjId *id;
  id=new HDataObjId;
  id->setCategory(cat);
  id->setLocation(loc);
   fParts.AddAt(id,aPartId); 
}

HDataObjId *HRecObject::getPartId(Int_t aPartId) {
  // Returns an identifier for the data object which is the part number aPartId
  HDataObjId *r=NULL;
  r=(HDataObjId *)fParts.At(aPartId);
  return r;
}
