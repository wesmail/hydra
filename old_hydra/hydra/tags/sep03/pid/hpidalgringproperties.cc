// @(#)$Id: hpidalgringproperties.cc,v 1.2 2003-07-21 16:29:38 christ Exp $
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

#include "hpidalgringpropertiespar.h"
#include "hpidphysicsconstants.h"

#include "hpidgaussconf.h"
#include "TVector.h"
#include <hkicktrack.h>
#include <hrichhit.h>

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

    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgRingProperties::reinit(void)
{
// Dummy method

    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgRingProperties::finalize(void)
{
// Dummy method

    return kTRUE;
    delete RingPattern5D;
    delete RingPattern4D;
    delete TransformedRingPattern5D;
    delete TransformedRingPattern4D;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgRingProperties::calculatePDF(HPidTrackCand *pTrack,
            Float_t afReturn[], Short_t &nCatIndex)
{
  // Calculate PDF from data stroed in histograms in HPidAlgRingPropertiesPar
  //pParams->setRichHadronEfficiency(0.0);
  //Get the ring associated to the current PidTrackCand (if any)
  HRichHit *pRichHit ;
  
  //We need the KT to check whether we have a positive or a negative particle!
  HKickTrack *pKickTrack;

  if(pTrack->getKickTrackId() < 0)
    {
      // not a full track
      return kTRUE;
    }
  else
    {
      if((pKickTrack = pTrack->getKickTrack()) == NULL)
	{
	  Error("execute", "Cannot get HKickTrack for the track: %d",
		pTrack->getKickTrackId());
	  
	  return kFALSE;
	}
    }

  //Get the charge from the KT!
  Float_t fCharge = pKickTrack->getCharge();
  if(pTrack->getRingId() < 0)
    {
      // not a full track - no ring was part of this pidtrackcand!
      // This is the default! We have to assume that the KT was 
      // a) either a hadron that cannot make a ring
      // b) a lepton that failed to produce a ring
      // Therefore we set the PDFs in the return array to the values
      // corresponing to the fraction of hadrons not correlated to a KT 
      // (for hadronic species)
      // respectively to (1- Rich lepton efficiency) 
      // (for leptonic species) 
      // Here we produce probability statements for Candidates without a ring:
      Int_t iPID;
      for(Int_t particleCounter=0; particleCounter<pRec->particlesNumber(); particleCounter++)
	{
	  afReturn[particleCounter]=0;
	  iPID = pRec->getParticleId(particleCounter);
	  if(iPID==2 && fCharge>0) //positron
	    {
	      afReturn[particleCounter]=1.0-pParams->getRichLeptonEfficiency();
	    }
	  if(iPID==3 && fCharge<0) //electron
	    {
	      afReturn[particleCounter]=1.0-pParams->getRichLeptonEfficiency();
	    }
	  if((iPID==-2 || iPID==8 || iPID==14 || iPID==11 || iPID==45 ) && fCharge>0) //fake+, pi+, proton, k+, deuteron
	    {
	      afReturn[particleCounter]=1.0-pParams->getRichHadronEfficiency();
	    }
	  if((iPID==-1 || iPID==9) && fCharge<0) //fake-, pi-
	    {
	      afReturn[particleCounter]=1.0-pParams->getRichHadronEfficiency();
	    }
	}
      return kTRUE;
      
    }
  
  if((pRichHit = pTrack->getRichHit()) == NULL)
    {
      Error("calculatePDF", "Cannot get HRichHit for ring: %d",
	    pTrack->getRingId());
      
      return kFALSE;
    }

  
  //we have a ring - now we get it's properties!
  Float_t fAvCharge = pRichHit->getRingAmplitude()/pRichHit->getRingPadNr();
  Float_t fPadNr = pRichHit->getRingPadNr();
  Float_t fCentroid = pRichHit->getCentroid();
  Float_t fNLocMax = pRichHit->getRingLocalMax4();
  Float_t fPatMat = pRichHit->getRingPatMat();
  
  Float_t Eigencoord_One=0.0;
  Float_t Eigencoord_Two=0.0;
  Int_t CentroidStatus;
  
  if(fCentroid>=0.0)
    {
      //We have a ring with a well defined centroid (all 5 pieces of information are available)
      RingPattern5D[0]=fPatMat;
      RingPattern5D[1]=fPadNr;
      RingPattern5D[2]=fCentroid;
      RingPattern5D[3]=fNLocMax;
      RingPattern5D[4]=fAvCharge;
      TransformToEigenspace5D(RingPattern5D,TransformedRingPattern5D);
      /*
      cout << "Ring has well defined centroid! " << endl;
      cout << "PMQ:" << RingPattern5D[0] << endl;
      cout << "PadNr:" << RingPattern5D[1] << endl;
      cout << "centroid:" << RingPattern5D[2] << endl;
      cout << "locmax:" << RingPattern5D[3] << endl;
      cout << "avcharge:" << RingPattern5D[4] << endl; 
      cout <<"Eigencomponent 1: " << TransformedRingPattern5D[0]<< endl;
      cout <<"Eigencomponent 2: " << TransformedRingPattern5D[1]<<endl; 
      */
    }
  else
    {
      //We have a ring without a well defined centroid (only 4 pieces of information are available)
      RingPattern4D[0]=fPatMat;
      RingPattern4D[1]=fPadNr;
      RingPattern4D[2]=fNLocMax;
      RingPattern4D[3]=fAvCharge;
      /*
      cout << "Ring has centroid -1!" << endl;
      cout << "PMQ:" << RingPattern4D[0] << endl;
      cout << "PadNr:" << RingPattern4D[1] << endl;
      cout << "locmax:" << RingPattern4D[2] << endl;
      cout << "avcharge:" << RingPattern4D[3] << endl;
      */
      //It turns out that the transformation to the eigenspace of the patterns is only improving 
      //the results in 5 dimensions! In 4 dimensions it is advantageous to use original coordinates
      //TransformToEigenspace4D(RingPattern4D,TransformedRingPattern4D);
    }

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
	  Eigencoord_One=TransformedRingPattern5D[0];
	  Eigencoord_Two=TransformedRingPattern5D[1];
	  CentroidStatus=CentroidDefined;
	  
	}
      //if not we use the second
      else
	{
	  Eigencoord_One=RingPattern4D[0];
	  Eigencoord_Two=RingPattern4D[1];
	  CentroidStatus=CentroidUndefined;
	}
      afReturn[i] = pParams->getNormalizedPDFValue(iId,CentroidStatus,Eigencoord_One,Eigencoord_Two);
    }
  correctRelativeYields(afReturn);
  return kTRUE;
}

