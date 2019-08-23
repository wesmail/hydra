// @(#)$Id: hpidgeanttrackset.cc,v 1.27 2009-10-15 14:42:28 halo Exp $
//*-- Author : Marcin Jaskula 11/11/2002
//  Modified : Marcin Jaskula 02/03/2003
//             get rid of TObject
//  Modified : Marcin Jaskula 22/06/2003
//             getGeantKine() improved to work with a filtered branch
//  Modified : Stefano Spataro 18/07/2005
//             libTriggerUtil data members (now protected) changed into member funtions

//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidGeantTrackSet                                                          //
//                                                                            //
// Geant tracks matched in HPidTrackCand                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
using namespace std;
#include "TArrayI.h"
#include "hpidgeanttrackset.h"
#include "hshowerhittof.h"
#include "hshowerhittoftrack.h"
#include "hmetamatch.h"
#include "hpidfl.h"
#include "hlinearcategory.h"
#include "hpidtrackcand.h"
#include "hmdctrkcand.h"
#include "hmdcsegsim.h"
#include "hrichhitsim.h"
#include "hrichhitIPUSim.h"
#include "htofhitsim.h"
#include "htofclustersim.h"
#include "TError.h"
#include <map>
#include "hgeantdef.h"
#include "hgeantkine.h"
#include "hgeanttof.h"
#include "hpidphysicsconstants.h"
// -----------------------------------------------------------------------------

ClassImp(HPidGeantTrackSet)
  
// -----------------------------------------------------------------------------
Int_t HPidGeantTrackSet::nullparent=0;

HPidGeantTrackSet::HPidGeantTrackSet(const HPidGeantTrackSet& source)
{
  isSorted = source.isSorted;

  sNCorrTrackIds = source.getNCorrelatedTrackIds();

  nRICHTracks = source.getNRichTracks();
  nRICHIPUTracks = source.getNRichIPUTracks();
  nInnerMdcTracks = source.getNInnerMdcTracks();
  nOuterMdcTracks = source.getNOuterMdcTracks();
  nShowerTracks = source.getNShowerTracks();
  nTOFTracks = source.getNTOFTracks();
  bIsLepFromPrimary = source.getPrimaryFlag();

  correlatedTrackIds.Set(sNCorrTrackIds);
  correlationFlags.Set(sNCorrTrackIds);
  ProcessIds.Set(sNCorrTrackIds);

  ParentIds.Set(sNCorrTrackIds);
  Parents.Set(sNCorrTrackIds);
  GrandParentIds.Set(sNCorrTrackIds);
  GrandParents.Set(sNCorrTrackIds);


  GenInfo.Set(sNCorrTrackIds);
  GenInfo1.Set(sNCorrTrackIds);
  GenInfo2.Set(sNCorrTrackIds);
  GenWeight.Set(sNCorrTrackIds);

  VertexX.Set(sNCorrTrackIds);
  VertexY.Set(sNCorrTrackIds);
  VertexZ.Set(sNCorrTrackIds);


  GeantPIDs.Set(sNCorrTrackIds);
  MediumIds.Set(sNCorrTrackIds);
  
  GeantMomX.Set(sNCorrTrackIds);
  GeantMomY.Set(sNCorrTrackIds);
  GeantMomZ.Set(sNCorrTrackIds);
  
  
  ShowerWeights.Set(sNCorrTrackIds);	 
  TOFWeights.Set(sNCorrTrackIds);	 
  RICHWeights.Set(sNCorrTrackIds);	 
  RICHIPUWeights.Set(sNCorrTrackIds); 
  InnerMDCWeights.Set(sNCorrTrackIds);
  OuterMDCWeights.Set(sNCorrTrackIds);
 
  hasHitInShower.Set(sNCorrTrackIds);
  hasHitInTOF.Set(sNCorrTrackIds);

  
  //These arrays have to have a default value of 0 as -99 is theoretically a valid number
  for(Int_t i=0;i<sNCorrTrackIds;i++)
    {
      correlatedTrackIds[i]=source.getGeantTrackID(i);
      correlationFlags[i]=source.getCorrelationFlag(i);
      ProcessIds[i]=source.getGeantProcessID(i);

      ParentIds[i]=source.getGeantParentID(i);
      Parents[i]=source.getGeantParentTrackID(i);
      GrandParentIds[i]=source.getGeantGrandParentID(i);
      GrandParents[i]=source.getGeantGrandParentTrackID(i);

      GenInfo[i]=source.getGenInfo(i);
      GenInfo1[i]=source.getGenInfo1(i);
      GenInfo2[i]=source.getGenInfo2(i);
      GenWeight[i]=source.getGenWeight(i);

      VertexX[i]=source.getGeantVertexX(i);
      VertexY[i]=source.getGeantVertexY(i);
      VertexZ[i]=source.getGeantVertexZ(i);

      GeantPIDs[i]=source.getGeantPID(i);
      MediumIds[i]=source.getGeantMediumID(i);
      

      GeantMomX[i]=source.getGeantMomX(i);
      GeantMomY[i]=source.getGeantMomY(i);
      GeantMomZ[i]=source.getGeantMomZ(i);
      
      
      ShowerWeights[i]=source.getShowerWeight(i);	 
      TOFWeights[i]=source.getTOFWeight(i);	 
      RICHWeights[i]=source.getRichWeight(i);	 
      RICHIPUWeights[i]=source.getRichIPUWeight(i);	 
      InnerMDCWeights[i]=source.getInnerMdcWeight(i);	 
      OuterMDCWeights[i]=source.getOuterMdcWeight(i);	 
      
      hasHitInShower[i]=source.hasHitInShower[i];
      hasHitInTOF[i]=source.hasHitInTOF[i];
      
    }
}

// Set all members to 0/-1 and set the default size of the arrays
void HPidGeantTrackSet::reset(void)//OK
{
  Clear();
}

// -----------------------------------------------------------------------------


//returns kTRUE when the flag has the detector bit set
Bool_t HPidGeantTrackSet::wasSeenInDetector(const UInt_t flag, const UInt_t detector,Bool_t verbose) const //OK
{

  if(verbose)
    {
      cout << "Testing function input : detector=" <<detector<<" flag="<<flag <<endl;
      cout << "Output: " << (detector & flag)<<endl;
    }
  if((detector & flag)==detector)
    {
      if(verbose){
	cout <<" same" <<endl;
      }
      return kTRUE;
    }
  if(verbose)
    {cout << "detector: " << detector<<endl;
    cout << "flag:" <<flag << endl;
    }
  return kFALSE;
}

Short_t HPidGeantTrackSet::getRichTrack(UInt_t iIdx) const //OK
{
// Get iIdx-th track from list of correlated tracks that was seen in RICH
  if( iIdx >= nRICHTracks) 
    {
      ::Error("HPidGeantTrackSet::getRichTrack","index for rich track greater than number of contributing particles in ring");
      return kOutOfBound;

    }
  UInt_t counter = 0;

  for(Int_t i = 0;i < sNCorrTrackIds; i++)
    {
      if(wasSeenInDetector(correlationFlags[i], kSeenInRICH))
	{
	  if (counter==iIdx) return correlatedTrackIds[i];
	  counter++;
	}
    }
  return kOutOfBound;
}



// -----------------------------------------------------------------------------

Short_t HPidGeantTrackSet::getRichIPUTrack(UInt_t iIdx) const //OK
{
// Get iIdx-th track from list of correlated tracks that was seen in RICH-IPU
  if( iIdx >= nRICHIPUTracks)
     {
      ::Error("HPidGeantTrackSet::getRichIPUTrack","index for rich ipu track greater than number of contributing particles in IPU ring");
      return kOutOfBound;
    }


  UInt_t counter = 0;

  for(Int_t i = 0;i < sNCorrTrackIds; i++)
    {
      if (wasSeenInDetector(correlationFlags[i], kSeenInRICHIPU))
	{
	  if (counter==iIdx) return correlatedTrackIds[i];
	  counter++;
	}
    }
  return kOutOfBound;
}


// -----------------------------------------------------------------------------

Short_t HPidGeantTrackSet::getInnerMdcTrack(UInt_t iIdx) const //OK
{
// Get iIdx-th track from list of correlated tracks that was seen in the inner MDC
     if( iIdx >= nInnerMdcTracks)
     {
       ::Error("HPidGeantTrackSet::getInnerMdcTrack","index for inner mdc track greater than number of contributing particles in inner mdc");
       return kOutOfBound;
     }

  UInt_t counter = 0;

  for(Int_t i = 0;i < sNCorrTrackIds; i++)
    {
      if (wasSeenInDetector(correlationFlags[i], kSeenInInnerMDC))
	{
	  if (counter==iIdx) return correlatedTrackIds[i];
	  counter++;
	}
    }
  return kOutOfBound;
}

// -----------------------------------------------------------------------------

Short_t HPidGeantTrackSet::getOuterMdcTrack(UInt_t iIdx) const//OK
{
// Get iIdx-th track from list of correlated tracks that was seen in the outer MDC
  if( iIdx >= nOuterMdcTracks) 
    {
       ::Error("HPidGeantTrackSet::getOutMdcTrack","index for outer mdc track greater than number of contributing particles in outer mdc");
       return kOutOfBound;
    }
  UInt_t counter = 0;

  for(Int_t i = 0;i < sNCorrTrackIds; i++)
    {
      if (wasSeenInDetector(correlationFlags[i], kSeenInOuterMDC))
	{
	  if (counter==iIdx) return correlatedTrackIds[i];
	  counter++;
	}
    }
  return kOutOfBound;
}

