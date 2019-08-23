// @(#)$Id: hpidtrackfillerpar.cc,v 1.5 2004-02-17 09:53:29 halo Exp $
//*-- Author : Marcin Jaskula 09/10/2002
//*-- Modified: Marcin Jaskula and Jacek Otwinowski 05/02/2003
//           - iTofinoMult removed
//*-- Modified: Marcin Jaskula 14/03/2003
//           - meta overlap added

////////////////////////////////////////////////////////////////////////////////
//
// HPidTrackFillerPar
//
// HPidFrackFiller parameters for correlations
//
////////////////////////////////////////////////////////////////////////////////
using namespace std;
#include "hpidtrackfillerpar.h"
#include "hparamlist.h"
#include "iostream"

// -----------------------------------------------------------------------------

ClassImp(HPidTrackFillerPar)

// -----------------------------------------------------------------------------

HPidTrackFillerPar::HPidTrackFillerPar(const char *name, const char *title,
                                const char *context)
                                : HPidParCont(name, title, context)
{
// constructor

    strcpy(detName, "Pid");
	pLeftKickRichTheta.Set(6);
	pLeftKickRichPhi.Set(6);
	pRightKickRichTheta.Set(6);
	pRightKickRichPhi.Set(6);
	pLeftMdcRichTheta.Set(6);
	pLeftMdcRichPhi.Set(6);
    pRightMdcRichTheta.Set(6);
    pRightMdcRichPhi.Set(6);
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

    fOverlapTofMax       = 48.0f;
    fOverlapShwMin       = 42.0f;
    fOverlapDPhi         = 3.0f;
    fOverlapDThetaMinNeg = -1.4f;
    fOverlapDThetaMaxNeg = 1.0f;
    fOverlapDThetaMinPos = -0.9f;
    fOverlapDThetaMaxPos = 1.95f;

	/*
    for(Int_t i = 0; i < 6; i++)  
    { // default values
		pLeftKickRichTheta.AddAt(-2.5f,i);
		pLeftKickRichPhi.AddAt(-2.5f,i);
		pRightKickRichTheta.AddAt(2.5f,i);
		pRightKickRichPhi.AddAt(2.5f,i);

		pLeftMdcRichTheta.AddAt(-2.5f,i);
		pLeftMdcRichPhi.AddAt(-2.5f,i);
		pRightMdcRichTheta.AddAt(2.5f,i);
		pRightMdcRichPhi.AddAt(2.5f,i);
	}
	*/
		pLeftKickRichTheta.Reset();
		pLeftKickRichPhi.Reset();
		pRightKickRichTheta.Reset();
		pRightKickRichPhi.Reset();

		pLeftMdcRichTheta.Reset();
		pLeftMdcRichPhi.Reset();
		pRightMdcRichTheta.Reset();
		pRightMdcRichPhi.Reset();

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
    l->add("overlapTofMax", fOverlapTofMax, 4);
    l->add("overlapShwMin", fOverlapShwMin, 4);
    l->add("overlapDPhi", fOverlapDPhi, 4);
    l->add("overlapDThetaMinNeg", fOverlapDThetaMinNeg, 4);
    l->add("overlapDThetaMaxNeg", fOverlapDThetaMaxNeg, 4);
    l->add("overlapDThetaMinPos", fOverlapDThetaMinPos, 4);
    l->add("overlapDThetaMaxPos", fOverlapDThetaMaxPos, 4);

    l->add("leftKickRichTheta",pLeftKickRichTheta,4);
    l->add("leftMdcRichTheta",pLeftMdcRichTheta,4);
    l->add("leftKickRichPhi",pLeftKickRichPhi,4);
    l->add("leftMdcRichPhi",pLeftKickRichPhi,4);

    l->add("rightKickRichTheta",pRightKickRichTheta,4);
	l->add("rightMdcRichTheta",pRightMdcRichTheta,4);
    l->add("rightKickRichPhi",pRightKickRichPhi,4);
    l->add("rightMdcRichPhi",pRightKickRichPhi,4);
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

    if(l->fill("overlapTofMax", &fOverlapTofMax) == kFALSE)
    {
        Error("getParams",
                "No \"overlapTofMax\" parameter in the HParamList");

        return kFALSE;
    }

    if(l->fill("overlapShwMin", &fOverlapShwMin) == kFALSE)
    {
        Error("getParams",
                "No \"fOverlapShwMin\" parameter in the HParamList");

        return kFALSE;
    }

    if(l->fill("overlapDPhi", &fOverlapDPhi) == kFALSE)
    {
        Error("getParams",
                "No \"fOverlapDPhi\" parameter in the HParamList");

        return kFALSE;
    }

    if(l->fill("overlapDThetaMinNeg", &fOverlapDThetaMinNeg) == kFALSE)
    {
        Error("getParams",
                "No \"fOverlapDThetaMinNeg\" parameter in the HParamList");

        return kFALSE;
    }

    if(l->fill("overlapDThetaMaxNeg", &fOverlapDThetaMaxNeg) == kFALSE)
    {
        Error("getParams",
                "No \"fOverlapDThetaMaxNeg\" parameter in the HParamList");

        return kFALSE;
    }

    if(l->fill("overlapDThetaMinPos", &fOverlapDThetaMinPos) == kFALSE)
    {
        Error("getParams",
                "No \"fOverlapDThetaMinPos\" parameter in the HParamList");

        return kFALSE;
    }

    if(l->fill("overlapDThetaMaxPos", &fOverlapDThetaMaxPos) == kFALSE)
    {
        Error("getParams",
                "No \"fOverlapDThetaMaxPos\" parameter in the HParamList");

        return kFALSE;
    }

    if(l->fill("leftKickRichTheta",&pLeftKickRichTheta) == kFALSE)
    {
        Error("getParams",
                "No \"fLeftKickRichTheta\" parameters in the HParamList");
        return kFALSE;
    }

	if(l->fill("leftMdcRichTheta",&pLeftMdcRichTheta) == kFALSE)
    {
        Error("getParams",
                "No \"fLeftMdcRichTheta\" parameters in the HParamList");
        return kFALSE;
    }

	if(l->fill("leftKickRichPhi",&pLeftKickRichPhi) == kFALSE)
    {
        Error("getParams",
                "No \"fLeftKickRichPhi\" parameters in the HParamList");
        return kFALSE;
    }

	if(l->fill("leftMdcRichPhi",&pLeftMdcRichPhi) == kFALSE)
    {
        Error("getParams",
                "No \"fLeftMdcRichPhi\" parameters in the HParamList");
        return kFALSE;
    }

    if(l->fill("rightKickRichTheta",&pRightKickRichTheta) == kFALSE)
    {
        Error("getParams",
                "No \"pRightKickRichTheta\" parameters in the HParamList");
        return kFALSE;
    }

	if(l->fill("rightMdcRichTheta",&pRightMdcRichTheta) == kFALSE)
    {
        Error("getParams",
                "No \"pRightMdcRichTheta\" parameters in the HParamList");
        return kFALSE;
    }

	if(l->fill("rightKickRichPhi",&pRightKickRichPhi) == kFALSE)
    {
        Error("getParams",
                "No \"pRightKickRichPhi\" parameters in the HParamList");
        return kFALSE;
    }

	if(l->fill("rightMdcRichPhi",&pRightMdcRichPhi) == kFALSE)
    {
        Error("getParams",
                "No \"pRightMdcRichPhi\" parameters in the HParamList");
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

        if(R__v == 2)
        {
        Int_t iTofinoMult;

            R__b >> iTofinoMult;
        }

        if(R__v >3)
        {
            R__b >> fOverlapTofMax;
            R__b >> fOverlapShwMin;
            R__b >> fOverlapDPhi;
            R__b >> fOverlapDThetaMinNeg;
            R__b >> fOverlapDThetaMaxNeg;
            R__b >> fOverlapDThetaMinPos;
            R__b >> fOverlapDThetaMaxPos;
        }

	if(R__v >=  5)
        {
	    pLeftKickRichTheta.Streamer(R__b);
	    pRightKickRichTheta.Streamer(R__b);
	    pLeftKickRichPhi.Streamer(R__b);
	    pRightKickRichPhi.Streamer(R__b);
	    pLeftMdcRichTheta.Streamer(R__b);
	    pRightMdcRichTheta.Streamer(R__b);
	    pLeftMdcRichPhi.Streamer(R__b);
	    pRightMdcRichPhi.Streamer(R__b);
	}
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

        R__b << fOverlapTofMax;
        R__b << fOverlapShwMin;
        R__b << fOverlapDPhi;
        R__b << fOverlapDThetaMinNeg;
        R__b << fOverlapDThetaMaxNeg;
        R__b << fOverlapDThetaMinPos;
        R__b << fOverlapDThetaMaxPos;

	pLeftKickRichTheta.Streamer(R__b);
	pRightKickRichTheta.Streamer(R__b);
	pLeftKickRichPhi.Streamer(R__b);
	pRightKickRichPhi.Streamer(R__b);
	pLeftMdcRichTheta.Streamer(R__b);
	pRightMdcRichTheta.Streamer(R__b);
	pLeftMdcRichPhi.Streamer(R__b);
	pRightMdcRichPhi.Streamer(R__b);

        R__b.SetByteCount(R__c, kTRUE);
    }
}