// -----------------------------------------------------------------------------
//This method corrects the yields which are stored in the parameter container.
//This is neccessary in order to compensate for the hadon blindness of the RICH
//otherwise we would never recognize leptons with the RICH algorithm.
//This method only corrects those probability statements which are made for
//Candidates with a ring!
Bool_t HPidAlgRingProperties::correctRelativeYields(Float_t* pdfValues)
{
  Int_t iPID;
  Float_t hadron_yield_correction = pParams->getRichHadronEfficiency();
  Float_t lepton_yield_correction = pParams->getRichLeptonEfficiency();
  for(Int_t particleCounter=0; particleCounter<pRec->particlesNumber(); particleCounter++)
    {
      iPID = pRec->getParticleId(particleCounter);
      if(iPID==3 || iPID==2) //leptonic species
	{
	  pdfValues[particleCounter]*=lepton_yield_correction;
	}
      else
	{
	  pdfValues[particleCounter]*=hadron_yield_correction;
	}
	
    }

  return kTRUE;
}


Bool_t HPidAlgRingProperties::calculateCL(HPidTrackCand *pTrack,
            Float_t afReturn[], Short_t &nCatIndex)
{
// Get CL from data stroed in histograms in HPidAlgRingPropertiesPar
  //pParams->setRichHadronEfficiency(0.0);
  //Get the ring associated to the current PidTrackCand (if any)
  HRichHit *pRichHit ;
  
  //We need the KT to check whether we have a positive or a negative particle!
  HKickTrack *pKickTrack;

  
  if(pTrack->getRingId() < 0)
    {
      // not a full track - no ring was part of this pidtrackcand!
      return kTRUE;
    }
  
  if((pRichHit = pTrack->getRichHit()) == NULL)
    {
      Error("calculatePDF", "Cannot get HRichHit for ring: %d",
	    pTrack->getRingId());
      
      return kFALSE;
    }

  
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

  //Get the charge from the KT!
  Float_t fCharge = pKickTrack->getCharge();

  //we have a ring - now we get it's properties!
  Float_t fAvCharge = pRichHit->getRingAmplitude()/pRichHit->getRingPadNr();
  Float_t fPadNr = pRichHit->getRingPadNr();
  Float_t fCentroid = pRichHit->getCentroid();
  Float_t fNLocMax = pRichHit->getRingLocalMax4();
  Float_t fPatMat = pRichHit->getRingPatMat();
  
  Float_t Eigencoord_One=0.0;
  Float_t Eigencoord_Two=0.0;
  Int_t CentroidStatus;

  if(fCentroid>=0)
    {
      //We have a ring with a well defined centroid (all 5 pieces of information are available)
      RingPattern5D[0]=fPatMat;
      RingPattern5D[1]=fPadNr;
      RingPattern5D[2]=fCentroid;
      RingPattern5D[3]=fNLocMax;
      RingPattern5D[4]=fAvCharge;
      TransformToEigenspace5D(RingPattern5D,TransformedRingPattern5D);
      /*
      cout << "Ring has well defined centroid! " << endl;
      cout << "PMQ:" << RingPattern5D[0] << endl;
      cout << "PadNr:" << RingPattern5D[1] << endl;
      cout << "centroid:" << RingPattern5D[2] << endl;
      cout << "locmax:" << RingPattern5D[3] << endl;
      cout << "avcharge:" << RingPattern5D[4] << endl; 
      cout <<"Eigencomponent 1: " << TransformedRingPattern5D[0]<< endl;
      cout <<"Eigencomponent 2: " << TransformedRingPattern5D[1]<<endl;
      */
    }
  else
    {
      //We have a ring without a well defined centroid (only 4 pieces of information are available)
      RingPattern4D[0]=fPatMat;
      RingPattern4D[1]=fPadNr;
      RingPattern4D[2]=fNLocMax;
      RingPattern4D[3]=fAvCharge;
      /*
      cout << "Ring has centroid -1!" << endl;
      cout << "PMQ:" << RingPattern5D[0] << endl;
      cout << "PadNr:" << RingPattern5D[1] << endl;
      cout << "locmax:" << RingPattern5D[2] << endl;
      cout << "avcharge:" << RingPattern5D[3] << endl; 
      */
      //TransformToEigenspace4D(RingPattern4D,TransformedRingPattern4D);
    } 
  
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
	  afReturn[i] = -2.0f;
	  continue;
        }

      //If the ring is complete we use the first PDF
      if(fCentroid>=0)
	{
	  Eigencoord_One=TransformedRingPattern5D[0];
	  Eigencoord_Two=TransformedRingPattern5D[1];
	  CentroidStatus=CentroidDefined;

	}
      //if not we use the second
      else
	{
	  Eigencoord_One=RingPattern4D[0];
	  Eigencoord_Two=RingPattern4D[1];
	  CentroidStatus=CentroidUndefined;
	}
      afReturn[i] = pParams->getCLValue(iId,CentroidStatus,Eigencoord_One,Eigencoord_Two);
    }
  
  return kTRUE;
}