Short_t HPidGeantTrackSet::getTOFTrack(UInt_t iIdx) const//OK
{
  // Get iIdx-th track from list of correlated tracks that was seen in TOF
  if( iIdx >= nTOFTracks)
    {
      ::Error("HPidGeantTrackSet::getTOFTrack","index for TOF track greater than number of contributing particles in TOF");
      return kOutOfBound;
    }

  UInt_t counter = 0;

  for(Int_t i = 0;i < sNCorrTrackIds; i++)
    {
      //This has to be checked in this way because there is not kSeenInTOF flag put into the correlation flag
      //in order to have ONE flag for all meta detectors! The information about the system is put into hasHitInTOF
      if (wasSeenInDetector(correlationFlags[i], kSeenInMETA) && hasHitInTOF.At(i)>0)
	{
	  if (counter==iIdx) 
	    {
	      return correlatedTrackIds[i];
	    }
	  counter++;
	}
    }
  return kOutOfBound;
}


Short_t HPidGeantTrackSet::getShowerTrack(UInt_t iIdx) const//OK
{
  // Get iIdx-th track from list of correlated tracks that was seen in Shower
  
  if( iIdx >= nShowerTracks) 
    {
      ::Error("HPidGeantTrackSet::getShowerTrack","index for Shower track greater than number of contributing particles in Shower");
      return kOutOfBound;
    }

  UInt_t counter = 0;

  for(Int_t i = 0;i < sNCorrTrackIds; i++)
    {
      //This has to be checked in this way because there is not kSeenInTOF flag put into the correlation flag
      //in order to have ONE flag for all meta detectors! The information about the system is put into hasHitInTOF
      if (wasSeenInDetector(correlationFlags[i], kSeenInMETA) && hasHitInShower.At(i)>0)
	{
	  if (counter==iIdx) 
	    {
	      return correlatedTrackIds[i];
	    }
	  counter++;
	}
    }
  return kOutOfBound;
}



Short_t HPidGeantTrackSet::getMetaTrack(UInt_t iIdx) const//Well
{
  // Get iIdx-th track from list of correlated tracks that was seen in META
  if(nTOFTracks>0 && nShowerTracks>0) //We have hits in both meta detectors!
    {
      //Here we make a common search over both Meta Detectors
      UInt_t counter = 0;

      if(iIdx >= getNMetaTracks())
	{
	  ::Error("HPidGeantTrackSet::getMEtaTrack",
		  "index for Meta track greater than number of contributing particles in Meta");
	  return kOutOfBound;
	}

      for(Int_t i = 0;i < sNCorrTrackIds; i++)
	{
	  if (wasSeenInDetector(correlationFlags[i], kSeenInMETA))
	    {
	      if (counter==iIdx) return correlatedTrackIds[i];
	      counter++;
	    }
	}
      return kOutOfBound;
    }
  return kOutOfBound;
}


// -----------------------------------------------------------------------------

HGeantKine* HPidGeantTrackSet::getGeantKine(Int_t iTrack, HCategory *pCat)//OK
{
// Return HGeantKine object corresponding to iTrack from pCat.
// If the pCat is not set (NULL) catGeantKine category from gHades is used.

    if(iTrack <= 0)
        return NULL;

    if((pCat == NULL) && ((pCat = HPidFL::getCategory(
                            catGeantKine, kFALSE)) == NULL))
    {
        ::Error("HPidGeantTrackSet::getGeantKine", "No catGeantKine category");

        return NULL;
    }

HGeantKine *pKine;
Int_t       iT, iMin, iMax, iC, iP;

    if((iMax = pCat->getEntries()) <= 0)
        return NULL;

    //check whether kine category has enough members at all to accomodate such a high track ID
    if((iMax > iTrack - 1)
            && (pKine = (HGeantKine *) pCat->getObject(iTrack - 1)) != NULL)
      {
	//iTrack is the geant track id! Those are sorted (well, they should be) and start with 1
        pKine->getParticle(iT, iP);

	//If the track number reported by the found kine object is the same that we ask for 
	//this means that the ordering is done correctly in catGeantKine
        if(iT == iTrack)
	  return pKine;
      }
    
    // fast searching for the proper track. The assumption: tracks are sorted!
    // implements a logarithmic search for the right object - very smart Mr. Jaskula :-)
    iMin = -1;

    do
    {
        iC = (iMax + iMin) / 2;
        if((pKine = (HGeantKine *) pCat->getObject(iC)) == NULL)
        {
            ::Error("HPidGeantTrackSet::getGeantKine",
                    "Hole in HGeantKine branch (%d)", iC);

            return NULL;
        }

        pKine->getParticle(iT, iP);
        if(iT == iTrack)
            return pKine;

        if(iTrack < iT)
            iMax = iC;
        else
            iMin = iC;
    } while(iMax - iMin > 1);

    return NULL;
}

void HPidGeantTrackSet::swapVectors(Int_t i, Int_t j)//OK
{
  swap(i,j,correlatedTrackIds);
  swap(i,j,correlationFlags);
  swap(i,j,ProcessIds);

  swap(i,j,ParentIds);
  swap(i,j,Parents);
  swap(i,j,GrandParentIds);
  swap(i,j,GrandParents);

  swap(i,j,GenInfo);
  swap(i,j,GenInfo1);
  swap(i,j,GenInfo2);
  swap(i,j,GenWeight);

  swap(i,j,VertexX);
  swap(i,j,VertexY);
  swap(i,j,VertexZ);

  swap(i,j,GeantPIDs);
  swap(i,j,MediumIds);

  swap(i,j,GeantMomX);
  swap(i,j,GeantMomY);
  swap(i,j,GeantMomZ);

  swap(i,j,ShowerWeights);
  swap(i,j,TOFWeights);
  swap(i,j,RICHWeights);
  swap(i,j,RICHIPUWeights);
  swap(i,j,InnerMDCWeights);
  swap(i,j,OuterMDCWeights);

  swap(i,j,hasHitInShower);
  swap(i,j,hasHitInTOF);
}

void HPidGeantTrackSet::swap(Int_t i, Int_t j, TArrayI& array)//OK
{
  Int_t tmp = array[i];
  array[i]=array[j];
  array[j]=tmp;
}

void HPidGeantTrackSet::swap(Int_t i, Int_t j, TArrayF& array) //OK
{
  Float_t tmp = array[i];
  array[i]=array[j];
  array[j]=tmp;
}

//This function implements a three-step sorting algorithm
void HPidGeantTrackSet::sortsmart(void) //OK
{
  //1. All particles which where seen in more than one detector are arranged BEFORE any particle that was seen in
  //   only one single subsystem. This is required because lonely hits in META have a HIGH bit (64) and would therefore
  //   be ranked higher than a combination of the three inner detectors. Anyway we mostly care for CORRELATIONS
  //   between detectors, so we bring the relevant particle indices to the front of the array.
  bringCorrelatedDetectorsToFront(); 

  //2. We now sort the correlation flags of those particles that where seen in more than one detector according
  //   to the size of the correlation flag (documented in HPidGeantTrackSet::fillFromMetaMatch()
  sortCorrelatedDetectors();

  //3. The last step is to sort the remaining flags of particles which where only seen in one subsystem
  sortSingleDetectors();

  isSorted=kTRUE;
}


//Sort all particles which where seen in more than one subsystem to the front - this does not perform sorting by size yet!
//See HPidGeantTrackSet::sortCorrelatedDetectors()!
void HPidGeantTrackSet::bringCorrelatedDetectorsToFront() //OK
{ 
  //We loop over all contributing particles
  for(Int_t i = 0;i<sNCorrTrackIds;i++)
    {
      //if the current corrflag indicates that this particle was seen in MORE THAN ONE subsystem
      //we do not touch it
      if(!isSingleDetector(correlationFlags[i])) 
	{
	  continue;
	}

      //If the above condition is NOT true we have found a particle in ONE AND ONLY ONE single detector
      //We need to swap it with the NEXT particle that has a hit in MORE THAN ONE subsystem
      else 
	{
	  //j=i+1 ensures that we swap only with particles coming LATER in the array
	  for(Int_t j=i+1; j<sNCorrTrackIds;j++) 
	    {
	      //We want to swap ONLY with particles that have a signal in multiple detectors
	      if(!isSingleDetector(correlationFlags[j]))
		{
		  swapVectors(i,j);	    
		  break;
		}
	      //After this the current position in the outer loop (i) is filled with a particle with 
	      //correlation in at least two subsystems! At position j there is now a particle with
	      //a hit in one single detector. This is the FIRST position in the array where we have
	      //encountered a correlated particle. This means that until position j there are no
	      //more correlated particles.
	    }
	}
    }
}

