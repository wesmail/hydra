// @(#)$Id: hpidhitdata.cc,v 1.1 2003-03-04 17:15:45 jaskula Exp $
//*-- Author : Marcin Jaskula 02/03/2003

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

#include <TError.h>

// -----------------------------------------------------------------------------

ClassImp(HPidHitData)

// -----------------------------------------------------------------------------

void HPidHitData::reset(void)
{
// Set all memebers to 0

    memset(((char *) this) + sizeof(TObject),
            0, sizeof(*this) - sizeof(TObject));
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
    }
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
}
