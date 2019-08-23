// @(#)$Id: hpidalgmdceloss.cc,v 1.2 2008-09-18 13:06:21 halo Exp $
//*-- Author : Tassilo Christ 22/04/2002


////////////////////////////////////////////////////////////////////////////////
//
// HPidAlgMdcEloss
//
// PID method using RICH ring properties
//
////////////////////////////////////////////////////////////////////////////////

#pragma implementation

#include "hpidalgmdceloss.h"
#include "hpidtrackcand.h"
#include "hpidreconstructor.h"
#include "hgeantkine.h"
#include "hpidgeanttrackset.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hpidfl.h"
#include "hpidalgmdcelosspar.h"
#include "hpidphysicsconstants.h"
#include "hpidhitdata.h"
#include "hpidtrackdata.h"

//#include "hpidgaussconf.h"
#include "TVector.h"
#include <hkicktrack.h>
#include <hrichhit.h>
#include "TMath.h"
// -----------------------------------------------------------------------------

ClassImp(HPidAlgMdcEloss)

// -----------------------------------------------------------------------------

HPidAlgMdcEloss::HPidAlgMdcEloss(void)
                        : HPidAlgorithm("MdcEloss", algRich)
{
  // Default constructor
}

// -----------------------------------------------------------------------------

HPidAlgMdcEloss::HPidAlgMdcEloss(Float_t fWeight)
                        : HPidAlgorithm("MdcEloss", algRich, fWeight)
{
  // Default constructor with weight
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgMdcEloss::init(void)
{
// Get pointer to the parameter's container
  if((pParams = (HPidAlgMdcElossPar *)gHades->getRuntimeDb()
      ->getContainer(PIDALGMDCELOSSPAR_NAME)) == NULL)
    {
      Error("init", "Cannot get parameters: %s", PIDALGMDCELOSSPAR_NAME);
      return kFALSE;
    }
  //Sets the context to a name that corresponds to the chosen momentum algorithm
  //This is neccessary because the parameters depend on the chosen tracking
  //method
  pParams->setContext(pRec->iSelectedMomAlg);
  return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgMdcEloss::reinit(void)
{
  //Check if parameter context corresponds to the
  //appropriate momentum algorithm index in the reconstructor task
  //If not -> return kFALSE (analysis can not be done in this case)
  //and print error message
    return pParams->checkContext(pRec->iSelectedMomAlg);

}

// -----------------------------------------------------------------------------

Bool_t HPidAlgMdcEloss::finalize(void)
{
  return kTRUE;
}

// -----------------------------------------------------------------------------
// Calculate PDF from data stored in histograms in HPidAlgMdcElossPar
Bool_t HPidAlgMdcEloss::calculatePDF(HPidTrackCand *pTrack,
					   Float_t afReturn[], Short_t &nCatIndex)
{
  const HPidHitData* pHitData = pTrack->getHitData();
  const HPidTrackData* pTrackData = pTrack->getTrackData();
  

  //check momentum algorithm selected by the user
  Int_t iSelectedMomAlg = pRec->iSelectedMomAlg;
  if(pTrackData->bIsAccepted[iSelectedMomAlg]==kFALSE)
    {
      Error("HPidAlgMdcEloss::calculatePDF()","Selected momentum algorithm was not successful");
      exit(-1);
    }


  //Get momentum of track
  Float_t TrackMomentum = pTrackData->fMomenta[iSelectedMomAlg];
  if(TrackMomentum<0) 
    {
      Error("HPidAlgMdcEloss::calculatePDF()","Track momentum is negative");
      exit(-1);
    }

  Bool_t  fMdcEloss = pHitData->getInnerMdcdEdx()+pHitData->getOuterMdcdEdx();
  Float_t fCharge = pTrackData->nPolarity[iSelectedMomAlg];
  Int_t   sector = pHitData->nSector;
  Int_t   iId=-99;
  Int_t   i=0;
  //Get all possible particle ids that the reconstructor is taking into account
  for(i = 0; i < pRec->particlesNumber(); i++)
    {
      iId = pRec->getParticleId(i);
      
      // compare the particle charge to all charges associated with the current species
      if(fCharge != HPidPhysicsConstants::charge(iId))
	{
	  //If the charge doesn't match we obviously have 0 pdf!
	  afReturn[i] = 0.0f;
	  continue;
	}
      if(iId<0)
	{
	  afReturn[i]=-1.0;
	  continue;
	}
      
      afReturn[i] = pParams->getValue(iId,0,sector,fMdcEloss);
    }
  
  //Check whether all PDF values where zero (because the PDFs have no smooth trail-off for far-out data points
  Bool_t allZeros=kTRUE;
  for(Int_t particleCounter=0; particleCounter<pRec->particlesNumber(); particleCounter++)
    {
      if(afReturn[particleCounter]>0.0)
	{
	  allZeros=kFALSE;
	}
    }

  //If the PDF is not providing an anser we need to exclude this algorithm
  if(allZeros)
    {
      //loop over all species
      for(Int_t i = 0; i < pRec->particlesNumber(); i++)
	{
	  iId = pRec->getParticleId(i);
	  afReturn[i] =-1.0f;
	}
    }
  
  return kTRUE;
}


// -----------------------------------------------------------------------------

void HPidAlgMdcEloss::print(void) const
{
// Empty function for printing info of the algorithm

    printf("\tMdcEloss algorithm\n");
}


