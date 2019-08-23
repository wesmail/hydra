// @(#)$Id: hpidalgringproperties.cc,v 1.16 2006-11-24 16:00:29 christ Exp $
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
#include <TMath.h>
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
      //When there is no ring - do not use the RICH
      //Set all probabilities to -1 indicating that the RICH shall not be considered 
      //in making the decision!
      for(Int_t particleCounter=0; particleCounter<pRec->particlesNumber(); particleCounter++)
	{
	  afReturn[particleCounter]=-1.0; 	  
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
  
  //Intercept noise in real experiment! - Treat it as "no-ring-case"
  /*
  if(fAvCharge<5 || fPatMat<200)
    {
      //When there is no ring - do not use the RICH
      //Set all probabilities to -1 indicating that the RICH shall not be considered 
      //in making the decision!
      for(Int_t particleCounter=0; particleCounter<pRec->particlesNumber(); particleCounter++)
	{
	  afReturn[particleCounter]=-1.0; 	  
	}
      return kTRUE;
    }
  */

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
      if(iId<0)
	{
	  afReturn[i]=-1.0;
	  continue;
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

  //If the PDF is not providing an anser we need to come to a decision using the hard cut method:  
  if(allZeros)
    {
      //loop over all species
      for(i = 0; i < pRec->particlesNumber(); i++)
	{
	  iId = pRec->getParticleId(i);
	  if(fCharge != HPidPhysicsConstants::charge(iId))
	    {
	      //If the charge doesn't match we obviously have 0 pdf!
	      afReturn[i] = 0.0f;
	      continue;
	    }

	  //if the species is a lepton we apply hard cut decision
	  if(iId==2 || iId==3)
	    {
	      if(fCentroid<=2.8 &&
		 fAvCharge>=5 &&
		 fPatMat>=200 &&
		 fPadNr>=4)
		{
		  afReturn[i]=0.99; //This leaves all other methods a chance to reject this guy still even if it has a nice ring
		}
	      else
		{
		  afReturn[i]=0.0;  //This rules out that this will ever become a lepton 
		}
	    }

	  //if the species is not leptonic we assign the original value of -1 to the probability as hard cut decision would do it.
	  else
	    {
	      afReturn[i]=nUNKNOWN;
	    }
	  //cout << "All Zeros" << endl;
	  //cout << "iId: " << iId << endl;
	  //cout << "avcharge: " << fAvCharge << endl;
	  //cout << "patmat: " << fPatMat << endl;
	}
    }
  
  correctRelativeYields(afReturn,TrackMomentum,fTheta,sector);
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

      if(iPID==14)
	{
	  if(pdfValues[particleCounter]>0.0)
	    {
	      pdfValues[particleCounter]*=proton_yield_correction;
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


