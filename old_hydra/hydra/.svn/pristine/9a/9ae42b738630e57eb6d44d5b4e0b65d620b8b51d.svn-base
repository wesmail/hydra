// @(#)$Id: hpidparticle.cc,v 1.16 2006-08-16 16:13:35 halo Exp $
//*-- Author : Marcin Jaskula 30/11/2002
//  Modified : Marcin Jaskula 11/11/2002
//             fExpMass added
//  Modified : Marcin Jaskula 01/12/2002
//             new methods getIndex(), getPidParticle()
//             new interface in getPidCandidate()
//  Modified : Marcin Jaskula 25/02/2003
//             new methods getTrackCand() and getKickTrack()
//  Modified : Marcin Jaskula 24/06/2003
//             fillMultiUsed method added

//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////////////////////////////////////
// This class has been redesigned also in the course of adaption to multiple tracking methods
// Documentation is provided at the individual function bodies and in the header file
//
// Tassilo Christ 05/07/2006
////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidParticle                                                               //
//                                                                            //
// Identified particle, output of HPidParticleFiller - last stage of PID      //
// analysis.                                                                  //
//                                                                            //
// fMassExp member defines the mass calulated from experimental parameters    //
// momentum and beta. The value of this memeber may have two artifical values://
// -1 (kMassExpInTLV) - means that fMassExp was used to build TLorentzVector  //
// -2 (kMassExpNoPhy) - the experimental beta was not physical one.           //
//                                                                            //
// Proper values of both masses: experimental and ideal can be got by methods://
// getMassExp(), getMassIdeal() which returns propper mass independently      //
// on the mass used to make TLorentzVector                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

using namespace std;
#include "hpidhitdata.h"
#include "hpidparticle.h"
#include "hpidcandidate.h"
#include "hpidtrackcand.h"

#include "hpidphysicsconstants.h"

#include "hades.h"
#include "hevent.h"
#include "hlinearcategory.h"
#include "hpidfl.h"
#include <TError.h>
#include <iostream>
#include "piddef.h"

// -----------------------------------------------------------------------------

ClassImp(HPidParticle)

// -----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This constructor takes as minimum input an index to the HPidCandidate-instance from which the 
// particle is made, an index indicating the type of momentum reconstruction, a vector with particle Ids
// for which weights where computed and a vector with these weights. The rest of the
// variables is optional. If useMassIdeal is kTRUE, then the ideal mass of the species which is 
// assigned to the particle is used to fill the TLorentzVector. If userDefSpecies is set, then
// the contents of the probability vector are stored but ignored and the assigned PID is taken from this 
// variable. In this case userWeight has to be set. Then the probability value for the assigned species is 
// set to the user-defined value.
//
//////////////////////////////////////////////////////////////////////////////////////////////////

HPidParticle::HPidParticle(HPidCandidate* pCandidate,Float_t* VectorOfWeights,
			   Bool_t useMassIdeal, Int_t userDefSpecies, Float_t userWeight):
  itsHitData(*(pCandidate->getTrackCandidate()->getHitData())),
  itsTrackData(*(pCandidate->getTrackCandidate()->getTrackData()))
{
  
  setDefault();


  //Allocation of dynamically managed memory
  //How many particles are we considering?
  nPossibleSpecies = pCandidate->getNParticles();

  //Each species is stored as a possible PID together with its weight
  possibleSpecies.Set(nPossibleSpecies,pCandidate->getParticleIds()->GetArray());
  assignedWeights.Set(nPossibleSpecies,VectorOfWeights);

  //Store index to hit collection and used momentum reconstruction
  nPidCandidateIndex = HPidFL::getCategory(catPidCandidate)->getIndex(pCandidate);
  setMomAlg(pCandidate->getMomAlg());
    
  //Set species and weight -> See documentation of this function
  setPidParams(0.0,userDefSpecies, userWeight); 

  //Get hit/tracking information 
  const HPidHitData* pHitData = getHitData();
  const HPidTrackData* pTrackData = getTrackData();


  Double_t tMomentum = pTrackData->fMomenta[getMomAlg()];
  Double_t tMass = -1.0;

  //Values are given in degree
  Double_t tTheta = pHitData->fMdcTheta;
  Double_t tPhi = pHitData->fMdcPhi;

  if(useMassIdeal) //we use the ideal mass of the particle species that was assigned to this particle above in setPidParams()
    {
      tMass=HPidPhysicsConstants::mass(getPid());
      kUsesIdealMass=kTRUE;
    }
  else //we trust in the momentum reconstruction
    {
      tMass = TMath::Sqrt(pTrackData->fMassSquared[momAlgIndex]);
    }

  SetXYZM(tMomentum * TMath::Sin(TMath::DegToRad()*tTheta) * //P-x
          TMath::Cos(TMath::DegToRad()*tPhi),
	  tMomentum * TMath::Sin(TMath::DegToRad()*tTheta) * //P-y
	  TMath::Sin(TMath::DegToRad()*tPhi),
	  tMomentum * TMath::Cos(TMath::DegToRad()*tTheta),  //P-z
	  tMass);                                            //Mass

}

