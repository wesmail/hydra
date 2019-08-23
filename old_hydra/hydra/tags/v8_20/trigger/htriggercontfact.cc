//*-- Author: Ilse Koenig
//*-- Modified: Joern Wuestenfeld 07/14/2005
//*-- Version: $Id: htriggercontfact.cc,v 1.11 2008-05-09 16:18:42 halo Exp $

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////
//
//  HTriggerContFact
//
//  Factory for the parameter containers in libTrigger
//
/////////////////////////////////////////////////////////////

#include "htriggercontfact.h"
#include "hruntimedb.h"
#include "htriggerparmulep.h"
#include "htriggerparmudilep.h"
#include "htriggerparmommap.h"
#include "htriggerparshowermap.h"
#include "htriggerparrichmap.h"
#include "hrichIPUparpattern.h"
#include "hrichIPUparthresholds.h"
#include "hrichIPUparlocmax.h"
#include "htriggersetup.h"

ClassImp(HTriggerContFact)

static HTriggerContFact gHTriggerContFact;

HTriggerContFact::HTriggerContFact() {
  // Constructor (called when the library is loaded)
  fName="TriggerContFact";
  fTitle="Factory for parameter containers in libTrigger";
  setAllContainers();
  HRuntimeDb::instance()->addContFactory(this);
}

void HTriggerContFact::setAllContainers() {
  // Creates the Container objects with all accepted contexts and adds them to
  // the list of containers for the Trigger library.
  //
  // Input parameters:
  //	none
  //
  // Output parameters:
  //	none
  //
  // Return code:
  //	none

  containers->Add(new HContainer("TriggerParMuLep",
          "Parameters for MU Lepton Emulation",
          "TriggerProduction"));


  containers->Add(new HContainer("TriggerParMuDilep",
          "Parameters for MU Dilepton Emulation",
          "TriggerProduction"));


  containers->Add(new HContainer("TriggerParMomMap",
        "Parameters for MU Momentum Mapping",
        "TriggerProduction"));


  containers->Add(new HContainer("TriggerParShowerMap",
        "Parameters for Shower Mapping",
        "TriggerProduction"));


  containers->Add(new HContainer("TriggerParRichMap",
        "Parameters for Rich Mapping",
        "TriggerProduction"));


  containers->Add(new HContainer("RichIPUParPattern",
        "Parameters for Rich IPU pattern",
        "RICHIPUEmulation"));

  containers->Add(new HContainer("TriggerSetup",
                                "Variouse trigger setup parameters",
                                "TriggerProduction"));

  HContainer *pt =new HContainer("RichIPUParThresholds",
        "Parameters for Rich IPU thresholds",
        "RICHIPUEmulation");
  pt->addContext("RICHIPUconservativethr");
  containers->Add(pt);

//   containers->Add(new HContainer("RichIPUParThresholds2",
//  				 "Parameters for Rich IPU thresholds",
//  				 "RICHIPUEmulation"));


  HContainer *lm =new HContainer("RichIPUParLocMax",
        "Parameters for Rich IPU local maximum",
        "RICHIPUEmulation");
  lm->addContext("RICHIPUconservativethr");
  containers->Add(lm);
}

HParSet* HTriggerContFact::createContainer(HContainer* c) {
  // Calls the constructor of the corresponding parameter container.
  // For an actual context, which is not an empty string and not the default context
  // of this container, the name is concatinated with the context.
  //
  // Input Parameters:
  //	HContainer* c
  //		Container object that defines which container sould be created.
  //
  // Output parameters:
  //	none
  //
  // Return code:
  //		Function returns pointer to created parameter container specified by input parameter.

  const Char_t* name=c->GetName();
  if (strncmp(name,"TriggerParMuLep",strlen("TriggerParMuLep"))==0)
    return new HTriggerParMuLep(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strncmp(name,"TriggerParMuDilep",strlen("TriggerParMuDilep"))==0)
    return new HTriggerParMuDilep(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strncmp(name,"TriggerParMomMap",strlen("TriggerParMomMap"))==0)
    return new HTriggerParMomMap(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strncmp(name,"TriggerParShowerMap",strlen("TriggerParShowerMap"))==0)
    return new HTriggerParShowerMap(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strncmp(name,"TriggerParRichMap",strlen("TriggerParRichMap"))==0)
    return new HTriggerParRichMap(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strncmp(name,"RichIPUParPattern",strlen("RichIPUParPattern"))==0)
    return new HRichIPUParPattern(c->getConcatName().Data(),c->GetTitle(),c->getContext());    
  if (strncmp(name,"RichIPUParThresholds",strlen("RichIPUParThresholds"))==0)
    return new HRichIPUParThresholds(c->getConcatName().Data(),c->GetTitle(),c->getContext()); 
//   if (strcmp(name,"RichIPUParThresholds2")==0)
//     return new HRichIPUParThresholds(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strncmp(name,"RichIPUParLocMax",strlen("RichIPUParLocMax"))==0)
    return new HRichIPUParLocMax(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  if (strncmp(name,"TriggerSetup",strlen("TriggerSetup"))==0)
    return new HTriggerSetup(c->getConcatName().Data(),c->GetTitle(),c->getContext());
return 0;
}
