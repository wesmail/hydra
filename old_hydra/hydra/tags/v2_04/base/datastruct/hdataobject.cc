#include "hdataobject.h"
#include "hcategory.h"
#include <iostream.h>

//*-- Author : Manuel Sanchez 
//*-- Modified : 17/04/98
//*-- Copyright : GENP (Univ. Santiago de Compostela)

////////////////////////////////////
//HDataObject
// 
//  ABC for all the data objects
////////////////////////////////

TObject *gNullObject=NULL;

ClassImp(HDataObject)


HDataObject::HDataObject(void) {
  fLocation=NULL;
}


HDataObject::~HDataObject(void) {
  if (fLocation) delete fLocation;
}

void HDataObject::setLocation(HLocation &aLoc) {
  //Sets the location of this object to that given in aLoc
  //
  // For more information on the HLocation role in a event, see HEvent
  if (!fLocation) delete fLocation;
  fLocation=new HLocation(aLoc);
}

HDataObjId *HDataObject::getId(void) {
  // Returns an HDataObjId which unambigously identifies this data object 
  //within the event structure
  HDataObjId *id;
  if (fLocation) id=new HDataObjId(getCategory(),(*fLocation));
  else id=NULL;
  return id;
}

HLocation *&HDataObject::getLocation(void) {
  //Returns the location of this data object within its HCategory
  return fLocation;
}

Cat_t HDataObject::getCategory(void) {
  //Returns the category this data object belongs to
  return catInvalid;
}

Bool_t HDataObject::HasIdentifier(void) {
  // Returns kTRUE if the object knows about its own location in the event 
  //structure, kFALSE in other case.
  return (fLocation==NULL)?kFALSE:kTRUE;
}

// void *HDataObject::operator new(size_t sz,HCategory *cat,HLocation &loc) {
//   TObject *&obj=cat->getSlot(loc);
//   void *r=NULL;
//   if (obj==NULL) {
//    cout << "<HDataObject::new> Attempted to allocate object in NULL slot" << endl;
//    r=TObject::operator new(sz, obj);
//   } else {
//     r=TObject::operator new(sz, obj);
//   }
//   //if (r!=NULL) ((HDataObject *)r)->setLocation(loc);
//   return r;
// }