// -----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This function is used to set either (A) a user speciefied particle ID and a corresponding weight 
// computed by any external method for this particle or (B) determine the decision from the PID supplied values. 
// In case (A) the user specifies a pid he whishes to be assigned to this particle. The probabilities are
// stored, but not USED to decide about the species then! 
// If the user omits the values for userDefSpecies and userWeight (B), then the assignment is done 
// according to the maximum weight in the assignedWeights TArrayF.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
void HPidParticle::setPidParams(Float_t fTV, Int_t userDefSpecies, Float_t userWeight)//OK
{
   if(userDefSpecies!=-99) //The user defines a species by hand
    {
      nAssignedPID=userDefSpecies;
      if(userWeight!=-99.0)
	{
	  fWeight=userWeight;
	}
      else
	{
	  ::Error("HPidParticle::SetPidParams()","User defined a species for this particle by hand, but no weight");
	  exit(-1);
	}
    }

  else //The user leaves it to the computed weights to decide about the assigned species
    {
      //Determine the biggest weight from the list of probabilities
      Float_t currentMaxWeight=0.0;
      for(Int_t i=0;i<nPossibleSpecies;i++)
	{
	  if(assignedWeights[i]>currentMaxWeight)
	    {
	      currentMaxWeight=assignedWeights[i];
	      nAssignedPID=possibleSpecies[i];
	      fWeight = currentMaxWeight;
	    }
	}
      //If there was NO particle with weight bigger than 0.0 the PID decision cannot be used - we assign "artifical" as PID
      if(currentMaxWeight==0.0){
        if( getTrackData()->nPolarity[momAlgIndex]<0){
	  nAssignedPID =HPidPhysicsConstants::artificialNeg();
        }
        else if (getTrackData()->nPolarity[momAlgIndex]>0){
	  nAssignedPID =HPidPhysicsConstants::artificialPos();
        }
      }
      //fTV=currentMaxWeight;
    }
   //setTestVal(fTV);
}

// -----------------------------------------------------------------------------

Float_t HPidParticle::getWeightForPID(Short_t pid) const //OK
{
  Int_t i = getSpeciesIndex(pid);
  if(i>-1) return assignedWeights[i];
  return -1.0;
}

Int_t HPidParticle::getSpeciesIndex(Short_t pid) const //OK
{
  for(Int_t i=0;i<nPossibleSpecies;i++)
    {if(possibleSpecies[i]==pid) return i;}
  return -1;
}

void HPidParticle::setDefault(void) //OK
{
  momAlgIndex=-1;

  nPossibleSpecies=0;

  nAssignedPID=-99;
  
  nPidCandidateIndex = -1;

  possibleSpecies.Set(0);

  assignedWeights.Set(0);
   
  fTestVal = 0.0f;
  
  fWeight  = 1.0f;
  
  fMomRescal = 1.0f;

  kUsesIdealMass=kTRUE;

  SetXYZT(0.0, 0.0, 0.0, 0.0);
}

// -----------------------------------------------------------------------------

void HPidParticle::Clear(Option_t *opt) //OK
{
// Clear all variables and frees dynamic memory!

  possibleSpecies.Set(0);
  assignedWeights.Set(0);
  //itsTrackData.Clear();
  //itsHitData.Clear();
}

// -----------------------------------------------------------------------------

void HPidParticle::print(void) const
{
// Print info about the particle

    printf("Particle  : %d \"%s\"\n", getPid(), HPidPhysicsConstants::pid(getPid()));
    printf("4Momentum : (%8.4f, %8.4f, %8.4f, %8.4f)\nMom. mag.: %8.4f MeV/c\n",
                X(), Y(), Z(), T(), P());
    printf("Theta/Phi : %8.4f  %8.4f\n", thetaDeg(), phiDeg());
    //printf("Sect./Sys : %d %d\n", sector(), nSystem);
    printf("R, Z, Pull: %8.4f  %8.4f  %8.4f\n", getR(), getZ(), getTrackData()->fPull);
    printf("Beta calc : %8.4f  Exp: %8.4f\n", Beta(), getBetaExp());
    printf("Mass ideal: %8.4f  Exp: %8.4f\n", getMassIdeal(), getMassExp());
    printf("TestVal   : %8.4f\n", fTestVal);
    printf("Weight    : %8.4f\n", fWeight);
}

