// @(#)$Id: hpidalgmomvsbeta.cc,v 1.7 2004-07-08 13:45:51 christ Exp $
//*-- Author : Marcin Jaskula 06/09/2002
//  Modified : Marcin Jaskula 25/02/2003
//             No calculate CL for fakes

////////////////////////////////////////////////////////////////////////////////
//
// HPidAlgMomVsBeta
//
// Test algorithm for PID from kinematic data from kick plain
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

#include "hshowerhittof.h"
#include "hshowerhittoftrack.h"

#include <hkicktrack.h>
#define MAX_FAKES_BETA 0.7
// -----------------------------------------------------------------------------

ClassImp(HPidAlgMomVsBeta)

// -----------------------------------------------------------------------------

const Float_t HPidAlgMomVsBeta::kDefMinBkg = 0.0f;
//        = TMath::Gaus(3.0) / TMath::Sqrt(2. * TMath::Pi());

const Float_t HPidAlgMomVsBeta::kDefMaxGaussDev = 3.0f;

// -----------------------------------------------------------------------------

HPidAlgMomVsBeta::HPidAlgMomVsBeta(void)
                        : HPidAlgorithm("MomVsBeta", algMomVsBeta)
{
// Default constructor

    fMinBkg      = kDefMinBkg;
    fMaxGaussDev = kDefMaxGaussDev;
}

// -----------------------------------------------------------------------------

HPidAlgMomVsBeta::HPidAlgMomVsBeta(Float_t fWeight)
                        : HPidAlgorithm("MomVsBeta", algMomVsBeta, fWeight)
{
// Default constructor with weight

    fMinBkg      = kDefMinBkg;
    fMaxGaussDev = kDefMaxGaussDev;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgMomVsBeta::init(void)
{
// Get pointer to the parameter's container

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

static  Float_t SQRT_2PI = TMath::Sqrt(2.0 * TMath::Pi());

Float_t fCharge = pKickTrack->getCharge();
Int_t   iSys    = pKickTrack->getSystem();
Float_t fMom    = pKickTrack->getP();
Float_t fTheta  = TMath::RadToDeg() * pKickTrack->getTheta();
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

        if( ! HPidPhysicsConstants::isFake(iId))
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
                    Error("calculatePDF", "Wrong sigma value for gauss distr. "
                            "%d %d %f %f = %f",
                            iId, 3 * iSys + 1, fMom, fTheta, fParams[1]);
                }

                afReturn[i] = 0.0f;

                continue;
            }

            f = (fBeta - fParams[0]) / fParams[1];

            if((fMaxGaussDev > 0.0f) && (f > fMaxGaussDev)){
                afReturn[i] = 0.0f;
            }
            else
            {
                afReturn[i] = TMath::Exp(-0.5 * f * f)
                                    / (SQRT_2PI * fParams[1]);
            }
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

            if(afReturn[i] < fMinBkg)
                afReturn[i] = fMinBkg;
        }
    }
    //Check if only fakes are non-zero
    Bool_t bIsOnlyFake=kTRUE;
    for(i = 0; i < pRec->particlesNumber(); i++){
        iId = pRec->getParticleId(i);
        if(afReturn[i]!=0 && !HPidPhysicsConstants::isFake(iId) ){
            bIsOnlyFake=kFALSE;
        } 
            
    }
    //
    if(bIsOnlyFake && iSys==0){
        //System 0 (TOFino needs a special help for mult>1
        Int_t nMult;
		if(pTrack->isSim()){
			nMult = pTrack->getShowerHitTofTrack()->getTofinoMult();
		}
		else{
			nMult = pTrack->getShowerHitTof()->getTofinoMult();
		}
        if(nMult>1){
            if (fBeta >MAX_FAKES_BETA){ // rich/shower should take decision
                for(i = 0; i < pRec->particlesNumber(); i++){
                    iId = pRec->getParticleId(i);
                    afReturn[i]=-1.0f;
                }
            }
    
        } 
        }
        
    //
    Bool_t bAllZeros=kTRUE;
    for(i = 0; i < pRec->particlesNumber(); i++){
        iId = pRec->getParticleId(i);
        if(afReturn[i]!=0) {
            bAllZeros=kFALSE;
        }
    }
    if(bAllZeros){
        for(i = 0; i < pRec->particlesNumber(); i++){
            iId = pRec->getParticleId(i);
            afReturn[i] =-1;
        }

    }

    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgMomVsBeta::calculateCL(HPidTrackCand *pTrack,
            Float_t afReturn[], Short_t &nCatIndex)
{
// Check the charge of the particle calculated by KickPlain

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

Float_t fCharge = pKickTrack->getCharge();
Int_t   iSys    = pKickTrack->getSystem();
Float_t fMom    = pKickTrack->getP();
Float_t fTheta  = TMath::RadToDeg() * pKickTrack->getTheta();
Float_t fBeta   = pKickTrack->getBeta();
Int_t   iId;
Float_t fParams[3];
Int_t   i, k;

    for(i = 0; i < pRec->particlesNumber(); i++)
    {
        iId = pRec->getParticleId(i);

        // do not calculate CL for fakes
        if(HPidPhysicsConstants::isFake(iId))
        {
            afReturn[i] = -1.0f;
            continue;
        }

        // compare the particle charge
        if(fCharge != HPidPhysicsConstants::charge(iId))
        {
            afReturn[i] = -2.0f;
            continue;
        }

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
                Error("calculateCL", "Wrong sigma value for gauss distr. "
                        "%d %d %f %f = %f",
                        iId, 3 * iSys + 1, fMom, fTheta, fParams[1]);
            }

            afReturn[i] = -2.0f;

            continue;
        }

        afReturn[i] = HPidGaussConf::getConfLevel(fParams[0],
                            fParams[1], fBeta);
    }

    return kTRUE;
}

// -----------------------------------------------------------------------------

void HPidAlgMomVsBeta::print(void) const
{
// Empty function for printing info of the algorithm

    printf("MomVsBeta algorithm\n");
    printf("fMinBkg:      %f\n", fMinBkg);
    printf("fMaxGaussDev: %f\n", fMaxGaussDev);
}
