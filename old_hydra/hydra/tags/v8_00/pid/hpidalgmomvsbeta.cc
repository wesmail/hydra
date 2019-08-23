// @(#)$Id: hpidalgmomvsbeta.cc,v 1.16 2006-08-16 16:13:34 halo Exp $
//*-- Author : Marcin Jaskula 06/09/2002
//  Modified : Marcin Jaskula 25/02/2003
//             No calculate CL for fakes
//  Modified : Stefano Spataro 26/08/2005
//             Changed fCorrectedBeta[] in getBeta[]

//_HADES_CLASS_DESCRIPTION
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
#define MAX_FAKES_BETA 1.0
// -----------------------------------------------------------------------------

ClassImp(HPidAlgMomVsBeta)

// -----------------------------------------------------------------------------

const Float_t HPidAlgMomVsBeta::kDefMinBkg = 0.0f;
//        = TMath::Gaus(3.0) / TMath::Sqrt(2. * TMath::Pi());

const Float_t HPidAlgMomVsBeta::kDefMaxGaussDev = 3.0f;

// -----------------------------------------------------------------------------

HPidAlgMomVsBeta::HPidAlgMomVsBeta(void)
                        : HPidAlgorithm("PidAlgMomVsBeta", algMomVsBeta)
{
// Default constructor

    fMinBkg      = kDefMinBkg;
    fMaxGaussDev = kDefMaxGaussDev;
}

// -----------------------------------------------------------------------------