//This function assumes that all particles that where seen in more than one detector are brought to the
//front of the array. It stops as soon as there is a particle that was seen in one single subsystem
void HPidGeantTrackSet::sortCorrelatedDetectors() //OK
{
  
  Short_t currentMaxPosition=0;      //Position of the biggest correlation flag in the array
  Int_t currentMax=0;                //Biggest correlation flag in the array

  for(Int_t i = 0;i<sNCorrTrackIds;i++)
    {
      if(isSingleDetector(correlationFlags[i]))
	{
	  //The current detector is a single one so we reached the particles having a signal in only one subsystem - quit!
	  break; 
	}

      //We assume that at the current position we already have the biggest correltion flag
      currentMaxPosition=i;
      currentMax=correlationFlags[i];

      //We then loop over all later ones and check whether they are greater
      for(Int_t j=i+1;j<sNCorrTrackIds;j++)
	{
	  if(isSingleDetector(correlationFlags[j])) 
	    {
	      //We have reached the singularly correlated detectors - those we do not want to bring to the front - quit!
	      break;
	    }

	  //The correlation flag of a "later" particle is bigger than the one of the current particle - we note this but
	  //keep searching because there might be even bigger flags later
	  if(correlationFlags[j]>currentMax )
	    {
	      currentMaxPosition=j;            //Store position and value of the currently biggest correlation flag
	      currentMax=correlationFlags[j];
	    }
	}

      //Here we know the position of the biggest MULTI-DETECTOR correlation flag BEHIND the current particle!
      //As we start from the first one and always bring the biggest one to the front. We can safely swap the current
      //particle and the most correlated one. This will ensure a descending order of correlation flags.
      if(i!=currentMaxPosition) swapVectors(i,currentMaxPosition);
    }
}

//This function assumes that all the particles that showed up in exactly one subsystem are located at the
//end of the array. It sorts them according to the correlation flag assigned to the corresponding detector
void HPidGeantTrackSet::sortSingleDetectors() //OK
{ 

  //Position and size of the current maximum
  Short_t currentMaxPosition=0;    
  Int_t currentMax=0;
  
  //Loop over all particles that contributed
  for(Int_t i = 0;i<sNCorrTrackIds;i++)
    {
      //If a correlation flag indicates that this particle was seen in more than one detector 
      //this function is not interested in it.
      if(!isSingleDetector(correlationFlags[i])) 
	{
	  continue;
	}
      
      //If the above condition is false we have found a particle that was seen in exactly one subsystem.
      //We store the position and the size of the current maximum.
      currentMaxPosition=i;
      currentMax=correlationFlags[i];

      //We search in the array at all LATER positions because our sorting algorithm has brought all correlation
      //flags into descending order by always putting the biggest corrflag to the front
      for(Int_t j=i+1;j<sNCorrTrackIds;j++)
	{
	  if(correlationFlags[j]>currentMax )
	    {
	      //There is a bigger correlation flag at a later position - we do not know yet whether it is the 
	      //biggest one, so we temporary store its position until we find another one which is still bigger
	      currentMaxPosition=j;
	      currentMax=correlationFlags[j];
	    }
	}

      //Here we know the position of the biggest unsorted correlation flag which is bigger than the one we
      //are currently investigating. We can swap these two particles
      if(i!=currentMaxPosition) swapVectors(i,currentMaxPosition);
    }
}


Bool_t HPidGeantTrackSet::isSingleDetector(UInt_t flag) //OK
{
  if(flag==kSeenInRICH ||
     flag==kSeenInRICHIPU || 
     flag==kSeenInInnerMDC ||
     flag==kSeenInOuterMDC ||
     flag==kSeenInMETA) //Exactly seen in ONE detector!
    return kTRUE;

  return kFALSE;
}
  

Bool_t HPidGeantTrackSet::getMomentumComponents(Float_t& momx, Float_t& momy, Float_t& momz, Int_t index)//OK
{
  if(index>sNCorrTrackIds-1){
    ::Error("HPidGeanTrackSet::getMomentumComponents","requested momenta for particle beyond index!");
    momx=-1.0;
    momy=-1.0;
    momz=-1.0;
    return kFALSE;
  }
  momx=GeantMomX[index];
  momy=GeantMomY[index];
  momz=GeantMomZ[index];
  return kTRUE;
}

//Get momentum for the i-th particle that contributed
Float_t HPidGeantTrackSet::getTotalMomentum(Int_t index) const //OK
{
  if(index>sNCorrTrackIds-1){
    ::Error("HPidGeantTrackSet::getTotalMomentum","requested momentum for particle beyond index!");
    return kFALSE;
  }
  Float_t totalMomentum=GeantMomX[index]*GeantMomX[index]+GeantMomY[index]*GeantMomY[index]+GeantMomZ[index]*GeantMomZ[index];
  return sqrt(totalMomentum);
}

