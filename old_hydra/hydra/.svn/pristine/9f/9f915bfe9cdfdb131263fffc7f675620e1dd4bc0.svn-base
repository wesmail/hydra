/////////////////////////////////////////////////////////////
//
//  HTriggerContFact
//
//  Factory for the parameter containers in libTrigger
//
/////////////////////////////////////////////////////////////

#include "htriggercontfact.h"
#include "hruntimedb.h"
#include "htriggerparmulep.h"
#include "htriggerparmudilep.h"
#include "htriggerparmommap.h"
#include "htriggerparshowermap.h"

ClassImp(HTriggerContFact)

static HTriggerContFact gHTriggerContFact;

HTriggerContFact::HTriggerContFact() {
  // Constructor (called when the library is loaded)
  fName="TriggerContFact";
  fTitle="Factory for parameter containers in libTrigger";
  setAllContainers();
  HRuntimeDb::instance()->addContFactory(this);
}

void HTriggerContFact::setAllContainers() {
  // Creates the Container objects with all accepted contexts and adds them to
  // the list of containers for the Trigger library.
  containers->Add(
    new HContainer("TriggerParMuLep",
                   "Parameters for MU Lepton Emulation",
                   "TriggerProduction"));
  containers->Add(
    new HContainer("TriggerParMuDilep",
                   "Parameters for MU Dilepton Emulation",
                   "TriggerProduction"));
  containers->Add(
    new HContainer("TriggerParMomMap",
                   "Parameters for MU Momentum Mapping",
                   "TriggerProduction"));
  containers->Add(
    new HContainer("TriggerParShowerMap",
                   "Parameters for Shower Mapping",
                   "TriggerProduction"));
}

HParSet* HTriggerContFact::createContainer(HContainer* c) {
  // Calls the constructor of the corresponding parameter container.
  // For an actual context, which is not an empty string and not the default context
  // of this container, the name is concatinated with the context.
  const char* name=c->GetName();
  if (strcmp(name,"TriggerParMuLep")==0)
    return new HTriggerParMuLep(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"TriggerParMuDilep")==0)
    return new HTriggerParMuDilep(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"TriggerParMomMap")==0)
    return new HTriggerParMomMap(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"TriggerParShowerMap")==0)
    return new HTriggerParShowerMap(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  return 0;
}














