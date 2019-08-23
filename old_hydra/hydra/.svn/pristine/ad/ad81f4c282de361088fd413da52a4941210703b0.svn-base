// @(#)$Id: hpidtrackfillerpar.cc,v 1.1 2002-10-30 10:55:33 halo Exp $
//*-- Author : Marcin Jaskula 09/10/2002

////////////////////////////////////////////////////////////////////////////////
//
// HPidTrackFillerPar
//
// HPidFrackFiller parameters for correlations
//
////////////////////////////////////////////////////////////////////////////////

#include "hpidtrackfillerpar.h"
#include "hparamlist.h"

// -----------------------------------------------------------------------------

ClassImp(HPidTrackFillerPar)

// -----------------------------------------------------------------------------

HPidTrackFillerPar::HPidTrackFillerPar(const char *name, const char *title,
                                const char *context)
                                : HPidParCont(name, title, context)
{
// constructor

    strcpy(detName, "Pid");
    clear();
}

// -----------------------------------------------------------------------------

HPidTrackFillerPar::~HPidTrackFillerPar(void)
{
// empty destructor
}

// -----------------------------------------------------------------------------

void HPidTrackFillerPar::clear(void)
{
// set all parameters to default values

    fWindowKickRichTheta = 2.5f;
    fWindowKickRichPhi   = 1.8f;
    fWindowMdcRichTheta  = 2.5f;
    fWindowMdcRichPhi    = 2.7f;
}

// -----------------------------------------------------------------------------

void HPidTrackFillerPar::putParams(HParamList* l)
{
// Put params into HParamList

    if(l == NULL)
    {
        Error("putParams", "l is NULL");
        return;
    }

    l->add("windowKickRichTheta", fWindowKickRichTheta, 4);
    l->add("windowKickRichPhi", fWindowKickRichPhi, 4);
    l->add("windowMdcRichTheta", fWindowMdcRichTheta, 4);
    l->add("windowMdcRichPhi", fWindowMdcRichPhi, 4);
}

// -----------------------------------------------------------------------------

Bool_t HPidTrackFillerPar::getParams(HParamList* l)
{
// Get params from HParamList

    clear();

    if(l == NULL)
    {
        Error("getParams", "l is NULL");
        return kFALSE;
    }

    if(l->fill("windowKickRichTheta", &fWindowKickRichTheta) == kFALSE)
    {
        Error("getParams",
                "No \"windowKickRichTheta\" parameter in the HParamList");

        return kFALSE;
    }

    if(l->fill("windowKickRichPhi", &fWindowKickRichPhi) == kFALSE)
    {
        Error("getParams",
                "No \"windowKickRichPhi\" parameter in the HParamList");

        return kFALSE;
    }

    if(l->fill("windowMdcRichTheta", &fWindowMdcRichTheta) == kFALSE)
    {
        Error("getParams",
                "No \"windowMdcRichTheta\" parameter in the HParamList");

        return kFALSE;
    }

    if(l->fill("windowMdcRichPhi", &fWindowMdcRichPhi) == kFALSE)
    {
        Error("getParams",
                "No \"windowMdcRichPhi\" parameter in the HParamList");

        return kFALSE;
    }

    return kTRUE;
}
