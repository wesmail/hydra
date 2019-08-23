
//*-- Author : Jaroslav Bielcik 14/11/2002

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidAlgLeptonCand                                                          //
// dummy lepton cand finder alg                                               //
// Algoritm fill PDF 1 for rich-kickplane correlated                          //
// Using Rich-KickPlane information                                           //
////////////////////////////////////////////////////////////////////////////////

#pragma implementation

#include "hades.h"
#include "hruntimedb.h"
#include "hpidalgleptoncand.h"
#include "hpidtrackcand.h"
#include "hpidreconstructor.h"
#include "hpidphysicsconstants.h"
#include <hkicktrack.h>
#include <hrichhit.h>
#include <hmdcseg.h>
// -----------------------------------------------------------------------------

ClassImp(HPidAlgLeptonCand)

// -----------------------------------------------------------------------------

HPidAlgLeptonCand::HPidAlgLeptonCand(void)
                        : HPidAlgorithm("LeptonCand", algRich)
{
// Default constructor
}

// -----------------------------------------------------------------------------

HPidAlgLeptonCand::HPidAlgLeptonCand(Float_t fWeight)
                        : HPidAlgorithm("LeptonCand", algRich, fWeight)
{
// Default constructor with weight
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgLeptonCand::init(void)
{


    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgLeptonCand::reinit(void)
{
// Dummy method

    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgLeptonCand::finalize(void)
{
// Dummy method

    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgLeptonCand::calculatePDF(HPidTrackCand *pTrack,
            Float_t afReturn[], Short_t &nCatIndex)
{
  
  HRichHit* pRichHit;
  HKickTrack *pKickTrack;
  if(pTrack->getRingId()<0 || pTrack->getKickTrackId() < 0 ) {
    
    //cout<<"not a correlated with the ring or not full track"<<endl; 
    return kTRUE;
  }
  
  
  
  if((pKickTrack = pTrack->getKickTrack()) == NULL)
    {
      Error("calculatePDF", "Cannot get HKickTrack  for the track: %d",
	    pTrack->getKickTrackId());
      
      return kFALSE;
    } 
  
  if((pRichHit = pTrack->getRichHit()) == NULL)
    {
      Error("calculatePDF", "Cannot get HRichHit  for the track: %d",
	    pTrack->getKickTrackId());
      
      return kFALSE;
    }

  // now I have to get cluster info from MdcSeg that belongs to this KickTrack
   
  //now I retrieve info from ring
  Float_t fCharge = pKickTrack->getCharge();
  Int_t   iId,i;
  
  
  for(i = 0; i < pRec->particlesNumber(); i++){
    iId = pRec->getParticleId(i);  
    if(fCharge != HPidPhysicsConstants::charge(iId))
      {
	afReturn[i] = 0.0f;
	continue;
      }
    
    if(! HPidPhysicsConstants::isFake(iId)){
      if(iId!=2&&iId!=3) {
	afReturn[i] = 0.0f;
	continue;
      }
      
      //FOR THE MOMENT DUMMY    
      
      afReturn[i] = 1.0; // rich correlated hits has PDF 1.0
    }
    
  }
  return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgLeptonCand::calculateCL(HPidTrackCand *pTrack,
            Float_t afReturn[], Short_t &nCatIndex)
{ 
  
  
  HRichHit* pRichHit;
  HKickTrack *pKickTrack;
  if(pTrack->getRingId()<0 || pTrack->getKickTrackId() < 0 ) {
    //not a correlated with the ring or not full track 
    return kTRUE;
  }
  

  
  if((pKickTrack = pTrack->getKickTrack()) == NULL)
    {
      Error("calculatePDF", "Cannot get HKickTrack  for the track: %d",
	    pTrack->getKickTrackId());
      
      return kFALSE;
    } 
  
  if((pRichHit = pTrack->getRichHit()) == NULL)
    {
      Error("calculatePDF", "Cannot get HRichHit  for the track: %d",
	    pTrack->getKickTrackId());
      
      return kFALSE;
    }

  // now I have to get cluster info from MdcSeg that belongs to this KickTrack
   
  //now I retrieve info from ring
  Float_t fCharge = pKickTrack->getCharge();
  Int_t   iId,i;
  
  
  for(i = 0; i < pRec->particlesNumber(); i++){
    iId = pRec->getParticleId(i);  
    
    // printf("i=%i iId=%i\n",i,iId); 
    if(fCharge != HPidPhysicsConstants::charge(iId))
      {
	afReturn[i] = -2.0f;
	continue;
      }
    
    if(! HPidPhysicsConstants::isFake(iId))
      if(iId!=2||iId!=3) {
	afReturn[i] = -1.0f;
	continue;
      }
    
    afReturn[i] =1.0; 
    
  }

    return kTRUE;
}

// -----------------------------------------------------------------------------

void HPidAlgLeptonCand::print(void) const
{
// Empty function for printing info of the algorithm
printf("\tLeptCand algorithm\n");
}






