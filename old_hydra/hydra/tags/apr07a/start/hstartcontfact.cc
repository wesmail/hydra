//*-- AUTHOR : Ilse Koenig
//*-- Modified last : 18/12/2001 by Ilse Koenig

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////
//
//  HStartContFact
//
//  Factory for the parameter containers in libStart
//
/////////////////////////////////////////////////////////////

#include "hstartcontfact.h"
#include "hruntimedb.h"
#include "hstartlookup.h"
#include "hstartcalpar.h"
#include "hstartselfcopar.h"
#include "hstartgeompar.h"

ClassImp(HStartContFact)

static HStartContFact gHStartContFact;

HStartContFact::HStartContFact() {
  // Constructor (called when the library is loaded)
  fName="StartContFact";
  fTitle="Factory for parameter containers in libStart";
  setAllContainers();
  HRuntimeDb::instance()->addContFactory(this);
}

void HStartContFact::setAllContainers() {
  // Creates the Container objects with all accepted contexts and adds them to
  // the list of containers for the Start library.
  containers->Add(
    new HContainer("StartCalPar",
                   "Calibration parameters for Start",
                   "StartCalProduction"));
  containers->Add(
    new HContainer("StartGeomPar",
                   "Start geometry parameters",
                   "GeomProduction"));
  containers->Add(
    new HContainer("StartLookup",
                   "Lookup table for Start unpacker",
                   "StartLookupProduction"));
  containers->Add(
    new HContainer("StartSelfCoPar",
                   "Selfcoincidence parameters for Start",
                   "StartSelfCoProduction"));
}

HParSet* HStartContFact::createContainer(HContainer* c) {
  // Calls the constructor of the corresponding parameter container.
  // For an actual context, which is not an empty string and not the default context
  // of this container, the name is concatinated with the context.
  const char* name=c->GetName();
  if (strcmp(name,"StartCalPar")==0)
    return new HStartCalPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"StartGeomPar")==0)
    return new HStartGeomPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"StartLookup")==0)
    return new HStartLookup(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"StartSelfCoPar")==0)
    return new HStartSelfCoPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  return 0;
}
