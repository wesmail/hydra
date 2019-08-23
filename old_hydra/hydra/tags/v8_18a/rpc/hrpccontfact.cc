//*-- AUTHOR :  Pablo Cabanelas
//*-- Created : 11/10/2005
//*-- Modified: 11/09/2006
//*-- Modified: 18/08/2007

//_HADES_CLASS_DESCRIPTION
/////////////////////////////////////////////////////////////
//
//  HRpcContFact
//
//  Factory for the parameter containers in libRpc
//
/////////////////////////////////////////////////////////////

#include "hrpccontfact.h"
#include "hruntimedb.h"
#include "hrpcdigipar.h"
#include "hrpccalpar.h"
#include "hrpcgeomcellpar.h"
#include "hrpcthrespar.h"
#include "hrpcgeompar.h"
//#include "hrpchitfpar.h"

ClassImp(HRpcContFact)

  static HRpcContFact gHRpcContFact;

HRpcContFact::HRpcContFact(void) {
  // Constructor (called when the library is loaded)
  fName="RpcContFact";
  fTitle="Factory for parameter containers in libRpc";
  setAllContainers();
  HRuntimeDb::instance()->addContFactory(this);
}

void HRpcContFact::setAllContainers(void) {
  // Creates the Container objects with all accepted contexts and adds them to
  // the list of containers for the Rpc library.

  containers->Add(
		  new HContainer("RpcDigiPar",
				 "Digitization parameters for the Rpc Detector",
				 "RpcDigiProduction"));
  containers->Add(
		  new HContainer("RpcCalPar",
				 "Calibration parameters for the Rpc Detector",
				 "RpcCalProduction"));
  containers->Add(
		  new HContainer("RpcGeomCellPar",
				 "Geometry parameters of cells for the Rpc Detector",
				 "GeomCellProduction"));
  containers->Add(
		  new HContainer("RpcThresPar",
				 "Thresholds for the Rpc Detector",
				 "RpcThresProduction"));
  containers->Add(
		  new HContainer("RpcGeomPar",
				 "Geometry parameters of the Rpc Detector",
				 "GeomProduction"));
}

HParSet* HRpcContFact::createContainer(HContainer* c) {
  // Calls the constructor of the corresponding parameter container.
  // For an actual context, which is not an empty string and not the default context
  // of this container, the name is concatinated with the context.
  const Char_t* name=c->GetName();
  if (strcmp(name,"RpcDigiPar")==0)
    return new HRpcDigiPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"RpcCalPar")==0)
    return new HRpcCalPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"RpcGeomCellPar")==0)
    return new HRpcGeomCellPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"RpcThresPar")==0)
    return new HRpcThresPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"RpcGeomPar")==0)
    return new HRpcGeomPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());

  return 0;
}
