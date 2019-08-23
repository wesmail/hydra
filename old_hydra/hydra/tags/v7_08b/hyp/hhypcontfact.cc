//*-- AUTHOR : I. Froehlich
//*-- Modified : 14/07/2005 by I. Froehlich

/////////////////////////////////////////////////////////////
//
//  HHypContFact
//
//  Factory for the parameter containers in libHyp
//
/////////////////////////////////////////////////////////////
using namespace std;

#include "hhypcontfact.h"
#include "hruntimedb.h"
#include "hparset.h"
#include "hparamlist.h"
#include "hcontfact.h"
#include "hhyprecpar.h"
#include <iostream>
#include <iomanip>

ClassImp(HHypContFact)

static HHypContFact gHHypContFact;

HHypContFact::HHypContFact()
{
  // Constructor (called when the library is loaded)
  fName = "HypContFact";
  fTitle = "Factory for parameter containers in libHyp";
  setAllContainers();
  HRuntimeDb::instance()->addContFactory(this);
}

void HHypContFact::setAllContainers()
{
  // Creates the Container objects with all accepted contexts and adds them to
  // the list of containers for the Hyp library.
  HContainer *cm = new HContainer("HypRecPar",
                                  "Hyp parameters",
                                  "HypRecProductionCuts");

  cm->addContext("HypRecSharpCuts");
  cm->addContext("HypRecWideCuts");

  containers->Add(cm);
}

HParSet *HHypContFact::createContainer(HContainer * c)
{
  // Calls the constructor of the corresponding parameter container.
  // For an actual context, which is not an empty string and not the default context
  // of this container, the name is concatinated with the context.
  const char *name = c->GetName();

  if (strcmp(name, "HypRecPar") == 0)
    return new HHypRecPar(c->getConcatName().Data(), c->GetTitle(),
                          c->getContext());
  return 0;
}
