//*-- AUTHOR : Ilse Koenig
//*-- Modified last : 13/12/2001 by Ilse Koenig

/////////////////////////////////////////////////////////////
//
//  HBaseContFact
//
//  Factory for the parameter containers in libHydra 
//
/////////////////////////////////////////////////////////////

#include "hbasecontfact.h"
#include "hruntimedb.h"
#include "hspecgeompar.h"
#include <iostream.h>

ClassImp(HBaseContFact)

static HBaseContFact gHBaseContFact;

HBaseContFact::HBaseContFact() {
  // Constructor (called when the library is loaded)
  fName="BaseContFact";
  fTitle="Factory for parameter containers in libHydra";
  setAllContainers();
  HRuntimeDb::instance()->addContFactory(this);  
}

void HBaseContFact::setAllContainers() {
  // Creates the Container objects with all accepted contexts and adds them to
  // the list of containers in libHydra.
  containers->Add(new HContainer(
                    "SpecGeomPar",
                    "Geometry of cave, sectors and target",
                    ""));
}

HParSet* HBaseContFact::createContainer(HContainer* c) {
  // Calls the constructor of the corresponding parameter container.
  // For an actual context, which is not an empty string and not the default context
  // of this container, the name is concatinated with the context.
  const char* name=c->GetName();
  if (strcmp(name,"SpecGeomPar")==0) {
    return new HSpecGeomPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  }
  return 0;
} 
