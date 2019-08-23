// @(#)$Id: hpidalgringproperties.cc,v 1.20 2008-09-18 13:06:21 halo Exp $
//*-- Author : Tassilo Christ 22/04/2002

//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////////////////////
//
// HPidAlgRingProperties
//
// PID method using RICH ring properties
//
////////////////////////////////////////////////////////////////////////////////

#pragma implementation

#include "hpidalgringproperties.h"
#include "hpidtrackcand.h"
#include "hpidreconstructor.h"
#include "hgeantkine.h"
#include "hpidgeanttrackset.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hpidfl.h"
#include "hpidalgringpropertiespar.h"
#include "hpidphysicsconstants.h"

//#include "hpidgaussconf.h"
#include "TVector.h"
#include <hkicktrack.h>
#include <hrichhit.h>
#include "TMath.h"
// -----------------------------------------------------------------------------

ClassImp(HPidAlgRingProperties)

// -----------------------------------------------------------------------------

HPidAlgRingProperties::HPidAlgRingProperties(void)
                        : HPidAlgorithm("RingProperties", algRich)
{
  // Default constructor
}

// -----------------------------------------------------------------------------

HPidAlgRingProperties::HPidAlgRingProperties(Float_t fWeight)
                        : HPidAlgorithm("RingProperties", algRich, fWeight)
{
  // Default constructor with weight
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgRingProperties::init(void)
{
// Get pointer to the parameter's container
  if((pParams = (HPidAlgRingPropertiesPar *)gHades->getRuntimeDb()
      ->getContainer(PIDALGRINGPROPERTIESPAR_NAME)) == NULL)
    {
      Error("init", "Cannot get parameters: %s", PIDALGRINGPROPERTIESPAR_NAME);
      return kFALSE;
    }
  //Sets the context to a name that corresponds to the chosen momentum algorithm
  //This is neccessary because the parameters depend on the chosen tracking
  //method
  pParams->setContext(pRec->iSelectedMomAlg);
  for(Int_t i = 0; i < pRec->particlesNumber(); i++)
    {
      Int_t iId = pRec->getParticleId(i);
      if(HPidPhysicsConstants::isFake(iId))
	{
	  ::Error("HPidAlgRingProperties::init()","The RICH cannot provide a PDF for fake particles!");
	  exit(-1);
	}

    }

  return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgRingProperties::reinit(void)
{
  //Check if parameter context corresponds to the
  //appropriate momentum algorithm index in the reconstructor task
  //If not -> return kFALSE (analysis can not be done in this case)
  //and print error message
    return pParams->checkContext(pRec->iSelectedMomAlg);

}

// -----------------------------------------------------------------------------

Bool_t HPidAlgRingProperties::finalize(void)
{
  return kTRUE;
}

// -----------------------------------------------------------------------------
// Calculate PDF from data stored in histograms in HPidAlgRingPropertiesPar
Bool_t HPidAlgRingProperties::calculatePDF(HPidTrackCand *pTrack,
					   Float_t afReturn[], Short_t &nCatIndex)
{
  const HPidTrackData* pTrackData = pTrack->getTrackData();
  const HPidHitData* pHitData = pTrack->getHitData();
  

  //check momentum algorithm selected by the user
  Int_t iSelectedMomAlg = pRec->iSelectedMomAlg;
  if(pTrackData->bIsAccepted[iSelectedMomAlg]==kFALSE)
    {
      Error("HPidAlgRingProperties::calculatePDF()","Selected momentum algorithm was not successful");
      exit(-1);
    }


  //Get momentum of track
  Float_t TrackMomentum = pTrackData->fMomenta[iSelectedMomAlg];
  if(TrackMomentum<0) 
    {
      Error("HPidAlgRingProperties::calculatePDF()","Track momentum is negative");
      exit(-1);
    }

  Bool_t  bCentroid;
  Float_t fCharge = pTrackData->nPolarity[iSelectedMomAlg];
  Int_t   sector = pHitData->nSector;
  Float_t fTheta = pHitData->fMdcTheta;  



  //Check whether RICH is usable at all
  if(pHitData->hasRingCorrelation[iSelectedMomAlg]==kFALSE)
    {
      //When there is no ring - do not use the RICH but make sure this NEVER becomes a lepton!
      for(Int_t particleCounter=0; particleCounter<pRec->particlesNumber(); particleCounter++)
	{
	  if(pRec->getParticleId(particleCounter)==2 || pRec->getParticleId(particleCounter)==3)
	    {
	      afReturn[particleCounter]=0.0; //This will NEVER be a lepton 	  
	    }
	  else
	    {
	      afReturn[particleCounter]=1.0; //We do not know what hadron this will be
	    }
	}
      return kTRUE;
    }      

  if(pHitData->iIndRICH < 0)
    {
      Error("HPidAlgRingProperties::calculatePDF()", "No HRichHit but correlation flag!");
      exit(-1);
    }
  
  
  //we have a ring - now we get it's properties!
  Float_t fAvCharge = (Float_t)pHitData->nRingAmplitude/(Float_t)pHitData->nRingPadNr;
  Float_t fPadNr = pHitData->nRingPadNr;
  Float_t fCentroid = pHitData->fRingCentroid;
  Float_t fPatMat = pHitData->nRingPatMat;
  
  //obsolete variables until revival of PCA
  Float_t Coordinate_One=0.0;
  Float_t Coordinate_Two=0.0;
  
  Int_t   iId;
  Int_t   i;
  
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
      if(HPidPhysicsConstants::isFake(iId))
	{
	  Error("HPidAlgRingProperties::calculatePDF()", "Rich does not provide PDFs for fake particle species!");
	  exit(-1);
	}
      
      //If the ring is complete (i.e. has a centroid) we use the first PDF
      if(fCentroid>=0)
	{
	  bCentroid=kTRUE;
	  Coordinate_One=fPatMat;
	  Coordinate_Two=fAvCharge;
	}
      
      //if not we use the second
      else
	{
	  bCentroid=kFALSE;
	  Coordinate_One=fPatMat;
	  Coordinate_Two=fAvCharge;
	}
      if(iId<0) cout << "Who said 'fake?'" <<endl;
      //Store the PDF value for the current index (i) and the associated species (iId)
      afReturn[i] = pParams->getNormalizedPDFValue(iId,fTheta,sector,bCentroid,Coordinate_One,Coordinate_Two);
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
  correctRelativeYields(afReturn,TrackMomentum,fTheta,sector);


  if(allZeros)
    {
      //If all RICH PDFs had 0 value at this specific point of the phase space, we have to treat this case as a no-ring-case.
      for(Int_t particleCounter=0; particleCounter<pRec->particlesNumber(); particleCounter++)
	{
	  if(pRec->getParticleId(particleCounter)==2 || pRec->getParticleId(particleCounter)==3)
	    {
	      afReturn[particleCounter]=0.0; //This will NEVER be a lepton 	  
	    }
	  else
	    {
	      afReturn[particleCounter]=1.0; //We do not know what hadron this will be
	    }
	}
    }
  return kTRUE;
}

//This method corrects the yields which are stored in the parameter container
//for HPidReconstructor. This is neccessary in order to compensate for the hadon 
//blindness of the RICH. Otherwise we would never recognize leptons with the RICH algorithm.
//This method only corrects those probability statements which are made for
//Candidates with a ring! For a detailed explanation see the WIKI pages about
//BAYES THEOREM
// -----------------------------------------------------------------------------
Bool_t HPidAlgRingProperties::correctRelativeYields(Float_t* pdfValues,Float_t TrackMomentum, 
						    Float_t fTheta, Int_t sector)
{
  Int_t iPID;

  //These numbers hold the approximate fractions of p,e+,e-,pi+,pi-,f+,f- that are associated to
  //a ring. 
  Float_t proton_yield_correction = pParams->getRichProtonFraction(TrackMomentum,fTheta,sector);
  Float_t kminus_yield_correction = pParams->getRichProtonFraction(TrackMomentum,fTheta,sector);
  Float_t kplus_yield_correction = pParams->getRichProtonFraction(TrackMomentum,fTheta,sector);
  Float_t deuteron_yield_correction = pParams->getRichProtonFraction(TrackMomentum,fTheta,sector);
  Float_t piplus_yield_correction = pParams->getRichPiPlusFraction(TrackMomentum,fTheta,sector);
  Float_t piminus_yield_correction = pParams->getRichPiMinusFraction(TrackMomentum,fTheta,sector);
  Float_t lepton_yield_correction = pParams->getRichLeptonFraction(TrackMomentum,fTheta,sector);
  //Float_t fake_yield_correction = pParams->getRichFakeFraction(TrackMomentum,fTheta,sector);
  //Float_t fake_yield_correction = 1.0;

  //Multiply the PDF values returned by the RICH with the probability that a particle is associated to a
  //ring at all
  for(Int_t particleCounter=0; particleCounter<pRec->particlesNumber(); particleCounter++)
    {
      iPID = pRec->getParticleId(particleCounter);
      if(iPID==3 || iPID==2) //leptonic species
      {

	if(pdfValues[particleCounter]>0.0) 
	  {
	    pdfValues[particleCounter]*=lepton_yield_correction;
	  }
	
      }
      if(iPID==8)
	{

	  if(pdfValues[particleCounter]>0.0)
	    {
	      pdfValues[particleCounter]*=piplus_yield_correction;
	    } 
	}

      if(iPID==9)
	{
	  if(pdfValues[particleCounter]>0.0)
	    {
	      pdfValues[particleCounter]*=piminus_yield_correction;
	    }
	}

      if(iPID==11)
	{

	  if(pdfValues[particleCounter]>0.0)
	    {
	      pdfValues[particleCounter]*=kplus_yield_correction;
	    } 
	}

      if(iPID==12)
	{
	  if(pdfValues[particleCounter]>0.0)
	    {
	      pdfValues[particleCounter]*=kminus_yield_correction;
	    }
	}

      if(iPID==14)
	{
	  if(pdfValues[particleCounter]>0.0)
	    {
	      pdfValues[particleCounter]*=proton_yield_correction;
	    }
	}

      if(iPID==45)
	{
	  if(pdfValues[particleCounter]>0.0)
	    {
	      pdfValues[particleCounter]*=deuteron_yield_correction;
	    }
	}

      if(iPID<0)
	{
	  //pdfValues[particleCounter]*=fake_yield_correction;
	  continue;
	}
      
    }

  return kTRUE;
}


// -----------------------------------------------------------------------------

void HPidAlgRingProperties::print(void) const
{
// Empty function for printing info of the algorithm

    printf("\tRingProperties algorithm\n");
}

Bool_t HPidAlgRingProperties::TransformToEigenspace5D(Double_t* OriginalData, Double_t* TransformedData, Int_t sector)
{
  //Here we use the Function X2P which was built by the TPrincipal->MakePrincipals routine for 
  //Rings which have a well defined centroid! 
  static Int_t    gNVariables = 5;

  if(pParams->getUse5DTransform())
    {
      for (Int_t i = 0; i < gNVariables; i++) {
	TransformedData[i] = 0;
	for (Int_t j = 0; j < gNVariables; j++)
	  TransformedData[i] += (OriginalData[j] - pParams->get5DMean(sector,j)) 
	    * pParams->get5DEvec(sector,j *  gNVariables + i) / pParams->get5DSigma(sector,j);
      }
    }
  else
    {
      for (Int_t i = 0; i < gNVariables; i++) 
	{
	  TransformedData[i] = OriginalData[i];
	}
    }
  return kTRUE;
}  

Bool_t HPidAlgRingProperties::TransformToEigenspace4D(Double_t* OriginalData, Double_t* TransformedData,Int_t sector)
{
  //Here we call the Function X2P which was built by the TPrincipal->MakePrincipals routine for 
  //Rings which have no well defined centroid!
  
  static Int_t    gNVariables = 4;

  if(pParams->getUse4DTransform())
    {
      for (Int_t i = 0; i < gNVariables; i++) {
	TransformedData[i] = 0;
	for (Int_t j = 0; j < gNVariables; j++)
	  TransformedData[i] += (OriginalData[j] - pParams->get4DMean(sector,j))
	    *pParams->get4DEvec(sector,j *  gNVariables + i) / pParams->get4DSigma(sector,j);
      }
    }
  else
    {
      for (Int_t i = 0; i < gNVariables; i++) 
	{
	  TransformedData[i] = OriginalData[i];
	}
    }
  return kTRUE;
}


