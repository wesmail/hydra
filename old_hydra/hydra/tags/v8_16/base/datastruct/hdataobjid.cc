#include "hdataobjid.h"

//*-- Author : Manuel Sanchez
//*-- Modified : 1/04/98 by Manuel Sanchez
//*-- Copyright : GENP (Univ. Santiago de Compostela)

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////
//HDataObjId
// 
//  One object of this class unambigously identifies one HDataObject in the 
//HEvent structure.
////////////////////////////////

ClassImp(HDataObjId)


HDataObjId::HDataObjId(void) {
    fCategory=catInvalid;
}

HDataObjId::HDataObjId(Cat_t aCat,HLocation &aLoc) {
  // Allocates a new Id for an object of category aCat in the location aLoc
  fCategory=aCat;
  fLocation=aLoc;
}

HDataObjId::~HDataObjId(void) {
}

void HDataObjId::setLocation(HLocation &aLoc) {
  // Stores the location of the object referenced by HDataObjId;
  // For more information on the HLocation role in a event, see HEvent
 fLocation=aLoc;
}
