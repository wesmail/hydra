//*-- AUTHOR : Ilse Koenig
//*-- Modified last : 09/01/2002 by Ilse Koenig

/////////////////////////////////////////////////////////////
//
//  HMdcContFact
//
//  Factory for the parameter containers in libMdc 
//
/////////////////////////////////////////////////////////////

#include "hmdccontfact.h"
#include "hruntimedb.h"
#include "hmdccal2par.h"
#include "hmdccal2parsim.h"
#include "hmdccalpar.h"
#include "hmdccalparraw.h"
#include "hmdccelleff.h"
#include "hmdcdigitpar.h"
#include "hmdcevreadout.h"
#include "hmdcgeomstruct.h"
#include "hmdcgeompar.h"
#include "hmdclookupgeom.h"
#include "hmdclookupraw.h"
#include "hmdcconnectedwires.h"
#include "hmdcrawstruct.h"
#include "hmdcmboreadout.h"
#include "hmdctimecut.h"
#include <iostream.h>
#include "hmdclayergeompar.h"

ClassImp(HMdcContFact)

static HMdcContFact gHMdcContFact;

HMdcContFact::HMdcContFact() {
  // Constructor (called when the library is loaded)
  fName="MdcContFact";
  fTitle="Factory for parameter containers in libMdc";
  setAllContainers();
  HRuntimeDb::instance()->addContFactory(this);  
}

void HMdcContFact::setAllContainers() {
  // Creates the Container objects with all accepted contexts and adds them to
  // the list of containers for the MDC library.
  containers->Add(
    new HContainer("MdcCal2Par",
                   "cal2 calibration parameters for Mdc",
                   ""));
  containers->Add(
    new HContainer("MdcCal2ParSim",
                   "cal2 calibration parameters for Mdc [distance->time]",
                   ""));
  containers->Add(
    new HContainer("MdcCalPar",
                   "calibration parameters for Mdc",
                   ""));
  containers->Add(
    new HContainer("MdcCalParRaw",
                   "raw calibration parameters for Mdc",
                   ""));
  containers->Add(
    new HContainer("MdcCellEff",
                   "cell efficiency parameters for MdcDigitizer",
                   ""));
  containers->Add(
    new HContainer("MdcDigitPar",
                   "digitisation parameters for Mdc",
                   ""));
  containers->Add(
    new HContainer("MdcEvReadout",
                   "Mdc subevent readout addresses",
                   ""));
  containers->Add(
    new HContainer("MdcGeomPar",
                   "Mdc geometry parameters",
                   ""));
  containers->Add(
    new HContainer("MdcGeomStruct",
                   "Mdc parameters for geometry structure",
                   ""));
  containers->Add(
    new HContainer("MdcLayerGeomPar",
                   "geometry parameters for Mdc layers",
                   ""));
  containers->Add(
    new HContainer("MdcLookupGeom",
                   "Mdc lookup table from raw to cal1",
                   ""));
  containers->Add(
    new HContainer("MdcLookupRaw",
                   "Mdc lookup table from cal1 to raw",
                   ""));
  containers->Add(
    new HContainer("MdcConnectedWires",
                   "Mdc wires connected to readout controllers",
                   ""));
  containers->Add(
    new HContainer("MdcRawStruct",
                   "Mdc parameters for hardware structure",
                   ""));
  containers->Add(
    new HContainer("MdcMboReadout",
                   "Mdc Mbo readout addresses",
                   ""));
  containers->Add(
    new HContainer("MdcTimeCut",
                   "cut on time1, time2 & time2-time1",
                   ""));
}

HParSet* HMdcContFact::createContainer(HContainer* c) {
  // Calls the constructor of the corresponding parameter container.
  // For an actual context, which is not an empty string and not the default context
  // of this container, the name is concatinated with the context.
  const char* name=c->GetName();
  if (strcmp(name,"MdcCal2Par")==0)
    return new HMdcCal2Par(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"MdcCal2ParSim")==0)
    return new HMdcCal2ParSim(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"MdcCalPar")==0)
    return new HMdcCalPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"MdcCalParRaw")==0)
    return new HMdcCalParRaw(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"MdcCellEff")==0)
    return new HMdcCellEff(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"MdcDigitPar")==0)
    return new HMdcDigitPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"MdcEvReadout")==0)
    return new HMdcEvReadout(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"MdcGeomPar")==0)
    return new HMdcGeomPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"MdcGeomStruct")==0)
    return new HMdcGeomStruct(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"MdcLayerGeomPar")==0)
    return new HMdcLayerGeomPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"MdcLookupGeom")==0)
    return new HMdcLookupGeom(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"MdcLookupRaw")==0)
    return new HMdcLookupRaw(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"MdcConnectedWires")==0)
    return new HMdcConnectedWires(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"MdcRawStruct")==0)
    return new HMdcRawStruct(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"MdcMboReadout")==0)
    return new HMdcMboReadout(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"MdcTimeCut")==0)
    return new HMdcTimeCut(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  return 0;
} 
