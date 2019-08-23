// @(#)$Id: hpidtrackfillerpar.cc,v 1.2 2002-11-04 02:53:59 jaskula Exp $
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
    iTofinoMult          = 0;
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

    l->add("tofinoMult", iTofinoMult);
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

    if(l->fill("tofinoMult", &iTofinoMult) == kFALSE)
    {
        Error("getParams", "No \"tofinomult\" parameter in the HParamList");

        return kFALSE;
    }

    return kTRUE;
}

// -----------------------------------------------------------------------------

void HPidTrackFillerPar::Streamer(TBuffer &R__b)
{
// Stream an object of class HPidTrackFillerPar.

UInt_t R__s, R__c;

    if(R__b.IsReading())
    {
        Version_t R__v = R__b.ReadVersion(&R__s, &R__c);

        HPidParCont::Streamer(R__b);
        R__b >> fWindowKickRichTheta;
        R__b >> fWindowKickRichPhi;
        R__b >> fWindowMdcRichTheta;
        R__b >> fWindowMdcRichPhi;

        if(R__v < 2)
            iTofinoMult = 0;
        else
            R__b >> iTofinoMult;

        R__b.CheckByteCount(R__s, R__c, HPidTrackFillerPar::IsA());
    }
    else
    {
        R__c = R__b.WriteVersion(HPidTrackFillerPar::IsA(), kTRUE);

        HPidParCont::Streamer(R__b);
        R__b << fWindowKickRichTheta;
        R__b << fWindowKickRichPhi;
        R__b << fWindowMdcRichTheta;
        R__b << fWindowMdcRichPhi;
        R__b << iTofinoMult;
        R__b.SetByteCount(R__c, kTRUE);
    }
}
