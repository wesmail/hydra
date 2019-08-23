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
#include "hkickplane2.h"
#include "hkickmatchpar.h"
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
  containers->Add(new HContainer("KickPlaneMeta",
				 "Parameter container for the kick plane",
				 "KickPlaneProduction"));
  
  containers->Add(new HContainer("KickPlaneMDC3",
				 "Parameter container for the kick plane",
				 "KickPlaneProduction"));

  containers->Add(new HContainer("KickTrackFPar",
				 "Parameter container for the track finder of "
				 "the kick plane",
				 "KickTrackFProduction"));

  containers->Add(new HContainer("KickPlane2Meta",
				 "Parameter container for kickplane v2",
				 "KickPlane2Production"));

  containers->Add(new HContainer("KickPlane2MDC3",
				 "Parameter container for kickplane v2",
				 "KickPlane2Production"));

  containers->Add(new HContainer("KickMatchParMeta",
				 "Matching parameters for kickplane v2",
				 "KickMatchProduction"));

  containers->Add(new HContainer("KickMatchParMDC3",
				 "Matching parameters for kickplane v2",
				 "KickMatchProduction"));
}

HParSet* HKickContFact::createContainer(HContainer* c) {
  // Calls the constructor of the corresponding parameter container.
  // For an actual context, which is not an empty string and not the default context
  // of this container, the name is concatinated with the context.
  const char* name=c->GetName();
  if (strcmp(name,"KickPlaneMeta")==0)
    return new HKickPlane(c->getConcatName().Data(),c->GetTitle(),
			  c->getContext());

  if (strcmp(name,"KickPlaneMDC3")==0)
    return new HKickPlane(c->getConcatName().Data(),c->GetTitle(),
			  c->getContext());

  if (strcmp(name,"KickTrackFPar")==0)
    return new HKickTrackFPar(c->getConcatName().Data(),c->GetTitle(),
			      c->getContext());
  
  if (strcmp(name,"KickPlane2Meta")==0)
    return new HKickPlane2(c->getConcatName().Data(),c->GetTitle(),
			   c->getContext());

  if (strcmp(name,"KickPlane2MDC3")==0)
    return new HKickPlane2(c->getConcatName().Data(),c->GetTitle(),
			   c->getContext()); 

  if (strcmp(name,"KickMatchParMeta")==0)
    return new HKickMatchPar(c->getConcatName().Data(),c->GetTitle(),
			     c->getContext()); 

  if (strcmp(name,"KickMatchParMDC3")==0)
    return new HKickMatchPar(c->getConcatName().Data(),c->GetTitle(),
			     c->getContext()); 
  return 0;
}