// -----------------------------------------------------------------------------

void HPidAlgRingProperties::print(void) const
{
// Empty function for printing info of the algorithm

    printf("\tRingProperties algorithm\n");
}

Bool_t HPidAlgRingProperties::TransformToEigenspace5D(Double_t* OriginalData, Double_t* TransformedData)
{
  
  //Here we use the Function X2P which was built by the TPrincipal->MakePrincipals routine for 
  //Rings which have a well defined centroid! The transformation parameters and the variances
  //are hardwired for the moment, but later they have to be retrieved from a parameter container

  static Int_t    gNVariables = 5;

  for (Int_t i = 0; i < gNVariables; i++) {
    TransformedData[i] = 0;
    for (Int_t j = 0; j < gNVariables; j++)
      TransformedData[i] += (OriginalData[j] - pParams->get5DMean(j)) 
	* pParams->get5DEvec(j *  gNVariables + i) / pParams->get5DSigma(j);
  }

  return kTRUE;
}  

Bool_t HPidAlgRingProperties::TransformToEigenspace4D(Double_t* OriginalData, Double_t* TransformedData)
{
  //Here we call the Function X2P which was built by the TPrincipal->MakePrincipals routine for 
  //Rings which have no well defined centroid!
  
  static Int_t    gNVariables = 4;

  for (Int_t i = 0; i < gNVariables; i++) {
    TransformedData[i] = 0;
    for (Int_t j = 0; j < gNVariables; j++)
      //TransformedData[i] += (OriginalData[j] - gMeanValues[j]) 
      //* gEigenVectors[j *  gNVariables + i] / gSigmaValues[j];
      TransformedData[i] += (OriginalData[j] - pParams->get4DMean(j))
	*pParams->get4DEvec(j *  gNVariables + i) / pParams->get4DSigma(j);
  }

  return kTRUE;
}


