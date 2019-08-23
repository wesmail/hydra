//*-- AUTHOR : Ilse Koenig
//*-- Modified last : 16/01/2002 by Ilse Koenig

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////
//
//  HRichContFact
//
//  Factory for the parameter containers in libRich
//
/////////////////////////////////////////////////////////////
using namespace std;
#include "hrichcontfact.h"
#include "hruntimedb.h"
#include "hrichanalysispar.h"
#include "hrichthresholdpar.h"
#include "hrichcalpar.h"
#include "hrichcorrelatorpar.h"
#include "hrichdigitisationpar.h"
#include "hrichgeometrypar.h"
#include "hrichmappingpar.h"
#include <iostream> 
#include <iomanip>

ClassImp(HRichContFact)

static HRichContFact gHRichContFact;  // instantiated when libRich is loaded

HRichContFact::HRichContFact() {
  // Constructor (called when the library is loaded)
  fName="RichContFact";
  fTitle="Factory for parameter containers in libRich";
  setAllContainers();
  HRuntimeDb::instance()->addContFactory(this);
}
//============================================================================

//---------------------------------------------------------------------------- 
void HRichContFact::setAllContainers() {
  // Creates the Container objects with all accepted contexts and adds them to
  // the list of containers for the Rich library.
  HContainer* ca=new HContainer("RichAnalysisParameters",
                                "Rich Analysis Parameters",
                                "RichAnaNormalBias");
  ca->addContext("RichAnaMinimumBias");
  ca->addContext("RichAnaHighBias");
  containers->Add(ca);

  containers->Add(
     new HContainer("RichCalPar",
                    "Rich Calibration Parameters",
                    "RichPrimaryCalibration"));

  containers->Add(
     new HContainer("RichThresholdPar",
                    "Rich Threshold Parameters",
                    "RichPrimaryCalibration"));

  HContainer* cc=new HContainer("RichCorrelatorParameters",
                                "Rich Correlator Parameters",
                                "RichCorrSharpCut");
  cc->addContext("RichCorrWideCut");
  cc->addContext("RichCorrSharpPhiWideThe");
  cc->addContext("Rich_Kick_Mode");
  containers->Add(cc);

  HContainer* cd=new HContainer("RichDigitisationParameters",
                                "Rich Digitisation Parameters",
                                //"OEM_April02"); //old default - not to be used for any beamtime analysis!
				"RichDigiProduction");
  cd->addContext("RichDigiNoiseOff");
  cd->addContext("RichDigiNoiseOn");
  //  cd->addContext("OEM_April02");
  containers->Add(cd);

  containers->Add(
     new HContainer("RichGeometryParameters",
                    "Rich Geometry Parameters",
                    ""));

  containers->Add(
     new HContainer("RichMappingParameters",
                    "Rich Mapping Parameters",
                    ""));
}
//============================================================================

//---------------------------------------------------------------------------- 
HParSet* HRichContFact::createContainer(HContainer* c) {
  // Calls the constructor of the corresponding parameter container.
  // For an actual context, which is not an empty string and not the default context
  // of this container, the name is concatinated with the context.
  const Char_t* name=c->GetName();
  if (strcmp(name,"RichAnalysisParameters")==0)
    return new HRichAnalysisPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"RichThresholdPar")==0)
     return new HRichThresholdPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"RichCalPar")==0)
     return new HRichCalPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"RichCorrelatorParameters")==0)
    return new HRichCorrelatorPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"RichDigitisationParameters")==0)
    return new HRichDigitisationPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"RichGeometryParameters")==0)
    return new HRichGeometryPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strcmp(name,"RichMappingParameters")==0)
    return new HRichMappingPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());    
  return 0;
}