HPidAlgMomVsBeta::HPidAlgMomVsBeta(Float_t fWeight)
                        : HPidAlgorithm("PidAlgMomVsBeta", algMomVsBeta, fWeight)
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
    pParams->setContext(pRec->iSelectedMomAlg);

    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgMomVsBeta::reinit(void)
{
    //Check if parameter context corresponds to the
    //appropriate momentum algorithm index
    //If not -> return kFALSE (analysis can not be done in such case)
    // and print error message
    return pParams->checkContext(pRec->iSelectedMomAlg);
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


//    HKickTrack *pKickTrack;

  //cout << "Calculating PDFs" << endl;
  HPidTrackData *pTrackData = pTrack->getTrackData();
  HPidHitData *pHitData = pTrack->getHitData();

    if(pHitData->iIndTOF < 0)
    {
        // not a full track ??? No hit in TOF?
      //cout << "No tof hit!" << endl;
     // return kTRUE;
    }

/*   // There are HPidTrackData present in HPidTrackCand

    if((pKickTrack = pTrack->getKickTrack()) == NULL)
    {
        Error("calculatePDF", "Cannot get HKickTrack for the track: %d",
                            pTrack->getKickTrackId());

        return kFALSE;
    }
*/

static  Float_t SQRT_2PI = TMath::Sqrt(2.0 * TMath::Pi());


Int_t   iSelectedMomAlg = pRec->iSelectedMomAlg;
Float_t fCharge = pTrackData->nPolarity[iSelectedMomAlg];
Int_t   iSys    = pHitData->iSystem;
Int_t   iSect   = pHitData->nSector;
Float_t fMom    = pTrackData->fMomenta[iSelectedMomAlg];
Float_t fTheta  = pHitData->fMdcTheta;
//Float_t fBeta   = pTrackData->fCorrectedBeta[iSelectedMomAlg];
Float_t fBeta   = pTrackData->getBeta(iSelectedMomAlg);

if (pTrackData->nTofRecFlag[iSelectedMomAlg]==-1)  // skip tracks where the time-of-flight is not usable 
    {
        return kFALSE;
    }

Float_t f;
Int_t   particleId;
Float_t fParams[3];
Int_t   k, particle_counter;
 if(fCharge==0) cout << "undefined charge!" << endl ;
 for( particle_counter = 0; particle_counter < pRec->particlesNumber(); particle_counter++)
    {
        particleId = pRec->getParticleId(particle_counter);
	//cout << "checking for particle counter : " << particle_counter << endl;
        // compare the particle charge
        if(fCharge != HPidPhysicsConstants::charge(particleId))
        {
            afReturn[particle_counter] = 0.0f;
            continue;
        }

        if( ! HPidPhysicsConstants::isFake(particleId))
        {
            // gauss fit
            for(k = 0; k < 2; k++)
            {
                fParams[k] = pParams->getParameterValue( particleId, 3 * iSys + k, iSect,
                                fMom, fTheta);
            }

            if(fParams[1] <= 0.0f)
            {
                if(fParams[1] < 0.0f)
                {
                    Error("calculatePDF", "Wrong sigma value for gauss distr. "
                            "%d %d %d %f %f = %f",
                            particleId, 3 * iSys + 1, iSect, fMom, fTheta, fParams[1]);
                }

                afReturn[particle_counter] = 0.0f;

                continue;
            }

            f = (fBeta - fParams[0]) / fParams[1];

            if(iSys==1 && (fMaxGaussDev > 0.0f) && (TMath::Abs(f) > fMaxGaussDev)){
                afReturn[particle_counter] = 0.0f;
            }
            else if (iSys==0 && (fMaxGaussDev > 0.0f) && (TMath::Abs(f) > fMaxGaussDev) && fBeta < 1.){
	      //else if (iSys==0 && (fMaxGaussDev > 0.0f) && (TMath::Abs(f) > fMaxGaussDev)){

                afReturn[particle_counter] = 0.0f;
            }
            else
            {
                afReturn[particle_counter] = TMath::Exp(-0.5 * f * f)
                                    / (SQRT_2PI * fParams[1]);
		/*
		if(fBeta<1.0 && fBeta>0.80 && fMom< 1000 && fMom>700 && particleId==14)
		  {
		    cout << "Candidate momentum: " << fMom <<  endl;
		    cout << "Candidate velocity: " << fBeta <<  endl;
		    cout << "Candidate mean: " << fParams[0] <<  endl;
		    cout << "Candidate variance: " << fParams[1] <<  endl;
		    cout << "Candidate pdf: " << afReturn[particle_counter] <<  endl;
		    cout << "Nominal value: " << TMath::Exp(-0.5 * f * f)/(SQRT_2PI * fParams[1])<< endl;
		  }
		*/
            }
        }
        else
        {
            // pol2 fit for the background functions
            for(k = 0; k < 3; k++)
            {
                fParams[k] = pParams->getParameterValue(particleId, 3 * iSys + k, iSect,
                                fMom, fTheta);
            }

            afReturn[particle_counter] = fParams[0] + fParams[1] * fBeta
                                         + fParams[2] * fBeta * fBeta;

            if(afReturn[particle_counter] < fMinBkg)
                afReturn[particle_counter] = fMinBkg;
        }
    }
    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgMomVsBeta::calculateCL(HPidTrackCand *pTrack,
            Float_t afReturn[], Short_t &nCatIndex)
{
// Check the charge of the particle calculated by KickPlain

    HPidTrackData *pTrackData = pTrack->getTrackData();
    HPidHitData *pHitData = pTrack->getHitData();

    if(pHitData->iIndTOF < 0)
    {
        // not a full track ??? No hit in TOF?
        return kTRUE;
    }

/*   // There are HPidTrackData present in HPidTrackCand

    if((pKickTrack = pTrack->getKickTrack()) == NULL)
    {
        Error("calculatePDF", "Cannot get HKickTrack for the track: %d",
                            pTrack->getKickTrackId());

        return kFALSE;
    }
*/

static  Float_t SQRT_2PI = TMath::Sqrt(2.0 * TMath::Pi());


Int_t   iSelectedMomAlg = pRec->iSelectedMomAlg;
Float_t fCharge = pTrackData->nPolarity[iSelectedMomAlg];
Int_t   iSys    = pHitData->iSystem;
Int_t   iSect   = pHitData->nSector;
Float_t fMom    = pTrackData->fMomenta[iSelectedMomAlg];
Float_t fTheta  = pHitData->fMdcTheta;
Float_t fBeta   = pTrackData->fCorrectedBeta[iSelectedMomAlg];

Int_t   particleId;
Float_t fParams[3];
Int_t   k, particle_counter;

    for(particle_counter = 0; particle_counter < pRec->particlesNumber(); particle_counter++)
    {
        particleId = pRec->getParticleId(particle_counter);

        // do not calculate CL for fakes
        if(HPidPhysicsConstants::isFake(particleId))
        {
            afReturn[particle_counter] = -1.0f;
            continue;
        }

        // compare the particle charge
        if(fCharge != HPidPhysicsConstants::charge(particleId))
        {
            afReturn[particle_counter] = -2.0f;
            continue;
        }

        // gauss fit
        for(k = 0; k < 2; k++)
        {
            fParams[k] = pParams->getParameterValue(particleId, 3 * iSys + k, iSect,
                            fMom, fTheta);
        }

        if(fParams[1] <= 0.0f)
        {
            if(fParams[1] < 0.0f)
            {
                Error("calculateCL", "Wrong sigma value for gauss distr. "
                        "%d %d %d %f %f = %f",
                        particleId, 3 * iSys + 1, iSect, fMom, fTheta, fParams[1]);
            }

            afReturn[particle_counter] = -2.0f;

            continue;
        }

        afReturn[particle_counter] = HPidGaussConf::getConfLevel(fParams[0],
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
