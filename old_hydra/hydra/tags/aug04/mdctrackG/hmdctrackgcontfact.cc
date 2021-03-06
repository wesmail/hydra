//*-- AUTHOR : A. Rustamov

/////////////////////////////////////////////////////////////
//
//  HMdcTrackGContFact
//
//  Factory for the parameter containers in libmdctrackG
//
/////////////////////////////////////////////////////////////
using namespace std;
#include "hmdctrackgcontfact.h"
#include "hruntimedb.h"
#include "hparset.h"
#include "hparamlist.h"
#include "hcontfact.h"
#include "hmdctrackgcorrpar.h"
#include  "hmdctrackgfieldpar.h"
#include <iostream> 
#include <iomanip>

ClassImp(HMdcTrackGContFact)

static HMdcTrackGContFact gHMdcTrackGContFact;

HMdcTrackGContFact::HMdcTrackGContFact() {
  // Constructor (called when the library is loaded)
  fName ="MdcTrackGContFact";
  fTitle="Factory for parameter containers in libMdcTrackG";
  setAllContainers();
  HRuntimeDb::instance()->addContFactory(this);  
}

void HMdcTrackGContFact::setAllContainers() {
  // Creates the Container objects with all accepted contexts and adds them to
  // the list of containers for the MdcTrackG library.
    HContainer* corrPar= new HContainer("MdcTrackGCorrPar",
					"parameters for Splinemomentum fit","MdcTrackGCorrParProduction");

    HContainer* fieldPar= new HContainer("MdcTrackGFieldPar",
					"Fieldmap","MdcTrackGFieldParProduction");

					
    containers->Add(corrPar);
    containers->Add(fieldPar);
    
}
HParSet* HMdcTrackGContFact::createContainer(HContainer* c) {
  // Calls the constructor of the corresponding parameter container.
  // For an actual context, which is not an empty string and not the default context
  // of this container, the name is concatinated with the context.
  const char* name=c->GetName();
  if (strcmp(name,"MdcTrackGCorrPar")==0)
      return new HMdcTrackGCorrPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
      
  if (strcmp(name,"MdcTrackGFieldPar")==0)
      return new HMdcTrackGFieldPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());       
  
  return 0;
} 
