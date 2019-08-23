// @(#)$Id: hpidalgmomvsbeta.cc,v 1.1 2002-10-30 10:55:33 halo Exp $
//*-- Author : Marcin Jaskula 06/09/2002

////////////////////////////////////////////////////////////////////////////////
//
// HPidAlgMomVsBeta
//
// Test algorithm for PID from kinematic data from kick plain
// This the first vestion checks only the charge of the particle
//
////////////////////////////////////////////////////////////////////////////////

#pragma implementation

#include "hpidalgmomvsbeta.h"
#include "hpidtrackcand.h"
#include "hpidreconstructor.h"

#include "hades.h"
#include "hruntimedb.h"

#include "hpidalgmomvsbetapar.h"
#include "hpidphysicsconstants.h"

#include "hpidgaussconf.h"

#include <hkicktrack.h>

// -----------------------------------------------------------------------------

ClassImp(HPidAlgMomVsBeta)

// -----------------------------------------------------------------------------

HPidAlgMomVsBeta::HPidAlgMomVsBeta(void)
                        : HPidAlgorithm("MomVsBeta", algMomVsBeta)
{
// Default constructor
}

// -----------------------------------------------------------------------------

HPidAlgMomVsBeta::HPidAlgMomVsBeta(Float_t fWeight)
                        : HPidAlgorithm("MomVsBeta", algMomVsBeta, fWeight)
{
// Default constructor with weight
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgMomVsBeta::init(void)
{
// Dummy method

    if((pParams = (HPidAlgMomVsBetaPar *)gHades->getRuntimeDb()
                        ->getContainer(PIDALGMOMVSBETAPAR_NAME)) == NULL)
    {
        Error("init", "Cannot get parameters: %s", PIDALGMOMVSBETAPAR_NAME);
        return kFALSE;
    }

    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgMomVsBeta::reinit(void)
{
// Dummy method

    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgMomVsBeta::finalize(void)
{
// Dummy method

    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgMomVsBeta::calculatePDF(HPidTrackCand *pTrack,
            Float_t afReturn[], Short_t &nCatIndex)
{
// Calculate PDF from data stroed in histograms in HPidAlgMomVsBetaPar
// afReturn MUST NOT BE NULL

HKickTrack *pKickTrack;

    if(pTrack->getKickTrackId() < 0)
    {
        // not a full track
        return kTRUE;
    }

    if((pKickTrack = pTrack->getKickTrack()) == NULL)
    {
        Error("calculatePDF", "Cannot get HKickTrack for the track: %d",
                            pTrack->getKickTrackId());

        return kFALSE;
    }

static  Float_t R2D      = 180.0 / TMath::Pi();
static  Float_t SQRT_2PI = TMath::Sqrt(2.0 * TMath::Pi());

Float_t fCharge = pKickTrack->getCharge();
Int_t   iSys    = pKickTrack->getSystem();
Float_t fMom    = pKickTrack->getP();
Float_t fTheta  = R2D * pKickTrack->getTheta();
Float_t fBeta   = pKickTrack->getBeta();
Float_t f;
Int_t   iId;
Float_t fParams[3];
Int_t   i, k;

    for(i = 0; i < pRec->particlesNumber(); i++)
    {
        iId = pRec->getParticleId(i);

        // compare the particle charge
        if(fCharge != HPidPhysicsConstants::charge(iId))
        {
            afReturn[i] = 0.0f;
            continue;
        }

        if(iId < ARTIFICAL_PID_OFFSET)
        {
            // gauss fit
            for(k = 0; k < 2; k++)
            {
                fParams[k] = pParams->getParameterValue(iId, 3 * iSys + k,
                                fMom, fTheta);
            }

            if(fParams[1] <= 0.0f)
            {
                if(fParams[1] < 0.0f)
                {
                    Error("execute", "Wrong sigma value for gauss distr. "
                            "%d %d %f %f = %f",
                            iId, 3 * iSys + 1, fMom, fTheta, fParams[1]);
                }

                afReturn[i] = 0.0f;

                continue;
            }

            f = (fBeta - fParams[0]) / fParams[1];

            afReturn[i] = TMath::Exp(-0.5 * f * f) / (SQRT_2PI * fParams[1]);
        }
        else
        {
            // pol2 fit for the background functions
            for(k = 0; k < 3; k++)
            {
                fParams[k] = pParams->getParameterValue(iId, 3 * iSys + k,
                                fMom, fTheta);
            }

            afReturn[i] = fParams[0] + fParams[1] * fBeta
                        + fParams[2] * fBeta * fBeta;

            if(afReturn[i] < 0.0f)
                afReturn[i] = 0.0f;
        }
    }

    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgMomVsBeta::calculateCL(HPidTrackCand *pTrack,
            Float_t afReturn[], Short_t &nCatIndex)
{
// Check the charge of the particle calculated by KickPlain
// afReturn MUST NOT BE NULL

HKickTrack *pKickTrack;

    if(pTrack->getKickTrackId() < 0)
    {
        // not a full track
        return kTRUE;
    }

    if((pKickTrack = pTrack->getKickTrack()) == NULL)
    {
        Error("execute", "Cannot get HKickTrack for the track: %d",
                            pTrack->getKickTrackId());

        return kFALSE;
    }

static  Float_t R2D      = 180.0 / TMath::Pi();
static  Float_t SQRT_2PI = TMath::Sqrt(2.0 * TMath::Pi());

Float_t fCharge = pKickTrack->getCharge();
Int_t   iSys    = pKickTrack->getSystem();
Float_t fMom    = pKickTrack->getP();
Float_t fTheta  = R2D * pKickTrack->getTheta();
Float_t fBeta   = pKickTrack->getBeta();
Int_t   iId;
Float_t fParams[3];
Int_t   i, k;
Double_t dD, dX1, dX2;

    for(i = 0; i < pRec->particlesNumber(); i++)
    {
        iId = pRec->getParticleId(i);

        // compare the particle charge
        if(fCharge != HPidPhysicsConstants::charge(iId))
        {
            afReturn[i] = -2.0f;
            continue;
        }

        if(iId < ARTIFICAL_PID_OFFSET)
        {
            // gauss fit
            for(k = 0; k < 2; k++)
            {
                fParams[k] = pParams->getParameterValue(iId, 3 * iSys + k,
                                fMom, fTheta);
            }

            if(fParams[1] <= 0.0f)
            {
                if(fParams[1] < 0.0f)
                {
                    Error("execute", "Wrong sigma value for gauss distr. "
                            "%d %d %f %f = %f",
                            iId, 3 * iSys + 1, fMom, fTheta, fParams[1]);
                }

                afReturn[i] = -2.0f;

                continue;
            }

            afReturn[i] = HPidGaussConf::getConfLevel(fParams[0],
                                fParams[1], fBeta);
        }
        else
        {
            // pol2 fit for the background functions
            for(k = 0; k < 3; k++)
            {
                fParams[k] = pParams->getParameterValue(iId, 3 * iSys + k,
                                fMom, fTheta);
            }

            // checking assumption that fParams[2] < 0.0
            if((fParams[2] >= 0.0f)
                    || ((dD = fParams[1] * fParams[1]
                                - 4.0 * fParams[0] * fParams[2]) <= 0.0))
            {
                afReturn[i] = -3.0f;
                continue;
            }

            dD = TMath::Sqrt(dD);
            dX1 = (-fParams[1] - dD) / (2.0 * fParams[2]);
            dX2 = (-fParams[1] + dD) / (2.0 * fParams[2]);

            if(dX2 < dX1)
            {
                dD = dX2; dX2 = dX1; dX1 = dD;
            }

            // out of range of
            if((fBeta < dX1) || (fBeta > dX2))
            {
                afReturn[i] = 1.0f;
                continue;
            }

            dD = fParams[0] * (fBeta - dX1)
                + fParams[1] * (fBeta * fBeta - dX1 * dX1) / 2.0
                + fParams[2] * (fBeta * fBeta * fBeta - dX1 * dX1 * dX1) / 3.0;

            // assumption that the integral == 1
            if(dD > 0.5)
                dD = 1.0 - dD;

            // symetric distr.
            afReturn[i] = 1.0 - 2.0 * dD;
        }
    }

    return kTRUE;
}

// -----------------------------------------------------------------------------

void HPidAlgMomVsBeta::print(void) const
{
// Empty function for printing info of the algorithm

    printf("\tMomVsBeta algorithm\n");
}
