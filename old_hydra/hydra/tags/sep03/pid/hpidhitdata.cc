// @(#)$Id: hpidhitdata.cc,v 1.6 2003-07-22 13:05:33 christ Exp $
//*-- Author : Marcin Jaskula 02/03/2003
//  Modified : Marcin Jaskula 14/03/2003
//              tof and mdc variables added
//  Modified : Marcin Jaskula 16/04/2003
//              flags added

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidHitData                                                                //
//                                                                            //
// Class for storing all intersting data from hit objects matched in          //
// HPidTrackCand. Dedicated to store data for extensions of HPidParticle.     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hpidhitdata.h"
#include "hpidtrackcand.h"

#include "hrichhit.h"
#include "hkicktrack.h"
#include "hpidmdcfunc.h"

#include <TError.h>

// -----------------------------------------------------------------------------

ClassImp(HPidHitData)

// -----------------------------------------------------------------------------

void HPidHitData::reset(void)
{
// Set all memebers to 0

    memset(((char *) this) + sizeof(TObject),
            0, sizeof(*this) - sizeof(TObject));

    clearFlags();
}

// -----------------------------------------------------------------------------

void HPidHitData::fill(HPidTrackCand *pTrack)
{
    if(pTrack == NULL)
    {
        ::Error("HPidHitData::fill", "pTack==NULL");
        return;
    }

HRichHit *pRichHit;

    if((pRichHit = pTrack->getRichHit()) != NULL)
    {
        nRingPadNr     = pRichHit->getRingPadNr();
        fRingCentroid  = pRichHit->getCentroid();
        fRichTheta     = pRichHit->getTheta();
        fRichPhi       = pRichHit->getPhi();
        nRingPatMat    = pRichHit->getRingPatMat();
        nRingHouTra    = pRichHit->getRingHouTra();
        nRingAmplitude = pRichHit->getRingAmplitude();
        nRingLocalMax4 = pRichHit->getRingLocalMax4();
        //fRingFitVar    = pRichHit->;
        //fRingFitRad    = pRichHit->;
        //fRingFitTheta  = pRichHit->;
        //fRingFitPhi    = pRichHit->;

        setFlagRICH();
    }

HKickTrack *pKickTrack;

    if((pKickTrack = pTrack->getKickTrack()) != NULL)
    {
        fMetaEloss = pKickTrack->getMetaEloss();
        setFlagTOF();
    }

    iMdcClsSize0  = HPidMdcFunc::getMdcClsSize(pTrack,  0);
    iMdcNWires0   = HPidMdcFunc::getMdcNWires(pTrack,   0);
    iMdcLevelCls0 = HPidMdcFunc::getMdcLevelCls(pTrack, 0);
    iMdcClsSize1  = HPidMdcFunc::getMdcClsSize(pTrack,  1);
    iMdcNWires1   = HPidMdcFunc::getMdcNWires(pTrack,   1);
    iMdcLevelCls1 = HPidMdcFunc::getMdcLevelCls(pTrack, 1);
    iMdcChiSquare = HPidMdcFunc::getMdcChiSquare(pTrack  );

    if(pTrack->getMdcSeg() != NULL)
        setFlagMDC();
}

// -----------------------------------------------------------------------------

void HPidHitData::print(void) const
{
    printf("HPidHitData\n");
    printf("nRingPadNr: %d\n", nRingPadNr);
    printf("fRingCentroid: %f\n", fRingCentroid);
    printf("fRichTheta: %f\n", fRichTheta);
    printf("fRichPhi: %f\n", fRichPhi);
    printf("nRingPatMat: %d\n", nRingPatMat);
    printf("nRingHouTra: %d\n", nRingHouTra);
    printf("nRingAmplitude: %d\n", nRingAmplitude);
    printf("nRingLocalMax4: %d\n", nRingLocalMax4);
    printf("fRingFitVar: %f\n", fRingFitVar);
    printf("fRingFitRad: %f\n", fRingFitRad);
    printf("fRingFitTheta: %f\n", fRingFitTheta);
    printf("fRingFitPhi: %f\n", fRingFitPhi);

    printf("fMetaEloss: %f\n", fMetaEloss);

    printf("iMdcClsSize0: %i\n", iMdcClsSize0);
    printf("iMdcNWires0: %i\n", iMdcNWires0);
    printf("iMdcLevelCls0: %i\n", iMdcLevelCls0);
    printf("iMdcClsSize1: %i\n", iMdcClsSize1);
    printf("iMdcNWires1: %i\n", iMdcNWires1);
    printf("iMdcLevelCls1: %i\n", iMdcLevelCls1);
}

// -----------------------------------------------------------------------------

void HPidHitData::Streamer(TBuffer &R__b)
{
// Stream an object of class HPidHitData.

UInt_t R__s, R__c;

    if (R__b.IsReading())
    {
        Version_t R__v = R__b.ReadVersion(&R__s, &R__c);

        TObject::Streamer(R__b);
        R__b >> nRingPadNr;
        R__b >> fRingCentroid;
        R__b >> fRichTheta;
        R__b >> fRichPhi;
        R__b >> nRingPatMat;
        R__b >> nRingHouTra;
        R__b >> nRingAmplitude;
        R__b >> nRingLocalMax4;
        R__b >> fRingFitVar;
        R__b >> fRingFitRad;
        R__b >> fRingFitTheta;
        R__b >> fRingFitPhi;

        if(R__v > 1)
        {
            R__b >> fMetaEloss;

            R__b >> iMdcClsSize0;
            R__b >> iMdcNWires0;
            R__b >> iMdcLevelCls0;
            R__b >> iMdcClsSize1;
            R__b >> iMdcNWires1;
            R__b >> iMdcLevelCls1;
	    R__b >> iMdcChiSquare;
        }

        R__b.CheckByteCount(R__s, R__c, HPidHitData::IsA());
    }
    else
    {
        R__c = R__b.WriteVersion(HPidHitData::IsA(), kTRUE);
        TObject::Streamer(R__b);

        R__b << nRingPadNr;
        R__b << fRingCentroid;
        R__b << fRichTheta;
        R__b << fRichPhi;
        R__b << nRingPatMat;
        R__b << nRingHouTra;
        R__b << nRingAmplitude;
        R__b << nRingLocalMax4;
        R__b << fRingFitVar;
        R__b << fRingFitRad;
        R__b << fRingFitTheta;
        R__b << fRingFitPhi;

        R__b << fMetaEloss;

        R__b << iMdcClsSize0;
        R__b << iMdcNWires0;
        R__b << iMdcLevelCls0;
        R__b << iMdcClsSize1;
        R__b << iMdcNWires1;
        R__b << iMdcLevelCls1;
	R__b << iMdcChiSquare;
        R__b.SetByteCount(R__c, kTRUE);
    }
}

