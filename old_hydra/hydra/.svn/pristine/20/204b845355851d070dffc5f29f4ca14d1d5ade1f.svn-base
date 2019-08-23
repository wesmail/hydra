//*-- AUTHOR : Ilse Koenig
//*-- Modified last : 14/12/2001 by Ilse Koenig

/////////////////////////////////////////////////////////////
//
//  HShowerContFact
//
//  Factory for the parameter containers in libShower 
//
/////////////////////////////////////////////////////////////

#include "hshowercontfact.h"
#include "hruntimedb.h"
#include "hshowergeometry.h"
#include "hshowerdigidetpar.h"
#include "hshowerhitfpar.h"
#include "hshowercalpar.h"
#include "hshowerhist.h"
#include "hshowerunpackparam.h"

ClassImp(HShowerContFact)

static HShowerContFact gHShowerContFact;

HShowerContFact::HShowerContFact() {
  // Constructor (called when the library is loaded)
  fName="ShowerContFact";
  fTitle="Factory for parameter containers in libShower";
  setAllContainers();
  HRuntimeDb::instance()->addContFactory(this);  
}

void HShowerContFact::setAllContainers() {
  // Creates the Container objects with all accepted contexts and adds them to
  // the list of containers for the Shower library.
  //
/*  containers->Add(
    new HContainer("ShowerCalPar",
                   "Calibration parameters for Shower",
                   ""));
*/				   

  HContainer *co = new HContainer("ShowerCalPar",
		                          "Calibration parameters for Shower",
								  "ShowerStandardCalib");
  co->addContext("ShowerJacusCalib");
  containers->Add(co); 
  

  containers->Add(
    new HContainer("ShowerDigiPar",
                   "Digitisation parameters for Shower",
                   ""));
  containers->Add(
    new HContainer("ShowerGeometry",
                   "Geometry parameters for Shower",
                   ""));
  containers->Add(
    new HContainer("ShowerHist",
                   "Histograms of offsets and slopes for Shower",
                   ""));
  containers->Add(
    new HContainer("ShowerHitFinderParams",
                   "Parameter for Shower hit finder",
                   ""));
  containers->Add(
    new HContainer("ShowerUnpackParam",
                   "Parameter for Shower unpacker",
                   ""));
}

HParSet* HShowerContFact::createContainer(HContainer* c) {
  // Calls the constructor of the corresponding parameter container.
  // For an actual context, which is not an empty string and not the default context
  // of this container, the name is concatinated with the context.
  const char* name=c->GetName();
  if (strcmp(name,"ShowerCalPar")==0)
    return new HShowerCalPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"ShowerDigiPar")==0)
    return new HShowerDigiDetPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"ShowerGeometry")==0)
    return new HShowerGeometry(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"ShowerHist")==0)
    return new HShowerHist(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"ShowerHitFinderParams")==0)
    return new HShowerHitFPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"ShowerUnpackParam")==0)
    return new HShowerUnpackParam(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  return 0;
} 