void HPidGeantTrackSet::fillFromMetaMatch(HMetaMatch* pMetaMatch) //OK
{
  ///////////////////////////////////////////////////////////////////////////////////////
  //
  // Each track Id found in any of the hits is added once (and only once)
  // to the list of correlated tracks. We store the degree of correlation right 
  // from the beginning in the datamembers correlatedTrackIds and correlationFlags. 
  // Also the geant PID, parent and process ID momentum, medium and system are 
  // stored in the corresponding arrays at the position corresponing to the position 
  // of the added particle. The common tracks are implicitly ranked by their degree of 
  // commonness using the following numerical code mapped to user-friendly names:
  //
  // kTrackNotSet         = 0x00,  // This object does not contain valid data
  // kSeenInRICHIPU       = 0x01,  // RICH IPU has seen this trackid
  // 
  // kSeenInOuterMDC      = 0x02,  // Outer Mdc has seen this trackid
  // kSeenInRICH          = 0x04,  // RICH has seen this trackid
  // kSeenInInnerMDC      = 0x08,  // Inner Mdc has seen this trackid
  // 
  // kSeenInTOF           = 0x10,  // TOF has seen this trackid
  // kSeenInShower        = 0x20,  // Shower has seen this trackid
  // kSeenInMETA          = 0x40,  // One of the Meta dets has seen this trackid
  //
  // Thereby META detectors are ranked highest because there is no way to perform 
  // particle identification without a hit in META. The second-most important 
  // piece of information is the inner MDC segment because together with META data
  // it allows momentum computation with KICKPLANE
  // The third highest numerical code is given to the RICH ring because together with
  // KICKTRACKS one can do lepton analysis if a ring is present.
  // Outer MDC segments are given the lowest number because they are not essential
  // for candidate searching. They only allow to apply high-resolution tracking.
  //
  ///////////////////////////////////////////////////////////////////////////////////////

  Bool_t debugoutput=kFALSE;

  //Initialize all pointers
  HMdcSegSim* pInnerMdcSeg=NULL;
  HMdcSegSim* pOuterMdcSeg=NULL;

  HRichHitSim* pRichHit=NULL;
  HRichHitIPUSim* pRichHitIPU=NULL;

  HShowerHitTofTrack* pShwTof = NULL;
  HTofHitSim* pTofHitSim=NULL;
  HTofClusterSim* pTofClsSim=NULL;
  HMdcTrkCand* pTrkCand = NULL;

  reset();

  Int_t i=0; 
  
  if(debugoutput) cout << "Starting to fill geant info!"<<endl;

  //We start with the outermost detectors TOF and SHOWER
  if(pMetaMatch->getSystem()==0)
    {  
      if(debugoutput) cout << "System is ShowerTofino";

      pShwTof = HPidFL::getShowerHitTofTrack(pMetaMatch->getShowerHitInd());

      if(pShwTof != NULL)
      {
          Int_t nTr = pShwTof->getNTracks();
	  for(Int_t i = 0; i < nTr; i ++)
	  {
	      if(pShwTof->getTrack(i) >= 0 || pShwTof->getTrack(i) == gHades->getEmbeddingRealTrackId())
	      {
		  nShowerTracks++;
		  //Here we add the index to the unique list of correlated Ids of this geanttrackset
		  addToContributingTracks(pShwTof->getTrack(i),kSeenInShower);

		  //... store which weight this particular track had in this detector hit
		  addTrackWeightToDetector(1.0/nTr,pShwTof->getTrack(i),kSeenInShower);

		  if(debugoutput&&sNCorrTrackIds) cout <<"Geant Track id in Shower: " << pShwTof->getTrack(i)<<endl;
	      }
	      else
	      {
		  ::Error("HPidGeantTrackSet::fillFromMetaMatch","Shower hit does not have valid geant track id");
	      }
	  } // end loop
      } else {
	  ::Error("HPidGeantTrackSet::fillFromMetaMatch","System 0 marked in MetaMatch object, but no Hit with corresponding index");
      }
    }
  
  if(pMetaMatch->getSystem()==1) 
    {  
      if(debugoutput) cout << "System is Tof" << endl;
      Int_t nClsSize = (Int_t)pMetaMatch->getTofClusterSize();
      
      //First check what we have from metamach - tofhit or tofcluster		
      Int_t nTrc0=-1;
      Int_t nTrc1=-1;
      
      if( nClsSize <= 0 )
	{
	  ////No tofcluster, take htofhitsim category
	  pTofHitSim = HPidFL::getTofHitSim(pMetaMatch->getMetaHitInd());
	  if(NULL!=pTofHitSim)
	    {
	      //Get track numbers
	      nTrc0 = pTofHitSim ->getNTrack1();
	      nTrc1 = pTofHitSim ->getNTrack2();

	      if(nTrc1==-1){nTrc1=nTrc0;}  // compatibility to old tofdigitizer


	      //-------------------------------------------------
	      // find the first track ID entering the TOF
	      // depending on storeFirstTrack flag (last argument)
	      // if replacing of tof track number is used
	      //        0 = realistic (secondaries included)
	      //        1 primary particle is stored
	      //        2 (default) the first track number entering the tof in SAME SECTOR is stored
	      //        3 as 2 but condition on SAME SECTOR && MOD
	      //        4 as 2 but SAME SECTOR && MOD && ROD
              Int_t mode=2;
	      Int_t count    =0;
              Int_t tempTrack;
              // always for first track
	      tempTrack=findFirstHitInTof(nTrc0,&count,mode);
	      if (nTrc0 != nTrc1){
                  // only if both numbers are not the same
		  count    =0;
		  nTrc1=findFirstHitInTof(nTrc1,&count,mode);
	      }
	      else {nTrc1=tempTrack;}
	      nTrc0=tempTrack;
	      //-------------------------------------------------

	      if( nTrc0 == nTrc1 && ( nTrc0 >0 || nTrc0==gHades->getEmbeddingRealTrackId() ))
	        {
                  // single hits
		  nTOFTracks++;
		  addToContributingTracks(nTrc0,kSeenInTOF);
		  addTrackWeightToDetector(1.0,nTrc0,kSeenInTOF);
		} 
	      if( nTrc0 != nTrc1 && ( nTrc1 >0 || nTrc1==gHades->getEmbeddingRealTrackId() ))
	        {
		  // Double_t hits
		  nTOFTracks++;
		  addToContributingTracks(nTrc0,kSeenInTOF);
		  addTrackWeightToDetector(0.5,nTrc0,kSeenInTOF);
		   nTOFTracks++;
		  addToContributingTracks(nTrc1,kSeenInTOF);
		  addTrackWeightToDetector(0.5,nTrc1,kSeenInTOF);
		  //The weight computation is bad! The weights cannot (in sum) be greater than 1!
#warning The computation of track weights in TOF needs to be checked
		}
	    }
	  else
	    {
	      ::Error("HPidGeantTrackSet::fillFromMetaMatch","System 1 marked in MetaMatch object, but no Hit with corresponding index");
	    }
	}
      else if ( nClsSize >= 1 )
	{
	  //tofclustersim
	  pTofClsSim = HPidFL::getTofClusterSim(pMetaMatch->getMetaHitInd());
	  if(NULL!=pTofClsSim)
	    {
		std::map<int,int> m;             // make map <trackID,ntimes>
		std::map<int,int>::iterator pos; // make iterator over map

		//-------------------------------------------------
		// count number of tracks for calulating weights
                // properly
		for(Int_t i=0;i<pTofClsSim->getNParticipants()&&i<3;i++)
		{
		    //Get track numbers
		    nTrc0 = pTofClsSim ->getNTrack1(i);
		    nTrc1 = pTofClsSim ->getNTrack2(i);
		    if(nTrc0==-1 && nTrc1==-1)
		      {
			continue;
		      }

		    if(nTrc1==-1){nTrc1=nTrc0;}  // compatibility to old tofdigitizer

		    //-------------------------------------------------
		    // find the first track ID entering the TOF
		    // depending on storeFirstTrack flag (last argument)
		    // if replacing of tof track number is used
		    //        0 = realistic (secondaries included)
		    //        1 primary particle is stored
		    //        2 (default) the first track number entering the tof in SAME SECTOR is stored
		    //        3 as 2 but condition on SAME SECTOR && MOD
		    //        4 as 2 but SAME SECTOR && MOD && ROD
		    Int_t mode =2;
		    Int_t count=0;


		    nTrc0=findFirstHitInTof(nTrc0,&count,mode);
                    nTrc1=findFirstHitInTof(nTrc1,&count,mode);

		    if (nTrc0 != nTrc1){
                        // Double_t hit, add both tracks to map
			if((pos=m.find(nTrc0))!=m.end()){
                            // trackID exists count up ntimes
			    pos->second ++;
			} else { // insert new trackID
			    m.insert(make_pair(nTrc0,1));}

			if((pos=m.find(nTrc1))!=m.end()){
			    // trackID exists count up ntimes
			    pos->second ++;
			} else { // insert new trackID
			    m.insert(make_pair(nTrc1,1));}
		    }
		    else {
			// single hit , add only one track
			if((pos=m.find(nTrc0))!=m.end()){
			    pos->second ++;
			} else {m.insert(make_pair(nTrc0,1));}
		    };
		    //-------------------------------------------------
		}
		//-------------------------------------------------

		//-------------------------------------------------
		// calulate the total sum of multiplier hits
                // for normalization of weights
		Int_t nTimesSum=0;
		for(pos = m.begin(); pos != m.end(); ++pos){
		    nTimesSum+=pos->second;
		}
		//-------------------------------------------------

		//-------------------------------------------------
		// fill tracks and weights of the complete cluster
		for(pos = m.begin(); pos != m.end(); ++pos){

		    nTOFTracks++;
		    addToContributingTracks(pos->first,kSeenInTOF);
		    addTrackWeightToDetector(pos->second/(Float_t)(nTimesSum),pos->first,kSeenInTOF);
		}
		//-------------------------------------------------

	    }
	  else
	    {
	      ::Error("HPidGeantTrackSet::fillFromMetaMatch","System 1 marked in MetaMatch object, but no Hit with corresponding index");
	    }
	}
      else 
	{
	  Warning("fillFromMetaMatch",
		  "What should I do with cluster size=%d?", nClsSize);
	}
      if(nTOFTracks==0)
	{
	  ::Error("fillFromMetaMatch","Should be at least one track in TOF!");
	}
    }  
  
  if(debugoutput&&sNCorrTrackIds) {cout << "Corrflag for track " << correlatedTrackIds[0] <<" after  meta: " <<correlationFlags[0] << endl;}
  

  //check whether MDC information is available in the meta match object
  if(pMetaMatch->getTrkCandInd()>-1) // we have a track candidate!
    {
      //get the track candidate retrieving it by index
      pTrkCand = HPidFL::getMdcTrkCand(pMetaMatch->getTrkCandInd());
    }
  
  if(pTrkCand == NULL)
    {
      Warning("getGeantTrackSet", "no track cand: tracks should be taken"
	      " from HMdcSegs, but this is not implemented");
      Warning("getGeantTrackSet", "no track cand found but index given!");
    }
  
  else
    {
      if(debugoutput) cout << "Track Candidate index:" << pMetaMatch->getTrkCandInd() << endl;

      //get inner and outer mdc segments
      if(pTrkCand->getSeg1Ind()>-1)
	pInnerMdcSeg = HPidFL::getMdcSegSim(pTrkCand->getSeg1Ind());

      if(pTrkCand->getSeg2Ind()>-1)
	pOuterMdcSeg = HPidFL::getMdcSegSim(pTrkCand->getSeg2Ind());


      if(debugoutput) cout << "Inner segment index:" << pTrkCand->getSeg1Ind() <<endl;
      if(debugoutput) cout << "Outer segment index:" << pTrkCand->getSeg2Ind() <<endl;

      Int_t nDriftTimes=0;
      Float_t weight=0;
      nInnerMdcTracks=0;

      if(pInnerMdcSeg != NULL) //we found an inner mdc segment
	{
	  for(i = 0; i < pInnerMdcSeg->getNTracks(); i++) //loop over all contributing tracks ...
	    {  
	      if(pInnerMdcSeg->getTrack(i) < 1 &&
		 pInnerMdcSeg->getTrack(i) != gHades->getEmbeddingRealTrackId() ) continue; // skip cells which contain no valid hit
	      nDriftTimes+=pInnerMdcSeg->getNTimes(i);
	      nInnerMdcTracks++;
	      addToContributingTracks(pInnerMdcSeg->getTrack(i), kSeenInInnerMDC); // ... and append them to the list of particles
	      if(debugoutput) cout << "Geant Track ID number " << i << "in inner Segment is " << pInnerMdcSeg->getTrack(i) << endl;
	    }
	  
	  //assign the weight of the particles making up a segment in MDC...
	  for( i = 0; i < pInnerMdcSeg->getNTracks(); i++)
	    {
	      if(pInnerMdcSeg->getTrack(i) < 1 &&
		 pInnerMdcSeg->getTrack(i) != gHades->getEmbeddingRealTrackId() ) continue; // skip cells which contain no valid hit
	      //...which is the fraction of wires it fired in the total number of wires
	      weight=(Float_t)pInnerMdcSeg->getNTimes(i)/(Float_t)nDriftTimes;

	      if(weight==-1) ::Error("HPidGeantTrackSet::fillFromMetaMatch","negative MDC segment weight");
	      addTrackWeightToDetector(weight,pInnerMdcSeg->getTrack(i),kSeenInInnerMDC);
	    }
	}
      else
	{
	  ::Error("HPidGeantTrackSet::fillFromMetaMatch","No inner segment in this trk cand!");
	}
      if(debugoutput&&sNCorrTrackIds) {cout << "Corrflag for track " << correlatedTrackIds[0] <<" after  mdc1: " <<correlationFlags[0] << endl;}

      
      nDriftTimes=0;
      nOuterMdcTracks=0;
      if(pOuterMdcSeg != NULL) //There is a segment in MDC III and IV
	{
	  //cout << "Outer seg OK" << endl;
	  for(i = 0; i < pOuterMdcSeg->getNTracks(); i++)
	  {
	    if(pOuterMdcSeg->getTrack(i) < 1 &&
	       pOuterMdcSeg->getTrack(i) != gHades->getEmbeddingRealTrackId() ) continue; // skip cells which contain no valid hit
	      nDriftTimes+=pOuterMdcSeg->getNTimes(i);
              nOuterMdcTracks++;
	      addToContributingTracks(pOuterMdcSeg->getTrack(i),kSeenInOuterMDC); // ... append it to the list of particles
	      if(debugoutput) cout << "Geant Track ID number " << i << "in outer Segment is " << pOuterMdcSeg->getTrack(i) << endl;
	    }

	  //assign weights the same way as explained above
	  for(i = 0; i < pOuterMdcSeg->getNTracks(); i++)
	    {
	      if(pOuterMdcSeg->getTrack(i) < 1 &&
		 pOuterMdcSeg->getTrack(i) != gHades->getEmbeddingRealTrackId() ) continue; // skip cells which contain no valid hit
	      weight=(Float_t)pOuterMdcSeg->getNTimes(i)/(Float_t)nDriftTimes;
	      if(weight==-1) ::Error("HPidGeantTrackSet::fillFromMetaMatch","negative MDC segment weight");
	      addTrackWeightToDetector(weight,pOuterMdcSeg->getTrack(i),kSeenInOuterMDC);
	    }
	  
	}                
    }
  if(debugoutput&&sNCorrTrackIds) {cout << "Corrflag for track " << correlatedTrackIds[0] <<" after  mdc2: " <<correlationFlags[0] << endl;}

  //After MDC and META information the RICH is still missing
  if(pMetaMatch->getARichInd(0)>-1)
    {
      pRichHit = HPidFL::getRichHitSim(pMetaMatch->getARichInd(0));
      if(pRichHit==NULL)
	::Error("HPidGeantTrackSet::fillFromMetaMatch","Warning! NO rich hit for given index!");
    }
  
  if(pRichHit != NULL) //There was a ring in RICH
    {
      if(debugoutput) cout << "rich hit found"<<endl;
      i = 0;

      //The weight of the particles contributing to a rich hit are the pads fired by the leptons
      Float_t npads =(Float_t)(pRichHit->weigTrack1 + pRichHit->weigTrack2 + pRichHit->weigTrack3);
      
      //Check for up to three possibly contributing leptons in this ring
      if(pRichHit->weigTrack1 > 0.0 && (pRichHit->track1>0 || pRichHit->track1==gHades->getEmbeddingRealTrackId())) //Important!! Geant starts counting from 1 not from 0
	{
	  nRICHTracks++;

	  //add the RICH to the detectors that have seen this particle...
	  addToContributingTracks(pRichHit->track1, kSeenInRICH); 

	  //...and give it an appropriate weight
	  addTrackWeightToDetector((Float_t)(pRichHit->weigTrack1)/npads,pRichHit->track1,kSeenInRICH);
	  if(debugoutput) cout << "Geant Track ID number 1 in RICH is " << pRichHit->track1 << endl;
	}
      if(pRichHit->weigTrack2 > 0.0 && (pRichHit->track2>0 || pRichHit->track2==gHades->getEmbeddingRealTrackId()))
	{
	  if(debugoutput) cout << "second track id found - booking it" << endl;
	  nRICHTracks++;
	  addToContributingTracks(pRichHit->track2, kSeenInRICH);
	  addTrackWeightToDetector((Float_t)(pRichHit->weigTrack2)/npads,pRichHit->track2,kSeenInRICH);
	  if(debugoutput) cout << "Geant Track ID number 2 in RICH is " << pRichHit->track2 << endl;
	}
      
      if(pRichHit->weigTrack3 > 0.0 && (pRichHit->track3>0 || pRichHit->track3==gHades->getEmbeddingRealTrackId()))
	{
	  if(debugoutput) cout << "third track id found - booking it" << endl;
	  nRICHTracks++;
	  addToContributingTracks(pRichHit->track3, kSeenInRICH);
	  addTrackWeightToDetector((Float_t)(pRichHit->weigTrack3)/npads,pRichHit->track3,kSeenInRICH);
	  if(debugoutput) cout << "Geant Track ID number 3 in RICH is " << pRichHit->track3 << endl;
	}
    }
  if(debugoutput&&sNCorrTrackIds) {cout << "Corrflag for track " << correlatedTrackIds[0] <<" after  rich: " <<correlationFlags[0] << endl;}
  if(debugoutput) cout << "After RICH " << endl;

  /*
  //The lowest bit in the correlation flag comes from the RICH - IPU it is taken out because we don`t need it :-)
  if(pMetaMatch->getARichIPUInd(0)>-1)
    pRichHitIPU = HPidFL::getRichHitIPUSim(pMetaMatch->getARichIPUInd(0));
  
  if(pRichHitIPU != NULL)//There was a online recognized ring
    {
      i = 0;

      if(pRichHitIPU->getWeightTrack1() > 0)
	{
	  nRICHIPUTracks++;
	  
	  //add ipu to the detectors that saw this particle
	  addToContributingTracks(pRichHitIPU->getTrack1(), kSeenInRICHIPU); 

	  //and give it a proper weight
	  addTrackWeightToDetector(pRichHitIPU->getWeightTrack1(),pRichHitIPU->getTrack1(),kSeenInRICHIPU);
	}
      else
	{
	  ::Error("HPidGeantTrackSet::fillFromMetaMatch","First geant track in RICH IPU hit has 0 weight!");
	}
      if(pRichHitIPU->getWeightTrack2() > 0)
	{
	  nRICHIPUTracks++;
	  addToContributingTracks(pRichHitIPU->getTrack2(), kSeenInRICHIPU);
	  addTrackWeightToDetector(pRichHitIPU->getWeightTrack2(),pRichHitIPU->getTrack2(),kSeenInRICHIPU);
	}
      
      if(pRichHitIPU->getWeightTrack3() > 0)
	{
	  nRICHIPUTracks++;
	  addToContributingTracks(pRichHitIPU->getTrack3(), kSeenInRICHIPU);
	  addTrackWeightToDetector(pRichHitIPU->getWeightTrack3(),pRichHitIPU->getTrack3(),kSeenInRICHIPU);
	}
    }
  */

  //Here we start sorting the collected information according to the degree of commonness of the contributing particles
  sortsmart();
  checkPrimaryOrigin();
  shrinkArrays();
  diagnosis();

}

