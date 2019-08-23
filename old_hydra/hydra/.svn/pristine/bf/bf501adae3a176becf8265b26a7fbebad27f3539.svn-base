//*-- AUTHOR : Ilse Koenig
//*-- Modified last : 14/12/2001 by Ilse Koenig

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////
//
//  HTofinoContFact
//
//  Factory for the parameter containers in libTofino 
//
/////////////////////////////////////////////////////////////

#include "htofinocontfact.h"
#include "hruntimedb.h"
#include "htofinocalpar.h"
#include "htofinoshowermap.h"
#include "htofinolookup.h"
#include "htofinogeompar.h"
#include "htofinodigitpar.h"

ClassImp(HTofinoContFact)

static HTofinoContFact gHTofinoContFact;

HTofinoContFact::HTofinoContFact() {
  // Constructor (called when the library is loaded)
  fName="TofinoContFact";
  fTitle="Factory for parameter containers in libTofino";
  setAllContainers();
  HRuntimeDb::instance()->addContFactory(this);  
}

void HTofinoContFact::setAllContainers() {
  // Creates the Container objects with all accepted contexts and adds them to
  // the list of containers for the Tofino library.
  containers->Add(
    new HContainer("TofinoCalPar",
                   "Calibration parameters for Tofino",
                   "TofinoStandardCalib"));

  
  containers->Add(
    new HContainer("TofinoGeomPar",
                   "Tofino geometry parameters",
                   "GeomProduction"));
  containers->Add(
    new HContainer("TofinoLookup",
                   "Lookup table for Tofino unpacker",
                   ""));
  containers->Add(
    new HContainer("TofinoShowerMap",
                   "Position of shower pads relative to tofino cells",
                   "TofinoStandardShowerMap"));
  containers->Add(
    new HContainer("TofinoDigitPar",
                   "Tofino digitisation parameters",
                   "TofinoDigitParProduction"));
}

HParSet* HTofinoContFact::createContainer(HContainer* c) {
  // Calls the constructor of the corresponding parameter container.
  // For an actual context, which is not an empty string and not the default context
  // of this container, the name is concatinated with the context.
  const char* name=c->GetName();
  if (strcmp(name,"TofinoCalPar")==0)
    return new HTofinoCalPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"TofinoGeomPar")==0)
    return new HTofinoGeomPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"TofinoLookup")==0)
    return new HTofinoLookup(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"TofinoShowerMap")==0)
    return new HTofinoShowerMap(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"TofinoDigitPar")==0)
    return new HTofinoDigitPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  return 0;
} 
