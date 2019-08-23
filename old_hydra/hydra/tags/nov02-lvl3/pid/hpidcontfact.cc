// @(#)$Id: hpidcontfact.cc,v 1.2 2003-03-14 15:21:17 jaskula Exp $
//*-- Author : Marcin Jaskula 09/10/2002

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
#include "hpidalgmomvselosspar.h"
#include "hpidelossmpvshiftpar.h"

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

    containers->Add(
            new HContainer(PIDALGMOMVSBETAPAR_NAME,
                           PIDALGMOMVSBETAPAR_TITLE,
                           PIDALGMOMVSBETAPAR_CONTEXT));

    containers->Add(
            new HContainer(PIDALGMOMVSELOSSPAR_NAME,
                           PIDALGMOMVSELOSSPAR_TITLE,
                           PIDALGMOMVSELOSSPAR_CONTEXT));

    containers->Add(
            new HContainer(PIDELOSSMPVSHIFTPAR_NAME,
                           PIDELOSSMPVSHIFTPAR_TITLE,
                           PIDELOSSMPVSHIFTPAR_CONTEXT));
}

// -----------------------------------------------------------------------------

HParSet* HPidContFact::createContainer(HContainer* c)
{
// Calls the constructor of the corresponding parameter container.
// For an actual context, which is not an empty string and not the default
// context of this container, the name is concatinated with the context.

const char* name = c->GetName();

    if(strcmp(name, "PidTrackFillerPar") == 0)
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

    if(strcmp(name, PIDALGMOMVSELOSSPAR_NAME) == 0)
    {
        return new HPidAlgMomVsElossPar(c->getConcatName().Data(),
                        c->GetTitle(), c->getContext());
    }

    if(strcmp(name, PIDELOSSMPVSHIFTPAR_NAME) == 0)
    {
        return new HPidElossMPVShiftPar(c->getConcatName().Data(),
                        c->GetTitle(), c->getContext());
    }

    return NULL;
}