// -----------------------------------------------------------------------------

HPidCandidate* HPidParticle::getPidCandidate(HCategory *pCat) const //OK
{
// Returns HPidCandidate object corresponding to nPidCandidateIndex (if exists)
// Works when pCat is set or gHades->getCurrentEvent() is accessible

    if(nPidCandidateIndex < 0)
        return NULL;

    return HPidFL::getPidCandidate(nPidCandidateIndex, pCat);
}

// -----------------------------------------------------------------------------

HCategory* HPidParticle::buildPidParticleCategory(void) //OK
{
// Static function for making the category HPidParticle

HCategory    *pCat;
HEvent       *pEvent;

    if((gHades == NULL) || ((pEvent = gHades->getCurrentEvent()) == NULL))
    {
        ::Error("HPidParticle::buildPidParticleCategory",
                    "Cannot access current event");

        return NULL;
    }

    if((pCat = pEvent->getCategory(catPidPart)) != NULL)
      {
	((HLinearCategory*)pCat)->setDynamicObjects(kTRUE);
        return pCat;
      }

    if((pCat = new HLinearCategory("HPidParticle", 1000)) == NULL)
    {
        ::Error("HPidParticle::buildPidParticleCategory",
                    "Cannot create new category");

        return NULL;
    }
    ((HLinearCategory*)pCat)->setDynamicObjects(kTRUE);
    pEvent->addCategory(catPidPart, pCat, "Pid");

    return pCat;
}

// -----------------------------------------------------------------------------


Int_t HPidParticle::getMostProbablePID(void) const //OK
{
  Float_t currentMax=0.0;  
  Int_t pid=-99;
  for(Int_t j =0;j<nPossibleSpecies;j++)
    {
      if(assignedWeights[j]>currentMax)
	{
	  currentMax=assignedWeights[j];
	  pid=possibleSpecies[j];
	}
    }
    if( !(currentMax>0.0)){ //no decision. Make artificial IDs
        if( getTrackData()->nPolarity[momAlgIndex]<0){
            pid =HPidPhysicsConstants::artificialNeg();
        }
        else if (getTrackData()->nPolarity[momAlgIndex]>0){
            pid =HPidPhysicsConstants::artificialPos();
        }
    }

  return pid;
}


// -----------------------------------------------------------------------------


Int_t HPidParticle::getIndex(HCategory *pCat) //OK
{
// Return index of the HPidParticle in the pCat or catPidPart if pCat == NULL

    if((pCat == NULL) && ((pCat = HPidFL::getCategory(
                            catPidPart, kFALSE)) == NULL))
    {
        Error("getIndex", "Cannot get category");
        return -2;
    }

    return pCat->getIndex(this);
}

// -----------------------------------------------------------------------------

HPidTrackCand* HPidParticle::getTrackCand(void) const //OK
{
// This method returns HPidTrackCand object from the corresponding
// HPidCandidate (when nPidCandidateIndex >= 0) or directly from catPidTrackCand
// when nPidCandidateIndex <= -1000
// (interface for HPidParticleFillerFromTrack)

    if(nPidCandidateIndex >= 0)
    {
    HPidCandidate *pCand;

        if((pCand = getPidCandidate()) == NULL)
        {
            Error("HPidParticle::getTrackCand()", "Cannot get HPidCandidate");
            return NULL;
        }

        if(pCand->getTrackCandIndex() < 0)
	  {
	    Error("HPidParticle::getTrackCand()","Track Candidate Index for PidCandidate is %d",pCand->getTrackCandIndex());
            return NULL;
	  }
        return pCand->getTrackCandidate();
    }

    if(nPidCandidateIndex <= kTrackCandOffset)
        return HPidFL::getTrackCand(kTrackCandOffset - nPidCandidateIndex);

    return NULL;
}


// -----------------------------------------------------------------------------

TLorentzVector HPidParticle::getRescaledVector(void) const //OK
{
TLorentzVector v = *this;

    if(fMomRescal != 1.0f)
    {
    TVector3 v3 = v.Vect();

        v3.SetMag(fMomRescal * v3.Mag());
        v.SetVectM(v3, M());
    }

    return v;
}


