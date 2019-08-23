//*-- AUTHOR : Ilse Koenig
//*-- Modified last : 17/01/2002 by Ilse Koenig

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////
//
//  HMdcTrackSContFact
//
//  Factory for the parameter containers in libMdcTrackS
//
/////////////////////////////////////////////////////////////
using namespace std;
#include "hmdctrackscontfact.h"
#include "hruntimedb.h"
#include "hmdchitfpar.h"
#include "hmdcleverarmgeometry.h"
#include "hmdcmodulegeometry.h"
#include "hmdcsegmentfpar.h"
#include "hmdctargetgeometry.h"
#include <iostream> 
#include <iomanip>

ClassImp(HMdcTrackSContFact)

static HMdcTrackSContFact gHMdcTrackSContFact;

HMdcTrackSContFact::HMdcTrackSContFact() {
  // Constructor (called when the library is loaded)
  fName="MdcTrackSContFact";
  fTitle="Factory for parameter containers in libMdcTrackS";
  setAllContainers();
  HRuntimeDb::instance()->addContFactory(this);
}

void HMdcTrackSContFact::setAllContainers() {
  // Creates the Container objects with all accepted contexts and adds them to
  // the list of containers for the MdcTrackS library.
  containers->Add(
    new HContainer("MdcHitFPar",
                   "parameters for Mdc hit finder HMdcHitF",
                   ""));
  containers->Add(
    new HContainer("MdcLeverArmGeometry",
                   "geometry container for the mdc reconstruction in one lever arm",
                   ""));
  containers->Add(
    new HContainer("MdcModuleGeometry",
                   "geometrical parameters for one MDC module",
                   ""));
  containers->Add(
    new HContainer("MdcSegmentFPar",
                   "parameter container for segment finding and fitting",
                   ""));
  containers->Add(
    new HContainer("MdcTargetGeometry",
                   "target geometry for mdc reconstruction",
                   ""));
}

HParSet* HMdcTrackSContFact::createContainer(HContainer* c) {
  // Calls the constructor of the corresponding parameter container.
  // For an actual context, which is not an empty string and not the default context
  // of this container, the name is concatinated with the context.
  const char* name=c->GetName();
  if (strcmp(name,"MdcHitFPar")==0)
    return new HMdcHitFPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());

  if (strcmp(name,"MdcLeverArmGeometry")==0)
    return new HMdcLeverArmGeometry(c->getConcatName().Data(),c->GetTitle(),c->getContext());

  if (strcmp(name,"MdcModuleGeometry")==0)
    return new HMdcModuleGeometry(c->getConcatName().Data(),c->GetTitle(),c->getContext());

  if (strcmp(name,"MdcSegmentFPar")==0)
    return new HMdcSegmentFPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());

  if (strcmp(name,"MdcTargetGeometry")==0)
    return new HMdcTargetGeometry(c->getConcatName().Data(),c->GetTitle(),c->getContext());

  return 0;
}
