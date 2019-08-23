// @(#)$Id: hpidalgringproperties.cc,v 1.12 2005-09-07 11:50:23 christ Exp $
//*-- Author : Tassilo Christ 22/04/2002


////////////////////////////////////////////////////////////////////////////////
//
// HPidAlgRingProperties
//
// Test algorithm for PID from kinematic data from kick plain
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
    pParams->setContext(pRec->iSelectedMomAlg);
    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgRingProperties::reinit(void)
{
     //Check if parameter context corresponds to the
    //appropriate momentum algorithm index
    //If not -> return kFALSE (analysis can not be done in such case)
    // and print error message
    return pParams->checkContext(pRec->iSelectedMomAlg);

}

// -----------------------------------------------------------------------------

Bool_t HPidAlgRingProperties::finalize(void)
{
// Dummy method

    return kTRUE;
    /*
    delete RingPattern5D;
    delete RingPattern4D;
    delete TransformedRingPattern5D;
    delete TransformedRingPattern4D;
    */
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgRingProperties::calculatePDF(HPidTrackCand *pTrack,
            Float_t afReturn[], Short_t &nCatIndex)
{
  // Calculate PDF from data stroed in histograms in HPidAlgRingPropertiesPar

  const HPidTrackData* pTrackData = pTrack->getTrackData();
  const HPidHitData* pHitData = pTrack->getHitData();

  Int_t iSelectedMomAlg = pRec->iSelectedMomAlg;
  Float_t TrackMomentum = pTrackData->fMomenta[iSelectedMomAlg];
  Bool_t bCentroid;

  //This is quesitonable - fix me
  if(TrackMomentum<0) return kTRUE;

  Float_t fCharge = pTrackData->nPolarity[iSelectedMomAlg];
  
  Int_t sector = pHitData->nSector;


  Float_t fTheta = pHitData->fMdcTheta;  

  if(pHitData->hasRingCorrelation[iSelectedMomAlg]==kFALSE)
    {
      // not a full track - no ring was part of this pidtrackcand!
      // This is the default! We have to assume that the KT was 
      // a) either a hadron that cannot make a ring
      // b) a lepton that failed to produce a ring
      // Therefore we set the PDFs in the return array to the values
      // corresponing to the fraction of hadrons not correlated to a KT 
      // (for hadronic species)
      // respectively to (1-"Rich lepton fraction") 
      // (for leptonic species) 
      // Here we produce probability statements for candidates without a ring:
      
      //First we take the momentum of the particle, because we built our normalization 
      //as a function of momentum

      
      Int_t iPID;

      for(Int_t particleCounter=0; particleCounter<pRec->particlesNumber(); particleCounter++)
	{
	  afReturn[particleCounter]=-1.0; //When there is no ring - do not use the RICH
	  /*
	  //cout << "particle counter : " << particleCounter << endl;
	  iPID = pRec->getParticleId(particleCounter);
	  //cout << "Species: " << iPID;
	  
	  if(iPID==2 && fCharge>0) //positron
	  {
	  //afReturn[particleCounter]=1.0-pParams->getRichLeptonFraction(TrackMomentumBin);
	  afReturn[particleCounter]=1.0-pParams->getRichLeptonFraction(TrackMomentum,fTheta,sector);
	  }
	  
	  if(iPID==3 && fCharge<0) //electron
	  {
	  //afReturn[particleCounter]=1.0-pParams->getRichLeptonFraction(TrackMomentumBin);
	  afReturn[particleCounter]=1.0-pParams->getRichLeptonFraction(TrackMomentum,fTheta,sector);
	  }
	  
	  if(iPID==8 && fCharge>0) //pi+, proton, k+, deuteron
	  {
	  //afReturn[particleCounter]=1.0-pParams->getRichHadronFraction(TrackMomentumBin);
	  afReturn[particleCounter]=1.0-pParams->getRichPiPlusFraction(TrackMomentum,fTheta,sector);
	  }
	  
	  if((iPID>9) && fCharge>0) //pi+, proton, k+, deuteron
	  {
	  //afReturn[particleCounter]=1.0-pParams->getRichHadronFraction(TrackMomentumBin);
	  afReturn[particleCounter]=1.0-pParams->getRichProtonFraction(TrackMomentum,fTheta,sector);
	  }
	  
	  if(iPID==9 && fCharge<0) //pi-
	  {
	  //afReturn[particleCounter]=1.0-pParams->getRichHadronFraction(TrackMomentumBin);
	  afReturn[particleCounter]=1.0-pParams->getRichPiMinusFraction(TrackMomentum,fTheta,sector);
	  }
	  
	  if((iPID==-1 && fCharge<0) || (iPID==-2 &&fCharge>0)) //fake=, fake-
	  {
	  afReturn[particleCounter]=1.0-pParams->getRichFakeFraction(TrackMomentum,fTheta,sector);
	  }
	  
	  
	  //cout << "Value: " <<afReturn[particleCounter] << endl;
	  if(afReturn[particleCounter]<0.0)
	  {
	  cout << "Error for ringless track! Negative PDF value returned!" << endl;
	  }
	  */
	}
	  /*
	  
	  Bool_t allZeros=kTRUE;
	  for(Int_t particleCounter=0; particleCounter<pRec->particlesNumber(); particleCounter++)
	  {
	  if(afReturn[particleCounter]>0.0)
	  {
	  allZeros=kFALSE;
	  }
	  if(allZeros) cout << "Error for ringless track! ZERO PDF value returned for all species!"<< endl;
	  */
      return kTRUE;
      
    }
  
  if(pHitData->iIndRICH < 0)
    {
      Error("calculatePDF", "No HRichHit for this track");
      return kFALSE;
    }

  
  //we have a ring - now we get it's properties!
  //cout << "Entering evaluation of particle with ring!" << endl;
  Float_t fAvCharge = (Float_t)pHitData->nRingAmplitude/(Float_t)pHitData->nRingPadNr;
  Float_t fPadNr = pHitData->nRingPadNr;
  Float_t fCentroid = pHitData->fRingCentroid;
  Float_t fNLocMax = pHitData->nRingLocalMax4;
  Float_t fPatMat = pHitData->nRingPatMat;

  Float_t Eigencoord_One=0.0;
  Float_t Eigencoord_Two=0.0;
  Int_t CentroidStatus;

  /*cout << "Ring properties: " << endl;
  cout << "Ring amplitude: " << pHitData->nRingAmplitude<< endl;
  cout << "Ring Pad Nr: " << pHitData->nRingPadNr<< endl;
  cout << "Ring centroid: " << pHitData->fRingCentroid<< endl;
  cout << "Ring pattern matrix: " << pHitData->nRingPatMat<< " " <<fPatMat<<endl;
  cout << "Ring avcharge: " << fAvCharge<<endl;
  */
  /*
  if(fCentroid>=0.0)
    {
      //We have a ring with a well defined centroid (all 5 pieces of information are available)
      RingPattern5D[0]=fPatMat;
      RingPattern5D[1]=fPadNr;
      RingPattern5D[2]=fCentroid;
      RingPattern5D[3]=fNLocMax;
      RingPattern5D[4]=fAvCharge;
      //TransformToEigenspace5D(RingPattern5D,TransformedRingPattern5D, sector);
    }
  else
    {
      //We have a ring without a well defined centroid (only 4 pieces of information are available)
      RingPattern4D[0]=fPatMat;
      RingPattern4D[1]=fPadNr;
      RingPattern4D[2]=fNLocMax;
      RingPattern4D[3]=fAvCharge;
      //TransformToEigenspace4D(RingPattern4D,TransformedRingPattern4D,sector);
    }
  */
  Int_t   iId;
  Int_t   i;
  for(i = 0; i < pRec->particlesNumber(); i++)
    {
      //Get all possible particle ids that the reconstructor is taking into account
      iId = pRec->getParticleId(i);


      // compare the particle charge to all charges associated with those ids
      if(fCharge != HPidPhysicsConstants::charge(iId))
        {
	  //If the charge doesn't match we obviously have 0 pdf!
	  afReturn[i] = 0.0f;
	  continue;
        }
      
      //If the ring is complete we use the first PDF
      if(fCentroid>=0)
	{
	  bCentroid=kTRUE;
	  
	  //Eigencoord_One=TransformedRingPattern5D[0];
	  //Eigencoord_Two=TransformedRingPattern5D[1];
	  Eigencoord_One=fPatMat;
	  Eigencoord_Two=fAvCharge;
	  CentroidStatus=CentroidDefined;
	  
	}
      //if not we use the second
      else
	{
	  bCentroid=kFALSE;
	  //Eigencoord_One=RingPattern4D[0];
	  //Eigencoord_Two=RingPattern4D[1];
	  Eigencoord_One=fPatMat;
	  Eigencoord_Two=fAvCharge;
	  CentroidStatus=CentroidUndefined;
	}
      //cout << "pm: " << Eigencoord_One << endl;
      //cout << "fAvCharge: " << Eigencoord_Two << endl;
      afReturn[i] = pParams->getNormalizedPDFValue(iId,fTheta,sector,bCentroid,Eigencoord_One,Eigencoord_Two);
    }

  Bool_t allZeros=kTRUE;
  for(Int_t particleCounter=0; particleCounter<pRec->particlesNumber(); particleCounter++)
    {
      if(afReturn[particleCounter]>0.0)
	{
	  allZeros=kFALSE;
	}
    }
  if(allZeros)
    {
      //We need to come to a decision using the hard cut method:
      for(i = 0; i < pRec->particlesNumber(); i++)
	{
	  iId = pRec->getParticleId(i);
	  if(fCharge != HPidPhysicsConstants::charge(iId))
	    {
	      //If the charge doesn't match we obviously have 0 pdf!
	      afReturn[i] = 0.0f;
	      continue;
	    }
	  if(iId==2 || iId==3) //lepton candidates!
	    {
	      if(fCentroid<=2.8 &&
		 fAvCharge>=5 &&
		 fPatMat>=200 &&
		 fPadNr>=4)
		{
		  afReturn[i]=1.0; //This was an electron by hard cut decision!
		}
	    }
	}
      /*
      cout << "Error for track with ring! ZERO PDF value returned for all species! " << endl;
      cout << "centroid: " << fCentroid <<endl;
      cout << "cavcharge: " << fAvCharge <<endl;
      cout << "fPatMat: " << fPatMat <<endl;
      cout << "fPadNr: " << fPadNr <<endl;
      cout << "fNLocMax: " << fNLocMax <<endl;
      cout << "centroid: " << fCentroid <<endl;
      cout << "theta: " << fTheta << endl;
      cout << "sector: " << sector << endl;
      */
    }
  correctRelativeYields(afReturn,TrackMomentum,fTheta,sector);
  return kTRUE;
}

// -----------------------------------------------------------------------------
//This method corrects the yields which are stored in the parameter container.
//This is neccessary in order to compensate for the hadon blindness of the RICH
//otherwise we would never recognize leptons with the RICH algorithm.
//This method only corrects those probability statements which are made for
//Candidates with a ring!
Bool_t HPidAlgRingProperties::correctRelativeYields(Float_t* pdfValues,Float_t TrackMomentum, 
						    Float_t fTheta, Int_t sector)
{
  Int_t iPID;

  Float_t proton_yield_correction = pParams->getRichProtonFraction(TrackMomentum,fTheta,sector);
  Float_t piplus_yield_correction = pParams->getRichPiPlusFraction(TrackMomentum,fTheta,sector);
  Float_t piminus_yield_correction = pParams->getRichPiMinusFraction(TrackMomentum,fTheta,sector);
  Float_t lepton_yield_correction = pParams->getRichLeptonFraction(TrackMomentum,fTheta,sector);
  Float_t fake_yield_correction = pParams->getRichFakeFraction(TrackMomentum,fTheta,sector);

  for(Int_t particleCounter=0; particleCounter<pRec->particlesNumber(); particleCounter++)
    {
      iPID = pRec->getParticleId(particleCounter);
      if(iPID==3 || iPID==2) //leptonic species
      {
	pdfValues[particleCounter]*=lepton_yield_correction;
	if(pdfValues[particleCounter]<0.0) cout << "Negative PDF after correction!";
      }

      if(iPID==8)
	{
	  pdfValues[particleCounter]*=piplus_yield_correction;
	  if(pdfValues[particleCounter]<0.0) cout << "Negative PDF after correction!";
	}

      if(iPID==9)
	{
	  pdfValues[particleCounter]*=piminus_yield_correction;
	  if(pdfValues[particleCounter]<0.0) cout << "Negative PDF after correction!";
	}

      if(iPID==14)
	{
	  pdfValues[particleCounter]*=proton_yield_correction;
	  if(pdfValues[particleCounter]<0.0) cout << "Negative PDF after correction!";
	}

      if(iPID<0)
	{
	  pdfValues[particleCounter]*=fake_yield_correction;
	  if(pdfValues[particleCounter]<0.0) cout << "Negative PDF after correction!";
	}
      
    }

  return kTRUE;
}


Int_t HPidAlgRingProperties::getThetaBin(Float_t fKickTheta)
{
  if(fKickTheta<=30) return 0;
  if(fKickTheta<=50) return 1;
  if(fKickTheta<=90) return 2;
  return -1;
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
  //Rings which have a well defined centroid! The transformation parameters and the variances
  //are hardwired for the moment, but later they have to be retrieved from a parameter container
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
	  //TransformedData[i] += (OriginalData[j] - gMeanValues[j]) 
	  //* gEigenVectors[j *  gNVariables + i] / gSigmaValues[j];
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


