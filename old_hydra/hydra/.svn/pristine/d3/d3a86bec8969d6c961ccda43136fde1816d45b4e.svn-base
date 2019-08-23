// @(#)$Id: hpidtrackfillerpar.cc,v 1.8 2004-07-08 13:45:52 christ Exp $
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
	for(Int_t s=0;s<6;s++){
        pLeftKickRichTheta[s].Set(MAX_KICK_RICH_BIN);
	    pLeftKickRichPhi[s].Set(MAX_KICK_RICH_BIN);
	    pRightKickRichTheta[s].Set(MAX_KICK_RICH_BIN);
	    pRightKickRichPhi[s].Set(MAX_KICK_RICH_BIN);
    }
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
    for(Int_t s = 0; s < 6; s++){
		pLeftKickRichTheta[s].Reset();
		pLeftKickRichPhi[s].Reset();
		pRightKickRichTheta[s].Reset();
		pRightKickRichPhi[s].Reset();
    }

    
		pLeftMdcRichTheta.Reset();
		pLeftMdcRichPhi.Reset();
		pRightMdcRichTheta.Reset();
		pRightMdcRichPhi.Reset();

        for(Int_t i = 0; i < 6; i++){
        pLeftMdcRichTheta.AddAt(-2.5f,i);
        pLeftMdcRichPhi.AddAt(-2.5f,i);
        pRightMdcRichTheta.AddAt(2.5f,i);
        pRightMdcRichPhi.AddAt(2.5f,i);

        }
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
    char cBuff[100];
    for(Int_t s=0;s<6;s++){
        sprintf(cBuff,"leftKickRichTheta_Sector_%d",s);
        l->add(cBuff,pLeftKickRichTheta[s],4);
        sprintf(cBuff,"leftKickRichPhi_Sector_%d",s);
        l->add(cBuff,pLeftKickRichPhi[s],4);
        sprintf(cBuff,"rightKickRichTheta_Sector_%d",s);
        l->add(cBuff,pRightKickRichTheta[s],4);
        sprintf(cBuff,"rightKickRichPhi_Sector_%d",s);
        l->add(cBuff,pRightKickRichPhi[s],4);
    }    
    l->add("leftMdcRichPhi",pLeftMdcRichPhi,4);
    l->add("leftMdcRichTheta",pLeftMdcRichTheta,4);

	l->add("rightMdcRichTheta",pRightMdcRichTheta,4);
    l->add("rightMdcRichPhi",pRightMdcRichPhi,4);
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

    for(Int_t s=0;s<6;s++){
        char cBuff[100];     
        sprintf(cBuff,"leftKickRichTheta_Sector_%d",s);       
        if(l->fill(cBuff,&pLeftKickRichTheta[s]) == kFALSE) {
            Error("getParams",
                "No \"fLeftKickRichTheta\" parameters in the HParamList");
            return kFALSE;
        }

        sprintf(cBuff,"leftKickRichPhi_Sector_%d",s);       
	    if(l->fill(cBuff,&pLeftKickRichPhi[s]) == kFALSE) {
            Error("getParams",
                "No \"fLeftKickRichPhi\" parameters in the HParamList");
            return kFALSE;
        }
        sprintf(cBuff,"rightKickRichTheta_Sector_%d",s);
        if(l->fill(cBuff,&pRightKickRichTheta[s]) == kFALSE) {
            Error("getParams",
                "No \"pRightKickRichTheta\" parameters in the HParamList");
            return kFALSE;
        }
        sprintf(cBuff,"rightKickRichPhi_Sector_%d",s);
	    if(l->fill(cBuff,&pRightKickRichPhi[s]) == kFALSE)
        {
            Error("getParams",
                "No \"pRightKickRichPhi\" parameters in the HParamList");
            return kFALSE;
        }

    }

	if(l->fill("leftMdcRichTheta",&pLeftMdcRichTheta) == kFALSE)
    {
        Error("getParams",
                "No \"fLeftMdcRichTheta\" parameters in the HParamList");
        return kFALSE;
    }


	if(l->fill("leftMdcRichPhi",&pLeftMdcRichPhi) == kFALSE)
    {
        Error("getParams",
                "No \"fLeftMdcRichPhi\" parameters in the HParamList");
        return kFALSE;
    }


	if(l->fill("rightMdcRichTheta",&pRightMdcRichTheta) == kFALSE)
    {
        Error("getParams",
                "No \"pRightMdcRichTheta\" parameters in the HParamList");
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
        if(R__v>5){
            int R__i;
            for (R__i = 0; R__i < 6; R__i++)
                pLeftKickRichTheta[R__i].Streamer(R__b);
            for (R__i = 0; R__i < 6; R__i++)
                pRightKickRichTheta[R__i].Streamer(R__b);
            for (R__i = 0; R__i < 6; R__i++)
                pLeftKickRichPhi[R__i].Streamer(R__b);
            for (R__i = 0; R__i < 6; R__i++)
                pRightKickRichPhi[R__i].Streamer(R__b);
            pLeftMdcRichTheta.Streamer(R__b);
            pRightMdcRichTheta.Streamer(R__b);
            pLeftMdcRichPhi.Streamer(R__b);
            pRightMdcRichPhi.Streamer(R__b);
            R__b >> fOverlapTofMax;
            R__b >> fOverlapShwMin;
            R__b >> fOverlapDPhi;
            R__b >> fOverlapDThetaMinNeg;
            R__b >> fOverlapDThetaMaxNeg;
            R__b >> fOverlapDThetaMinPos;
            R__b >> fOverlapDThetaMaxPos;
            

        }
        else{
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

            if(R__v ==  5){
                //Copy values to all momentum bins and sectors
                TArrayF aTemp;
                aTemp.Set(6);
                aTemp.Streamer(R__b);
                for(Int_t s=0;s<6;s++){
                    for(Int_t i=0;i<10;i++){
                        pLeftKickRichTheta[s].fArray[i] = aTemp.fArray[s];
                    }
                }
                aTemp.Streamer(R__b);
                for(Int_t s=0;s<6;s++){
                    for(Int_t i=0;i<10;i++){
                        pRightKickRichTheta[s].fArray[i] = aTemp.fArray[s];
                    }
                }
                aTemp.Streamer(R__b);
                for(Int_t s=0;s<6;s++){
                    for(Int_t i=0;i<10;i++){
                        pLeftKickRichPhi[s].fArray[i] = aTemp.fArray[s];
                    }
                }
                aTemp.Streamer(R__b);
                for(Int_t s=0;s<6;s++){
                    for(Int_t i=0;i<10;i++){
                        pRightKickRichPhi[s].fArray[i] = aTemp.fArray[s];
                    }
                }
                pLeftMdcRichTheta.Streamer(R__b);
                pRightMdcRichTheta.Streamer(R__b);
                pLeftMdcRichPhi.Streamer(R__b);
                pRightMdcRichPhi.Streamer(R__b);
            }
            else{
                //copy values from fWindowKickRichTheta etc.
                for(Int_t s=0;s<6;s++){
                    for(Int_t i=0;i<10;i++){
                        pRightKickRichTheta[s].fArray[i] = fWindowKickRichTheta;
                        pRightKickRichPhi[s].fArray[i] =  fWindowKickRichPhi;
                        pLeftKickRichTheta[s].fArray[i] = -1*fWindowKickRichTheta;
                        pLeftKickRichPhi[s].fArray[i] = -1.*fWindowKickRichPhi;
                    }
                }
            }
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
      int R__i;
      for (R__i = 0; R__i < 6; R__i++)
         pLeftKickRichTheta[R__i].Streamer(R__b);
      for (R__i = 0; R__i < 6; R__i++)
         pRightKickRichTheta[R__i].Streamer(R__b);
      for (R__i = 0; R__i < 6; R__i++)
         pLeftKickRichPhi[R__i].Streamer(R__b);
      for (R__i = 0; R__i < 6; R__i++)
         pRightKickRichPhi[R__i].Streamer(R__b);
      pLeftMdcRichTheta.Streamer(R__b);
      pRightMdcRichTheta.Streamer(R__b);
      pLeftMdcRichPhi.Streamer(R__b);
      pRightMdcRichPhi.Streamer(R__b);
      R__b << fOverlapTofMax;
      R__b << fOverlapShwMin;
      R__b << fOverlapDPhi;
      R__b << fOverlapDThetaMinNeg;
      R__b << fOverlapDThetaMaxNeg;
      R__b << fOverlapDThetaMinPos;
      R__b << fOverlapDThetaMaxPos;

        R__b.SetByteCount(R__c, kTRUE);
    }
}

