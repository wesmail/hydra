//*-- AUTHOR : Ilse Koenig
//*-- Created : 09/05/2005

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////
//
//  HHodoContFact
//
//  Factory for the parameter containers in libHodo
//
/////////////////////////////////////////////////////////////

#include "hhodocontfact.h"
#include "hruntimedb.h"
#include "hhodolookup.h"
#include "hhodocalpar.h"
#include "hhodorefhitpar.h"

ClassImp(HHodoContFact)

static HHodoContFact gHHodoContFact;

HHodoContFact::HHodoContFact(void) {
  // Constructor (called when the library is loaded)
  fName="HodoContFact";
  fTitle="Factory for parameter containers in libHodo";
  setAllContainers();
  HRuntimeDb::instance()->addContFactory(this);
}

void HHodoContFact::setAllContainers(void) {
  // Creates the Container objects with all accepted contexts and adds them to
  // the list of containers for the Hodo library.
  containers->Add(
    new HContainer("HodoLookup",
                   "Unpacker lookup table for the (Pion) Hodo",
                   "HodoLookupProduction"));

  containers->Add(
    new HContainer("HodoCalPar",
                   "Calibration parameters for (Pion) Hodo",
                   "HodoCalProduction"));

  containers->Add(
    new HContainer("HodoRefHitPar",
                   "Reference Trigger and Hit parameters for (Pion) Hodo",
                   "HodoRawAndHitProduction"));

}

HParSet* HHodoContFact::createContainer(HContainer* c) {
  // Calls the constructor of the corresponding parameter container.
  // For an actual context, which is not an empty string and not the default context
  // of this container, the name is concatinated with the context.
  const Char_t* name=c->GetName();
  if (strcmp(name,"HodoLookup")==0)
    return new HHodoLookup(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"HodoCalPar")==0)
    return new HHodoCalPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"HodoRefHitPar")==0)
    return new HHodoRefHitPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());

  return 0;
}