void HPidGeantTrackSet::checkPrimaryOrigin(void)
{
  bIsLepFromPrimary=kFALSE;

  //We check only leptons and we care only for the first particle
  if(GeantPIDs[0]!=2 && GeantPIDs[0]!=3)
    return;
  
  if(ParentIds[0]==0) //Embedded lepton or pluto lepton ??
    {
      bIsLepFromPrimary=kTRUE;
      return;
    }

  if(ParentIds[0]!=1) //No conversion! (but e.g. pi0 dalitz)
    {
      //cout << "Parent: " << ParentIds[0]<< endl;
      if(GrandParents[0]==0)//This parent was a primary and NOT a Geant-produced secondary!
	{
	  bIsLepFromPrimary=kTRUE;
	  //cout << "Primary parent: " << ParentIds[0] << endl;
	}
      else
	{
	  //cout << "Secondary parent: " << ParentIds[0] << endl;
	  bIsLepFromPrimary=kFALSE;
	}
      return;
    }

  if(ParentIds[0]==1) //Gamma-Conversion (parent is gamma)
    {
      if(GrandParentIds[0]==0)//This one was a primary gamma and NOT a Geant-produced secondary gamma!!! (rare case)
	{
	  bIsLepFromPrimary=kTRUE;
	  //cout << "Primary gamma parent!" << endl;
	  return;
	}
      else
	{
	  //The Grandparent can be anything (X) decaying into X+gamma
	  //If this X was a primary (e.g. primary pi0 -> g,g -> e+,e-...) then the current lepton comes from a primary particle.
	  Int_t ggp_medium, ggp_process;
	  Int_t grandGrandParent=0;

	  HGeantKine* pKineGrandParent = getGeantKine(GrandParents[0]); //This is e.g. the pi0
	  if(!pKineGrandParent)
	    {
	      Error("HPidGeantTrackSet::checkPrimaryOrigin(void)","GrandParent has no pointer!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	      return;
	    }
	  pKineGrandParent->getCreator(grandGrandParent, ggp_medium,ggp_process);
	  
	  if(grandGrandParent==0) //The Pi0 had NO grandparent -> So it's primary
	    {
	      //cout << "primary grandparent: " << GrandParentIds[0] <<endl;
	      bIsLepFromPrimary=kTRUE;
	    }
	  else  //The Pi0 had a grandparent -> So it's secondary
	    {
	      //cout << "secondary grandparent: " << GrandParentIds[0] << endl;
	      bIsLepFromPrimary=kFALSE;
	    }
	}
    }
}


Int_t HPidGeantTrackSet::shrinkArrays(void) //OK
{
  correlatedTrackIds.Set(sNCorrTrackIds);
  correlationFlags.Set(sNCorrTrackIds);
  ProcessIds.Set(sNCorrTrackIds);

  ParentIds.Set(sNCorrTrackIds);
  Parents.Set(sNCorrTrackIds);
  GrandParentIds.Set(sNCorrTrackIds);
  GrandParents.Set(sNCorrTrackIds);

  GenInfo.Set(sNCorrTrackIds);
  GenInfo1.Set(sNCorrTrackIds);
  GenInfo2.Set(sNCorrTrackIds);
  GenWeight.Set(sNCorrTrackIds);

  VertexX.Set(sNCorrTrackIds);
  VertexY.Set(sNCorrTrackIds);
  VertexZ.Set(sNCorrTrackIds);

  GeantPIDs.Set(sNCorrTrackIds);
  MediumIds.Set(sNCorrTrackIds);
  
  GeantMomX.Set(sNCorrTrackIds);
  GeantMomY.Set(sNCorrTrackIds);
  GeantMomZ.Set(sNCorrTrackIds);

  ShowerWeights.Set(sNCorrTrackIds);	 
  TOFWeights.Set(sNCorrTrackIds);	 
  RICHWeights.Set(sNCorrTrackIds);	 
  RICHIPUWeights.Set(sNCorrTrackIds); 
  InnerMDCWeights.Set(sNCorrTrackIds);
  OuterMDCWeights.Set(sNCorrTrackIds);

  hasHitInShower.Set(sNCorrTrackIds);
  hasHitInTOF.Set(sNCorrTrackIds);
  return sNCorrTrackIds;
}

Int_t HPidGeantTrackSet::enlargeArrays(void) //OK
{
  Int_t newsize=correlatedTrackIds.GetSize()+1;

  correlatedTrackIds.Set(newsize);
  correlationFlags.Set(newsize);
  ProcessIds.Set(newsize);

  ParentIds.Set(newsize);
  Parents.Set(newsize);

  GrandParents.Set(newsize);
  GrandParentIds.Set(newsize);
  
  GenInfo.Set(newsize);
  GenInfo1.Set(newsize);
  GenInfo2.Set(newsize);
  GenWeight.Set(newsize);
  
  VertexX.Set(newsize);
  VertexY.Set(newsize);
  VertexZ.Set(newsize);

  GeantPIDs.Set(newsize);
  MediumIds.Set(newsize);
  
  GeantMomX.Set(newsize);
  GeantMomY.Set(newsize);
  GeantMomZ.Set(newsize);

  
  ShowerWeights.Set(newsize);	 
  TOFWeights.Set(newsize);	 
  RICHWeights.Set(newsize);	 
  RICHIPUWeights.Set(newsize); 
  InnerMDCWeights.Set(newsize);
  OuterMDCWeights.Set(newsize);

  hasHitInShower.Set(newsize);
  hasHitInTOF.Set(newsize);
  return newsize;
}

//This function checks once again that the sorting algorithm has done its job properly. It makes
//sure that a correlation flag is bigger than its subsequent one except when there is a 
//transition from particles seen in several detectors to particles seen in only one subsystem.
void HPidGeantTrackSet::diagnosis() //OK
{

  //Loop over all but the last particle
    if(getNCorrelatedTrackIds()>0)
    {
	for (UInt_t i =0;i<getNCorrelatedTrackIds()-1;i++)
	{
	    UInt_t j;
	    j= i+1;
	    //Check size of this corrflag and next corrflag
	    if(isSingleDetector(correlationFlags[i])==isSingleDetector(correlationFlags[j]))
	    {
		//Both detectors are single or both are non-single - so the one with index i MUST be bigger than the one with index j
		if(correlationFlags[j]>correlationFlags[i])
		{
		    Error("HPidGeantTrackSet::diagnosis()","Correlation-flag %d (%d) is bigger than flag %d (%d)",
			  j,correlationFlags[j],i,correlationFlags[i]);
		    exit(-1);
		}
	    }

	    //If this particle is NOT seen in more then one detector but the next one IS - then there is something rotten in the
	    //state of Denmark.
	    if(isSingleDetector(correlationFlags[i]) && !isSingleDetector(correlationFlags[j]))
	    {
		Error("HPidGeantTrackSet::diagnosis()","There is a correlated particle sorted BEHIND an uncorrelated one!");
	    }

	    //No particle can occupy two positions in the array
	    if(correlatedTrackIds[i]==correlatedTrackIds[j])
	    {
		Error("HPidGeantTrackSet::diagnosis()","Particle %d (%d) is idential to particle %d (%d)",
		      j,correlatedTrackIds[j],i,correlatedTrackIds[i]);
	    }
	}
    }
}


void HPidGeantTrackSet::Clear(Option_t *opt) //OK
{
  isSorted=kFALSE;
  sNCorrTrackIds=0;

  nRICHTracks=0; 
  nRICHIPUTracks=0;
  nInnerMdcTracks=0;
  nOuterMdcTracks=0;
  nShowerTracks=0;
  nTOFTracks=0;
  bIsLepFromPrimary=kFALSE;

  correlatedTrackIds.Set(0);
  correlationFlags.Set(0);
  ProcessIds.Set(0);

  ParentIds.Set(0);
  Parents.Set(0);

  GrandParentIds.Set(0);
  GrandParents.Set(0);

  GenInfo.Set(0);
  GenInfo1.Set(0);
  GenInfo2.Set(0);
  GenWeight.Set(0);

  VertexX.Set(0);
  VertexY.Set(0);
  VertexZ.Set(0);
    
  GeantPIDs.Set(0);
  MediumIds.Set(0);
  
  GeantMomX.Set(0);
  GeantMomY.Set(0);
  GeantMomZ.Set(0);
  
  
  ShowerWeights.Set(0);	 
  TOFWeights.Set(0);	 
  RICHWeights.Set(0);	 
  RICHIPUWeights.Set(0); 
  InnerMDCWeights.Set(0);
  OuterMDCWeights.Set(0);

  hasHitInShower.Set(0);
  hasHitInTOF.Set(0);

}

void HPidGeantTrackSet::addTrackWeightToDetector(Float_t weight, Int_t trackID, UInt_t detector) //OK
{

  Int_t particleIndex=getParticleIndex(trackID);

  if(particleIndex==-1) {
    ::Error("HPidGeantTrackSet::addTrackWeightToDetector","This particle is not yet part of the track set!");
  }
  else
  {
      switch (detector) {
      case kSeenInRICHIPU:
	  RICHIPUWeights[particleIndex]=weight;
	  break;

      case kSeenInRICH:
	  RICHWeights[particleIndex]=weight;
	  break;

      case kSeenInInnerMDC:
	  InnerMDCWeights[particleIndex]=weight;
	  break;

      case kSeenInOuterMDC:
	  OuterMDCWeights[particleIndex]=weight;
	  break;

      case kSeenInShower:
	  ShowerWeights[particleIndex]=weight;
	  break;

      case kSeenInTOF:
	  TOFWeights[particleIndex]=weight;
	  break;

      default:
	  ::Error("HPidGeantTrackSet::addTrackWeightToDetector","This detector does NOT EXIST!");
	  break;
      }
  }
}

Int_t HPidGeantTrackSet::getParticleIndex(Int_t trackID)//OK
{
  for(Int_t i = 0;  i<sNCorrTrackIds;i++)
    {
      if(correlatedTrackIds[i]==trackID)
	{
	  return i;
	}
    }
  return -1;
}

Bool_t HPidGeantTrackSet::isPartOfTrackSet(Int_t trackID)//OK
{
  if(getParticleIndex(trackID)==-1)
    return kFALSE;
  return kTRUE;
}

TLorentzVector* HPidGeantTrackSet::buildGeantLorentzVector(Int_t iPos) const //OK
{
// Build TLorentzVector from the momentum and the particle mass from the track at position iPos in the array
  TLorentzVector* v = NULL;
  #warning This can be made safer
  if(correlationFlags[iPos] != kTrackNotSet)
    {
      Double_t Energy = TMath::Sqrt(pow(GeantMomX[iPos],2)+
				    pow(GeantMomY[iPos],2)+
				    pow(GeantMomZ[iPos],2)+
				    pow(HPidPhysicsConstants::mass(GeantPIDs[iPos]),2));
      v=new TLorentzVector(GeantMomX[iPos],GeantMomY[iPos],GeantMomZ[iPos],Energy);
    }
  
  return v;
  
}

TVector3* HPidGeantTrackSet::buildGeantMomentumVector(Int_t iPos) const //OK
{
  TVector3* vPtr = NULL;
  vPtr = new TVector3(GeantMomX[iPos],GeantMomY[iPos],GeantMomZ[iPos]);
  return vPtr;
}

Int_t HPidGeantTrackSet::addToContributingTracks(Int_t trackID, UInt_t detector) //OK
{
  Int_t grandparent, parent, medium, process, parentId, grandparentId;
  Int_t gp_process, gp_medium;
  Float_t momx, momy, momz;
  Float_t vertx, verty, vertz;

  Float_t fGgeninfo,fGgeninfo1,fGgeninfo2,fGgenweight;

  HGeantKine* pKineGrandParent = NULL;
  HGeantKine* pKineParent= NULL;

  parent=grandparent=parentId=grandparentId=0;
  medium=process=gp_process=gp_medium=-1;
  fGgeninfo=fGgeninfo1=fGgeninfo2=fGgenweight=-1.0;

  momx=momy=momz=0.0;
  vertx=verty=vertz=-999.0;

  if(!isPartOfTrackSet(trackID)) //Not yet in the array
    {
      if(sNCorrTrackIds==correlatedTrackIds.GetSize()) //Do we need to resize our arrays?
	{ 
	  enlargeArrays();
	}
      
      correlatedTrackIds[sNCorrTrackIds]=trackID;

      //If we are handling a hit in the META detector we add a flag for META and not for TOF/SHOWER in order to 
      //have one flag for the whole META system. We store the system additionally in the flags hasHitInShower, 
      //hasHitInTOF
      if(detector==kSeenInShower)
	{
	  correlationFlags[sNCorrTrackIds]|=kSeenInMETA;
	  hasHitInShower[sNCorrTrackIds]=kTRUE;
	}

      if(detector==kSeenInTOF)
	{
	  correlationFlags[sNCorrTrackIds]|=kSeenInMETA;
	  hasHitInTOF[sNCorrTrackIds]=kTRUE;
	}

      //The other detectores are simply "ored" into the the correlation flag
      if(detector!=kSeenInTOF && detector!=kSeenInShower)
	{
	  correlationFlags[sNCorrTrackIds]|=detector;
	}
      
      //Get the kine object corresponding to our present particle to collect further information
      if(trackID>0)
      {
	  HGeantKine* pKine = getGeantKine(trackID);

	  pKine->getCreator(parent,medium,process);
	  if(parent)
	  {
	      //cout << "parent track: " <<parent<< endl;
	      //cout << "medium: " << medium <<endl;
	      //cout << "process: " << process <<endl;
	  }
	  else
	  {
	      nullparent++;
	  }
	  pKine->getGenerator(fGgeninfo,fGgenweight);
	  pKine->getGenerator(fGgeninfo,fGgeninfo1,fGgeninfo2);
	  pKine->getVertex(vertx,verty,vertz);

	  //Get Parent particle
	  if(parent!=0)
	  {
	      pKineParent = getGeantKine(parent);
	      if(NULL!=pKineParent)
	      {
		  pKineParent->getParticle(parent,parentId);
	      }
	      else
	      {
		  ::Error("HPidGeantTrackSet::addToContributiingTracks","No pKineObject for track id %d",parent);
	      }


	      //Get Grandparent particle
	      if(pKineParent)
	      {
		  pKineParent->getCreator(grandparent,gp_medium,gp_process);
		  if(grandparent!=0)
		  {
		      pKineGrandParent = getGeantKine(grandparent);
		      if(NULL!=pKineGrandParent)
		      {
			  pKineGrandParent->getParticle(grandparent, grandparentId);
		      }
		      else
		      {
			  ::Error("HPidGeantTrackSet::addToContributingTracks","No pKineObject for track id %d",grandparent);
		      }

		  }
	      }
	      //Fill the arrays that contain simulation information about process, medium, true momentum, true pid and parent
	  }

	  Parents[sNCorrTrackIds]=parent;
	  ParentIds[sNCorrTrackIds]=parentId;
	  GrandParents[sNCorrTrackIds]=grandparent;
	  GrandParentIds[sNCorrTrackIds]=grandparentId;

	  /*
	   if(grandparent==0)
	   {
	   cout << "Error condition! " << endl;
	   cout << "Pointer to parent: " <<pKineParent << endl;
	   cout << "Pointer to grandparent: " <<pKineGrandParent << endl;
	   }
	   */

	  MediumIds[sNCorrTrackIds]=medium;
	  ProcessIds[sNCorrTrackIds]=process;

	  GenInfo[sNCorrTrackIds]=fGgeninfo;
	  GenInfo1[sNCorrTrackIds]=fGgeninfo1;
	  GenInfo2[sNCorrTrackIds]=fGgeninfo2;
	  GenWeight[sNCorrTrackIds]=fGgenweight;

	  pKine->getMomentum(momx,momy,momz);
	  GeantMomX[sNCorrTrackIds]=momx;
	  GeantMomY[sNCorrTrackIds]=momy;
	  GeantMomZ[sNCorrTrackIds]=momz;

	  VertexX[sNCorrTrackIds]=vertx;
	  VertexY[sNCorrTrackIds]=verty;
	  VertexZ[sNCorrTrackIds]=vertz;


	  GeantPIDs[sNCorrTrackIds]=pKine->getID();
	  sNCorrTrackIds++;
      }
      else{

	  if(trackID==gHades->getEmbeddingRealTrackId())
	  {
	      Parents[sNCorrTrackIds]=gHades->getEmbeddingRealTrackId();
	      ParentIds[sNCorrTrackIds]=gHades->getEmbeddingRealTrackId();
	      GrandParents[sNCorrTrackIds]=gHades->getEmbeddingRealTrackId();
	      GrandParentIds[sNCorrTrackIds]=gHades->getEmbeddingRealTrackId();


	      MediumIds[sNCorrTrackIds]=gHades->getEmbeddingRealTrackId();
	      ProcessIds[sNCorrTrackIds]=gHades->getEmbeddingRealTrackId();

	      GenInfo[sNCorrTrackIds]=gHades->getEmbeddingRealTrackId();
	      GenInfo1[sNCorrTrackIds]=gHades->getEmbeddingRealTrackId();
	      GenInfo2[sNCorrTrackIds]=gHades->getEmbeddingRealTrackId();
	      GenWeight[sNCorrTrackIds]=gHades->getEmbeddingRealTrackId();

	      GeantMomX[sNCorrTrackIds]=0;
	      GeantMomY[sNCorrTrackIds]=0;
	      GeantMomZ[sNCorrTrackIds]=0;

	      VertexX[sNCorrTrackIds]=0;
	      VertexY[sNCorrTrackIds]=0;
	      VertexZ[sNCorrTrackIds]=0;


	      GeantPIDs[sNCorrTrackIds]=gHades->getEmbeddingRealTrackId();
	      sNCorrTrackIds++;
	  }
	  else {

	      Error("addToContributingTracks()","detected negative trackId other than embedding ID :%i! ",trackID);

	  }
      }
    }
  else
  {
      Int_t index = getParticleIndex(trackID);

      //If we introduce a particle in shower we fill the META flag and not the individual detector flags
      if(detector==kSeenInShower)
      {
	  correlationFlags[index]|=kSeenInMETA;
	  hasHitInShower[index]=kTRUE;
      }

      if(detector==kSeenInTOF)
      {
	  correlationFlags[index]|=kSeenInMETA;
	  hasHitInTOF[index]=kTRUE;
      }

      if(detector!=kSeenInTOF && detector!=kSeenInShower)
      {
	  correlationFlags[index]|=detector;
      }
  }
  return sNCorrTrackIds;
}


// -----------------------------------------------------------------------------

UInt_t HPidGeantTrackSet::getNMetaTracks(void) const //OK
{
  Int_t nDistinctMetaTracks=0;
  //Loop over all correlated track ids  and check whether they are in TOF or in Shower
  
  for(Int_t i = 0;i < sNCorrTrackIds; i++)
    {
      if (wasSeenInDetector(correlationFlags[i], kSeenInMETA)  && hasHitInShower.At(i)>0)
	{
	  nDistinctMetaTracks++;
	}
      //if the above condition was false then there might still be the possibility that the particle
      //at position i was seen in TOF
      else if(wasSeenInDetector(correlationFlags[i], kSeenInMETA) && hasHitInTOF.At(i)>0)
	{
	  nDistinctMetaTracks++;
	}
      //nDistinctMetaTracks holds now the number of tracks that where seen in tof XOR shower
    }
  return nDistinctMetaTracks;
}
// -----------------------------------------------------------------------------

void HPidGeantTrackSet::print(void) const //OK
{
// Print all the members

UInt_t i;

    printf("HPidGeantTrackSet tracks:\nRICH (%d) :", getNRichTracks());
    for(i = 0; i < getNRichTracks(); i++)
      printf(" %3d", getRichTrack(i));
    
    printf("HPidGeantTrackSet tracks:\nRICHIPU (%d) :", getNRichIPUTracks());
    for(i = 0; i < getNRichIPUTracks(); i++)
      printf(" %3d", getRichIPUTrack(i));

    printf("\nMDC  (%d) :", getNInnerMdcTracks());
    for(i = 0; i < getNInnerMdcTracks(); i++)
        printf(" %3d", getInnerMdcTrack(i));

    printf("\nMDC  (%d) :", getNOuterMdcTracks());
    for(i = 0; i < getNOuterMdcTracks(); i++)
        printf(" %3d", getOuterMdcTrack(i));

    printf("\nMeta (%d) :", getNMetaTracks());
    for(i = 0; i < getNMetaTracks(); i++)
        printf(" %3d", getMetaTrack(i));

    printf("\nShower (%d) :", getNShowerTracks());
    for(i = 0; i < getNShowerTracks(); i++)
        printf(" %3d", getShowerTrack(i));

    printf("\nTOF (%d) :", getNTOFTracks());
    for(i = 0; i < getNTOFTracks(); i++)
        printf(" %3d", getTOFTrack(i));

    printf("\n");
}
Int_t HPidGeantTrackSet::findFirstHitInTof(Int_t trackID,Int_t* count,Int_t storeFirstTrack)
{
    //-------------------------------------------------
    // find the first track ID entering the TOF
    // Used to suppress the secondaries created in the
    // TOF itself.
    //        0 = realistic (secondaries included)
    //        1 primary particle is stored
    //        2 (default) the first track number entering the tof in SAME SECTOR is stored
    //        3 as 2 but condition on SAME SECTOR && MOD
    //        4 as 2 but SAME SECTOR && MOD && ROD

    Int_t numTrack=trackID;
    *count=0;

    if(numTrack<=0) return numTrack; // nothing to do for negative track numbers

    //------------------------------------------
    // getting categories
    HLinearCategory* fGeantKineCat = (HLinearCategory*)HPidFL::getCategory(catGeantKine, kFALSE);
    if(!fGeantKineCat){
	::Error("HPidGeantTrackSet::findFirstHitInTof()","Received Zero pointer for catGeantKine!");
        return trackID;
    }
    HLinearCategory* fGeantCat = (HLinearCategory*)gHades->getCurrentEvent()->getCategory(catTofGeantRaw);
    if(!fGeantCat){
	::Error("HPidGeantTrackSet::findFirstHitInTof()","Received Zero pointer for catGeantTof!");
        return trackID;
    }
    //------------------------------------------

    HGeantTof *poldTof, *pnewTof;
    Int_t first=0;

    HGeantKine* kine=(HGeantKine*)fGeantKineCat->getObject(numTrack-1);
    if(kine){

	first=kine->getFirstTofHit();
	if(first!=-1){
	    poldTof=(HGeantTof*)fGeantCat->getObject(first);
            pnewTof=poldTof;
	} else {
	    ::Error("HPidGeantTrackSet::findFirstHitInTof()","No first tof hit!");
	    return numTrack;
	}

    } else {
	::Error("HPidGeantTrackSet::findFirstHitInTof()","Received Zero pointer for kine!");
	return numTrack;
    }
    if(numTrack!=poldTof->getTrack()){
	::Error("HPidGeantTrackSet::findFirstHitInTof()","First tof hit not same trackID!");
	return numTrack;
    }

    //--------------------------------------------------------
    // return the track number for
    // the selected option storeFirstTrack

    //--------------------------------------
    // case 0
    if(storeFirstTrack==0) return numTrack;

    //--------------------------------------
    // case 1
    if(storeFirstTrack==1)
    {   // return track number of primary particle
	// of the given track
	kine=(HGeantKine*)fGeantKineCat->getObject(numTrack-1);
	Int_t parent=kine->getParentTrack();


	kine=HGeantKine::getPrimary(numTrack,fGeantKineCat);
	if(parent>0)(*count)--; // mark only those which change
	return kine->getTrack();
    }

    //--------------------------------------
    // case 2 and 3
    kine=(HGeantKine*)fGeantKineCat->getObject(numTrack-1);
    if(kine->getParentTrack()==0){return numTrack;} // nothing to do

    Int_t s,m,c;
    s= poldTof->getSector();
    m= 21-poldTof->getModule();
    c= 7 -poldTof->getCell();

    first=0;
    Int_t tempTrack=numTrack;
    while((kine=kine->getParent(tempTrack,fGeantKineCat))!=0)
    {
	first=kine->getFirstTofHit();
	if(first!=-1)
	{
	    // track is still in TOF

	    // now we have to check if it is in TOF or TOFINO
	    HGeantTof* gtof=(HGeantTof*)fGeantCat->getObject(first);

	    Int_t s1,m1,c1;
	    s1=m1=c1=0;

	    m1 = 21-gtof->getModule();
	    if(m1>=0)
	    {
		// inside TOF
		s1= gtof->getSector();
		c1= 7-gtof->getCell();

		if(storeFirstTrack==2&&
		   s==s1)
		{   // case 2 :: check only sector
		    tempTrack  =kine->getTrack();
		    pnewTof=gtof;
		    (*count)--;
		}
		if(storeFirstTrack==3&&
		   s==s1&&m==m1)
		{   // case 3 :: check only sector,module
		    tempTrack  =kine->getTrack();
		    pnewTof=gtof;
		    (*count)--;
		}
		else if(storeFirstTrack==4&&
			s==s1&&m==m1&&c==c1)
		{   // case 4 :: check for same rod
		    tempTrack  =kine->getTrack();
		    pnewTof=gtof;
		    (*count)--;
		}
		else {
		    // track has no TOF hit any longer
		    // which fulfills the condition
		    break;
		}

	    } else {
		// track is in TOFINO
		break;
	    }
	}
	else {
	    // track has no TOF hit any longer,
	    // so the previous object was the one we
	    // searched  for
	    break;
	}
    }
    return tempTrack;
}



//Obsolete member functions follow below
/*
Float_t HPidGeantTrackSet::getMetaWeight(UInt_t iIdx)
{

Int_t i = getParticleIndex(getMetaTrack(iIdx));
if(i!=kOutOfBound)
{
  if(ShowerWeights[i]>0)//positive weight in Shower
    return ShowerWeights[i];
  if(TOFWeights[i]>0)//positive weight in TOF
    return TOFWeights[i];
} 
#warning "This is only temporary!"
 return kOutOfBound;
}

Float_t HPidGeantTrackSet::getRichWeight(UInt_t iIdx) const
{
  Int_t i = getParticleIndex(getRichTrack(iIdx));
  if(i!=kOutOfBound)
    return RICHWeights[i];
  return kOutOfBound;
}

Float_t HPidGeantTrackSet::getRichIPUWeight(UInt_t iIdx) const
{
  Int_t i = getParticleIndex(getRichIPUTrack(iIdx));
  if(i!=kOutOfBound)
    return RICHIPUWeights[i];
  return kOutOfBound;
}

Float_t HPidGeantTrackSet::getInnerMdcWeight(UInt_t iIdx) const
{

  Int_t i = getParticleIndex(getInnerMdcTrack(iIdx));
  if(i!=kOutOfBound)
    return InnerMDCWeights[i];
  return kOutOfBound;
}


Float_t HPidGeantTrackSet::getOuterMdcWeight(UInt_t iIdx) const
{

  Int_t i = getParticleIndex(getOuterMdcTrack(iIdx));
  if(i!=kOutOfBound)
    return OuterMDCWeights[i];
  return kOutOfBound;
}
Float_t HPidGeantTrackSet::getTOFWeight(UInt_t iIdx) const
{

  Int_t i = getParticleIndex(getTOFTrack(iIdx));
  if(i!=kOutOfBound)
    return TOFWeights[i];
  return kOutOfBound;
}

Float_t HPidGeantTrackSet::getShowerWeight(UInt_t iIdx) const
{

  Int_t i = getParticleIndex(getShowerTrack(iIdx));
  if(i!=kOutOfBound)
    return ShowerWeights[i];
  return kOutOfBound;
}


void HPidGeantTrackSet::sort(void)
{
  for(Int_t i = 0;i<sNCorrTrackIds;i++)
    {
      Short_t currentMaxPosition=i;
       Int_t currentMax=correlationFlags[i];
       for(Int_t j=i+1;j<sNCorrTrackIds;j++)
 	{
	  if(correlationFlags[j]>currentMax)
	    {
	      currentMaxPosition=j;
	      currentMax=correlationFlags[j];
	    }
	}
      swapVectors(i,currentMaxPosition);
    }
  isSorted=kTRUE;
  //diagnosis();
}

 void HPidGeantTrackSet::sortverbose(void)
{
  cout << "----------------------------------------------------------" <<endl;
  //Int_t wait;
  cout << "We have " << sNCorrTrackIds << endl;
  for(Int_t i = 0;i<sNCorrTrackIds;i++)
    {      
      Short_t currentMaxPosition=i;
      cout << "Starting with position " << i << endl;
	  cout << "current corrflag is : " << correlationFlags[i] << endl;
      for(Int_t j=i+1;j<sNCorrTrackIds;j++)
	{
	  cout << "current competitor at position " << j << " is : " << correlationFlags[j] << endl;
	  if(correlationFlags[j]>currentMaxPosition) 
	    currentMaxPosition=j;
	}
      cout << "Current maximum position: " <<currentMaxPosition << endl;
      swapVectors(i,currentMaxPosition);
    }
  isSorted=kTRUE;
  //cin >> wait;
  //diagnosis();
} 


Bool_t HPidGeantTrackSet::getWeights(Float_t& weightRICHIPU, Float_t& weightRICH, 
				     Float_t& weightInnerMdc,Float_t& weightOuterMdc,
				     Float_t& weightShower, Float_t& weightTOF,
				     Int_t index)
{
  if(index<sNCorrTrackIds)
    {
      weightRICHIPU=RICHIPUWeights[index];
      weightRICH=RICHWeights[index];
      weightInnerMdc=InnerMDCWeights[index];
      weightOuterMdc=OuterMDCWeights[index];
      weightShower=ShowerWeights[index];
      weightTOF=TOFWeights[index];
      return kTRUE;
    }
  else {
    ::Error("HPidGeantTrackSet::getWeights","index of particle is out of bounds");
    return kFALSE;
  }
}



*/
