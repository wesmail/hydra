// @(#)$Id: hpidcontfact.cc,v 1.12 2005-04-20 10:24:48 jacek Exp $
//*-- Author :   Marcin Jaskula   09/10/2002
//*-- Modified : Jacek Otwinowski 03/03/2005
// parameters for the algorithm algStandCuts added

////////////////////////////////////////////////////////////////////////////////
//
//  HPidContFact
//
//  Factory for the parameter containers in libPid
//
////////////////////////////////////////////////////////////////////////////////

#include "hpidcontfact.h"
#include "hruntimedb.h"

#include "hpidtrackfillerpar.h"
#include "hpidreconstructorpar.h"
#include "hpidalgmomvsbetapar.h"
#include "hpidalgringpropertiespar.h"

#include "hpidalgshowerpar.h"
#include "hpidalgstandcutspar.h"
/*
#include "hpidalgmomvselosspar.h"
#include "hpidelossmpvshiftpar.h"
*/
// -----------------------------------------------------------------------------

ClassImp(HPidContFact)

// -----------------------------------------------------------------------------

static HPidContFact gHPidContFact;

// -----------------------------------------------------------------------------

HPidContFact::HPidContFact(void)
{
// Constructor (called when the library is loaded)

    fName  = "PidContFact";
    fTitle = "Factory for parameter containers in libPid";
    setAllContainers();
    HRuntimeDb::instance()->addContFactory(this);
}

// -----------------------------------------------------------------------------

void HPidContFact::setAllContainers(void)
{
// Creates the Container objects with all accepted contexts and adds them to
// the list of containers for the Pid library.
  
    containers->Add(
            new HContainer(HPIDTRACKFILLERPAR_NAME,
                           HPIDTRACKFILLERPAR_TITLE,
                           HPIDTRACKFILLERPAR_CONTEXT));
  
    containers->Add(
	    new HContainer(PIDRECONSTRUCTORPAR_NAME,
			   PIDRECONSTRUCTORPAR_TITLE,
			   PIDRECONSTRUCTORPAR_CONTEXT));
    
    HContainer* pAlgMomVsBetaPar =
                                new HContainer(PIDALGMOMVSBETAPAR_NAME,
                                                PIDALGMOMVSBETAPAR_TITLE,
                                                PIDALGMOMVSBETAPAR_CONTEXT);
    setContexts(pAlgMomVsBetaPar,PIDALGMOMVSBETAPAR_CONTEXT);
    containers->Add(pAlgMomVsBetaPar);


    HContainer* pAlgRichPar=
                            new HContainer(PIDALGRINGPROPERTIESPAR_NAME,
                                            PIDALGRINGPROPERTIESPAR_TITLE,
                                            PIDALGRINGPROPERTIESPAR_CONTEXT);

    setContexts(pAlgRichPar,PIDALGRINGPROPERTIESPAR_CONTEXT);
    containers->Add(pAlgRichPar);

 /*   
    containers->Add(
            new HContainer(PIDALGMOMVSELOSSPAR_NAME,
                           PIDALGMOMVSELOSSPAR_TITLE,
			   PIDALGMOMVSELOSSPAR_CONTEXT));
    


   
    containers->Add(
            new HContainer(PIDELOSSMPVSHIFTPAR_NAME,
                           PIDELOSSMPVSHIFTPAR_TITLE,
                           PIDELOSSMPVSHIFTPAR_CONTEXT));
*/
    HContainer* pAlgShowerPar =
                                new HContainer(PIDALGSHOWERPAR_NAME,
                                    PIDALGSHOWERPAR_TITLE,
                                    PIDALGSHOWERPAR_CONTEXT);
    setContexts(pAlgShowerPar,PIDALGSHOWERPAR_CONTEXT);
    containers->Add(pAlgShowerPar);

	HContainer *pAlgStandCuts = new HContainer(PIDALGSTANDCUTSPAR_NAME,
                           					   PIDALGSTANDCUTSPAR_TITLE,
                                               PIDALGSTANDCUTSPAR_CONTEXT);

    setContexts(pAlgStandCuts,PIDALGSTANDCUTSPAR_CONTEXT);
    containers->Add(pAlgStandCuts);
}

// -----------------------------------------------------------------------------

HParSet* HPidContFact::createContainer(HContainer* c)
{
// Calls the constructor of the corresponding parameter container.
// For an actual context, which is not an empty string and not the default
// context of this container, the name is concatinated with the context.

  const char* name = c->GetName();
  
  if(strcmp(name, HPIDTRACKFILLERPAR_NAME) == 0)
    {
      return new HPidTrackFillerPar(c->getConcatName().Data(),
				    c->GetTitle(), c->getContext());
    }
  
  if(strcmp(name, PIDRECONSTRUCTORPAR_NAME) == 0)
    {
      return new HPidReconstructorPar(c->getConcatName().Data(),
                        c->GetTitle(), c->getContext());
    }
    
    if(strcmp(name, PIDALGMOMVSBETAPAR_NAME) == 0)
    {
        return new HPidAlgMomVsBetaPar(c->getConcatName().Data(),
                        c->GetTitle(), c->getContext());
    }

    if(strcmp(name, PIDALGRINGPROPERTIESPAR_NAME) == 0)
    {
        return new HPidAlgRingPropertiesPar(c->getConcatName().Data(),
                        c->GetTitle(), c->getContext());
    }

    /*
    if(strcmp(name, PIDALGMOMVSELOSSPAR_NAME) == 0)
    {
        return new HPidAlgMomVsElossPar(c->getConcatName().Data(),
                        c->GetTitle(), c->getContext());
    }
    if(strcmp(name, PIDELOSSMPVSHIFTPAR_NAME) == 0)
    {
        return new HPidElossMPVShiftPar(c->getConcatName().Data(),
                        c->GetTitle(), c->getContext());
    }*/
    if(strcmp(name, PIDALGSHOWERPAR_NAME) == 0) {
        return new HPidAlgShowerPar(c->getConcatName().Data(),
                        c->GetTitle(), c->getContext());
    }

    if(strcmp(name, PIDALGSTANDCUTSPAR_NAME) == 0)
    {
        return new HPidAlgStandCutsPar(c->getConcatName().Data(),
                        c->GetTitle(), c->getContext());
    }

    return NULL;
}
// -----------------------------------------------------------------------------

void HPidContFact::setContexts(HContainer *pCont,const char* pDefContName){
    //register all know/possible context for pid params 
    //Possible substrings in contex name are: 
    // KICK123
    // KICK 
    // SPLINE
    // REFT
    // RUNGEKUTTA
    //See also HPidHistogramsCont::checkContext(Int_t nMomAlgInd) function.

    char cBuff[250];
    snprintf(cBuff,sizeof(cBuff),"%s%s",pDefContName,"KICK");
    pCont->addContext(cBuff);
    snprintf(cBuff,sizeof(cBuff),"%s%s",pDefContName,"KICK123");
    pCont->addContext(cBuff);
    snprintf(cBuff,sizeof(cBuff),"%s%s",pDefContName,"SPLINE");
    pCont->addContext(cBuff);
    snprintf(cBuff,sizeof(cBuff),"%s%s",pDefContName,"RUNGEKUTTA");
    pCont->addContext(cBuff);
    snprintf(cBuff,sizeof(cBuff),"%s%s",pDefContName,"REFT");
    pCont->addContext(cBuff);
}


