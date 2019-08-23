//*-- AUTHOR : Ilse Koenig
//*-- Modified last : 21/01/2002 by Ilse Koenig
//*-- Modified : P.Cabanelas, 15/12/2003; containers for nov02 params for two targets

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////
//
//  HKickContFact
//
//  Factory for the parameter containers in libKick
//
/////////////////////////////////////////////////////////////
using namespace std;
#include "hkickcontfact.h"
#include "hruntimedb.h"
#include "hkicktrackfpar.h"
#include "hkickplane.h"
#include "hkickplane2.h"
#include "hkickmatchpar.h"
#include "hrtmatchingpar.h"
#include "hrtfunctional.h"
#include <iostream> 
#include <iomanip>

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
  HContainer *cc = new HContainer("KickPlaneMeta",
				 "Parameter container for the kick plane",
				 "KickPlaneProduction");
  containers->Add(cc);
  
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
  /*containers->Add(new HContainer("KickPlane2MetaT1",
				 "Parameter container for kickplane v2",
				 "KickPlane2Production"));*/
  containers->Add(new HContainer("KickPlane2MetaT2",
				 "Parameter container for kickplane v2",
				 "KickPlane2Production"));

  containers->Add(new HContainer("KickPlane2MDC3",
				 "Parameter container for kickplane v2",
				 "KickPlane2Production"));
  /*containers->Add(new HContainer("KickPlane2MDC3T1",
				 "Parameter container for kickplane v2",
				 "KickPlane2Production"));*/
  containers->Add(new HContainer("KickPlane2MDC3T2",
				 "Parameter container for kickplane v2",
				 "KickPlane2Production"));

  containers->Add(new HContainer("KickMatchParMeta",
				 "Matching parameters for kickplane v2",
				 "KickMatchProduction"));
  /*containers->Add(new HContainer("KickMatchParMetaT1",
				 "Matching parameters for kickplane v2",
				 "KickMatchProduction"));*/
  containers->Add(new HContainer("KickMatchParMetaT2",
				 "Matching parameters for kickplane v2",
				 "KickMatchProduction"));

  containers->Add(new HContainer("KickMatchParMDC3",
				 "Matching parameters for kickplane v2",
				 "KickMatchProduction"));
  /*containers->Add(new HContainer("KickMatchParMDC3T1",
				 "Matching parameters for kickplane v2",
				 "KickMatchProduction"));*/
  containers->Add(new HContainer("KickMatchParMDC3T2",
				 "Matching parameters for kickplane v2",
				 "KickMatchProduction"));

  containers->Add(new HContainer("RtMatchingParMDC3",
				 "Matching parameters for MDC12 - MDC3",
				 "RtMatchingParMDC3Production"));
  
  containers->Add(new HContainer("RtMatchingParMeta",
				 "Matching parameters for MDC - Meta",
				 "RtMatchingParMetaProduction"));

  containers->Add(new HContainer("RtData",
				 "Reference trajectories table",
				 "RtProduction"));

  containers->Add(new HContainer("RtSavGolPar",
				 "Savitzky-Golay params",
				 "RtProduction"));
}

HParSet* HKickContFact::createContainer(HContainer* c) {
  // Calls the constructor of the corresponding parameter container.
  // For an actual context, which is not an empty string and not the default context
  // of this container, the name is concatinated with the context.
  const Char_t* name=c->GetName();
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
  /*if (strcmp(name,"KickPlane2MetaT1")==0)
    return new HKickPlane2(c->getConcatName().Data(),c->GetTitle(),
			   c->getContext());*/
  if (strcmp(name,"KickPlane2MetaT2")==0)
    return new HKickPlane2(c->getConcatName().Data(),c->GetTitle(),
			   c->getContext());

  if (strcmp(name,"KickPlane2MDC3")==0)
    return new HKickPlane2(c->getConcatName().Data(),c->GetTitle(),
			   c->getContext()); 
  /*if (strcmp(name,"KickPlane2MDC3T1")==0)
    return new HKickPlane2(c->getConcatName().Data(),c->GetTitle(),
			   c->getContext()); */
  if (strcmp(name,"KickPlane2MDC3T2")==0)
    return new HKickPlane2(c->getConcatName().Data(),c->GetTitle(),
			   c->getContext()); 

  if (strcmp(name,"KickMatchParMeta")==0)
    return new HKickMatchPar(c->getConcatName().Data(),c->GetTitle(),
			     c->getContext()); 
  /*if (strcmp(name,"KickMatchParMetaT1")==0)
    return new HKickMatchPar(c->getConcatName().Data(),c->GetTitle(),
			     c->getContext());*/ 
  if (strcmp(name,"KickMatchParMetaT2")==0)
    return new HKickMatchPar(c->getConcatName().Data(),c->GetTitle(),
			     c->getContext()); 

  if (strcmp(name,"KickMatchParMDC3")==0)
    return new HKickMatchPar(c->getConcatName().Data(),c->GetTitle(),
			     c->getContext()); 
  /*if (strcmp(name,"KickMatchParMDC3T1")==0)
    return new HKickMatchPar(c->getConcatName().Data(),c->GetTitle(),
			     c->getContext());*/ 
  if (strcmp(name,"KickMatchParMDC3T2")==0)
    return new HKickMatchPar(c->getConcatName().Data(),c->GetTitle(),
			     c->getContext()); 

  if (strcmp(name,"RtMatchingParMDC3")==0)
    return new HRtMatchingPar(c->getConcatName().Data(),c->GetTitle(),
		    c->getContext()); 

  if (strcmp(name,"RtMatchingParMeta")==0)
    return new HRtMatchingPar(c->getConcatName().Data(),c->GetTitle(),
		    c->getContext()); 
  
  if (strcmp(name,"RtData")==0)
    return new HRtData(c->getConcatName().Data(),c->GetTitle(),
		    c->getContext()); 
  
  if (strcmp(name,"RtSavGolPar")==0)
    return new HRtSavGolPar(c->getConcatName().Data(),c->GetTitle(),
		    c->getContext()); 
  return 0;
}
