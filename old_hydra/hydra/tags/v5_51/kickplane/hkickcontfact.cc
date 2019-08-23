//*-- AUTHOR : Ilse Koenig
//*-- Modified last : 21/01/2002 by Ilse Koenig

/////////////////////////////////////////////////////////////
//
//  HKickContFact
//
//  Factory for the parameter containers in libKick
//
/////////////////////////////////////////////////////////////

#include "hkickcontfact.h"
#include "hruntimedb.h"
#include "hkicktrackfpar.h"
#include "hkickplane.h"
#include <iostream.h>

ClassImp(HKickContFact)

static HKickContFact gHKickContFact;

HKickContFact::HKickContFact() {
  // Constructor (called when the library is loaded)
  fName="KickContFact";
  fTitle="Factory for parameter containers in libKick";
  setAllContainers();
  HRuntimeDb::instance()->addContFactory(this);
}

void HKickContFact::setAllContainers() {
  // Creates the Container objects with all accepted contexts and adds them to
  // the list of containers for the Kickplane library.
  containers->Add(
    new HContainer("KickPlaneMeta",
                   "Parameter container for the kick plane",
                   ""));
  containers->Add(
    new HContainer("KickPlaneMDC3",
                   "Parameter container for the kick plane",
                   ""));
  containers->Add(
    new HContainer("KickTrackFPar",
                   "Parameter container for the track finder of the kick plane",
                   ""));
}

HParSet* HKickContFact::createContainer(HContainer* c) {
  // Calls the constructor of the corresponding parameter container.
  // For an actual context, which is not an empty string and not the default context
  // of this container, the name is concatinated with the context.
  const char* name=c->GetName();
  if (strcmp(name,"KickPlaneMeta")==0)
    return new HKickPlane(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"KickPlaneMDC3")==0)
    return new HKickPlane(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"KickTrackFPar")==0)
    return new HKickTrackFPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  return 0;
}
