// @(#)$Id: hpidgeanttrackset.cc,v 1.18 2005-09-01 15:13:48 wojcik Exp $
//*-- Author : Marcin Jaskula 11/11/2002
//  Modified : Marcin Jaskula 02/03/2003
//             get rid of TObject
//  Modified : Marcin Jaskula 22/06/2003
//             getGeantKine() improved to work with a filtered branch
//  Modified : Stefano Spataro 18/07/2005
//             libTriggerUtil data members (now protected) changed into member funtions
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidGeantTrackSet                                                          //
//                                                                            //
// Geant tracks matched in HPidTrackCand                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
#include <TArrayI.h>
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
#include <TError.h>
#include "hgeantdef.h"
#include "hgeantkine.h"
#include "hpidphysicsconstants.h"
// -----------------------------------------------------------------------------

ClassImp(HPidGeantTrackSet)

// -----------------------------------------------------------------------------

void HPidGeantTrackSet::reset(void)
{
  // Set all members to 0/-1
  // Set the default size of the arrays
  isSorted = kFALSE;

  sNCorrTrackIds = 0;

  nRICHTracks = 0;
  nRICHIPUTracks = 0;
  nInnerMdcTracks = 0;
  nOuterMdcTracks = 0;
  nShowerTracks = 0;
  nTOFTracks = 0;
  
  correlatedTrackIds.Set(5);
  correlationFlags.Set(5);
  ProcessIds.Set(5);
  Parents.Set(5);
  ParentIds.Set(5);
  GeantPIDs.Set(5);
  MediumIds.Set(5);
  
  GeantMomX.Set(5);
  GeantMomY.Set(5);
  GeantMomZ.Set(5);
  
  
  ShowerWeights.Set(5);	 
  TOFWeights.Set(5);	 
  RICHWeights.Set(5);	 
  RICHIPUWeights.Set(5); 
  InnerMDCWeights.Set(5);
  OuterMDCWeights.Set(5);
 
  hasHitInShower.Set(5);
  hasHitInTOF.Set(5);


  correlatedTrackIds.Reset(-99);
  correlationFlags.Reset(0);
  ProcessIds.Reset(-99);
  Parents.Reset(-99);
  ParentIds.Reset(-99);
  GeantPIDs.Reset(-99);
  MediumIds.Reset(-99);
  
  GeantMomX.Reset(0);
  GeantMomY.Reset(0);
  GeantMomZ.Reset(0);
  
  
  ShowerWeights.Reset(0);	 
  TOFWeights.Reset(0);	 
  RICHWeights.Reset(0);	 
  RICHIPUWeights.Reset(0); 
  InnerMDCWeights.Reset(0);
  OuterMDCWeights.Reset(0);
 
  hasHitInShower.Reset(0);
  hasHitInTOF.Reset(0);

  
  //we might need also a reset of the dynamic data members
}

// -----------------------------------------------------------------------------


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



Bool_t HPidGeantTrackSet::wasSeenInDetector(const UInt_t flag, const UInt_t detector,Bool_t verbose) const
{
  //returns kTRUE when the flag has the detector bit set
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

Short_t HPidGeantTrackSet::getRichTrack(UInt_t iIdx)
{
// Get track with index iIdx from list of RICH tracks
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

Float_t HPidGeantTrackSet::getRichWeight(UInt_t iIdx)
{
  Int_t i = getParticleIndex(getRichTrack(iIdx));
  if(i!=kOutOfBound)
    return RICHWeights[i];
  return kOutOfBound;
}


// -----------------------------------------------------------------------------

Short_t HPidGeantTrackSet::getRichIPUTrack(UInt_t iIdx)
{
// Get track with index n from list of RICH IPU tracks
  if( iIdx >= nRICHIPUTracks) return kOutOfBound;

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

Float_t HPidGeantTrackSet::getRichIPUWeight(UInt_t iIdx)
{
  Int_t i = getParticleIndex(getRichIPUTrack(iIdx));
  if(i!=kOutOfBound)
    return RICHIPUWeights[i];
  return kOutOfBound;
}

// -----------------------------------------------------------------------------

Short_t HPidGeantTrackSet::getInnerMdcTrack(UInt_t iIdx)
{
// Get track with index n from list of Inner Mdc tracks
  if( iIdx >= nInnerMdcTracks) return kOutOfBound;

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

Float_t HPidGeantTrackSet::getInnerMdcWeight(UInt_t iIdx)
{

  Int_t i = getParticleIndex(getInnerMdcTrack(iIdx));
  if(i!=kOutOfBound)
    return InnerMDCWeights[i];
  return kOutOfBound;
}

// -----------------------------------------------------------------------------

Short_t HPidGeantTrackSet::getOuterMdcTrack(UInt_t iIdx)
{
// Get track with index n from list of Outer Mdc tracks
  if( iIdx >= nOuterMdcTracks) return kOutOfBound;

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

Float_t HPidGeantTrackSet::getOuterMdcWeight(UInt_t iIdx)
{

  Int_t i = getParticleIndex(getOuterMdcTrack(iIdx));
  if(i!=kOutOfBound)
    return OuterMDCWeights[i];
  return kOutOfBound;
}

// -----------------------------------------------------------------------------


Short_t HPidGeantTrackSet::getMetaTrack(UInt_t iIdx)
{
  if(nTOFTracks && nShowerTracks) //Hit in both meta detectors!
    {
      //Here we make a common search over both Meta Detectors
      UInt_t counter = 0;

      if(iIdx >= getNMetaTracks()) return kOutOfBound;

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

  if(nTOFTracks>0)
    return getTOFTrack(iIdx);
  if(nShowerTracks>0)
    return getShowerTrack(iIdx);
  else
    return kOutOfBound;
}

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

Short_t HPidGeantTrackSet::getTOFTrack(UInt_t iIdx)
{
// Get track with index n from list of TOF tracks
  if( iIdx >= nTOFTracks) return kOutOfBound;
  UInt_t counter = 0;

  for(Int_t i = 0;i < sNCorrTrackIds; i++)
    {
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

Float_t HPidGeantTrackSet::getTOFWeight(UInt_t iIdx)
{

  Int_t i = getParticleIndex(getTOFTrack(iIdx));
  if(i!=kOutOfBound)
    return TOFWeights[i];
  return kOutOfBound;
}

Short_t HPidGeantTrackSet::getShowerTrack(UInt_t iIdx)
{
// Get track with index n from list of shower tracks
  if( iIdx >= nShowerTracks) return kOutOfBound;

  UInt_t counter = 0;

  for(Int_t i = 0;i < sNCorrTrackIds; i++)
    {
      if (wasSeenInDetector(correlationFlags[i], kSeenInMETA) && hasHitInShower.At(i)>0)
	{
	  if (counter==iIdx) return correlatedTrackIds[i];
	  counter++;
	}
    }
  return kOutOfBound;
}

Float_t HPidGeantTrackSet::getShowerWeight(UInt_t iIdx)
{

  Int_t i = getParticleIndex(getShowerTrack(iIdx));
  if(i!=kOutOfBound)
    return ShowerWeights[i];
  return kOutOfBound;
}


// -----------------------------------------------------------------------------

HGeantKine* HPidGeantTrackSet::getGeantKine(Int_t iTrack, HCategory *pCat)
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

    if((iMax > iTrack - 1)
            && (pKine = (HGeantKine *) pCat->getObject(iTrack - 1)) != NULL)
      {
	//iTrack is the geant track id! Those are sorted (well, they should be) and start with 1
        pKine->getParticle(iT, iP);
        if(iT == iTrack)
	  return pKine;
      }
    
    // fast searching for the proper track. The assumption: tracks are sorted!
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

void HPidGeantTrackSet::swapVectors(Int_t i, Int_t j)
{
  swap(i,j,correlatedTrackIds);
  swap(i,j,correlationFlags);
  swap(i,j,ProcessIds);
  swap(i,j,GeantPIDs);
  swap(i,j,MediumIds);
  swap(i,j,Parents);
  swap(i,j,ParentIds);
  swap(i,j,GeantMomX);
  swap(i,j,GeantMomY);
  swap(i,j,GeantMomZ);
  swap(i,j,RICHIPUWeights);
  swap(i,j,RICHWeights);
  swap(i,j,InnerMDCWeights);
  swap(i,j,OuterMDCWeights);
  swap(i,j,TOFWeights);
  swap(i,j,ShowerWeights);
  swap(i,j,hasHitInTOF);
  swap(i,j,hasHitInShower);
}

void HPidGeantTrackSet::swap(Int_t i, Int_t j, TArrayI& array)
{
  Int_t tmp = array[i];
  array[i]=array[j];
  array[j]=tmp;
}
/*
void HPidGeantTrackSet::swap(Int_t i, Int_t j, UInt_t& array)
{
  UInt_t tmp = array[i];
  array[i]=array[j];
  array[j]=tmp;
}
*/

void HPidGeantTrackSet::swap(Int_t i, Int_t j, TArrayF& array)
{
  Float_t tmp = array[i];
  array[i]=array[j];
  array[j]=tmp;
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

void HPidGeantTrackSet::sortsmart(void)
{
  bringCorrelatedDetectorsToFront();
  sortCorrelatedDetectors();
  sortSingleDetectors();
  isSorted=kTRUE;
}

void HPidGeantTrackSet::bringCorrelatedDetectorsToFront()
{ 
  for(Int_t i = 0;i<sNCorrTrackIds;i++)
    {
      if(!isSingleDetector(correlationFlags[i])) //The current detector is already a correlated one
	{
	  continue;
	}
      else //We have found a single detector in the list - swap it with the first non-single one
	{
	  for(Int_t j=i+1; j<sNCorrTrackIds;j++)
	    {
	      if(!isSingleDetector(correlationFlags[j]))
		{
		  swapVectors(i,j);	      
		}
	    }
	}
    }
}

void HPidGeantTrackSet::sortCorrelatedDetectors()
{
  Short_t currentMaxPosition=0;
  Int_t currentMax=0;
  for(Int_t i = 0;i<sNCorrTrackIds;i++)
    {
      if(isSingleDetector(correlationFlags[i]))
	{
	  break; //The current detector is a single one so we reached the singular detectors
	}
      currentMaxPosition=i;
      currentMax=correlationFlags[i];
      for(Int_t j=i+1;j<sNCorrTrackIds;j++)
	{
	  if(isSingleDetector(correlationFlags[j])) //We have reached the singularly correlated detectors
	    {
	      break;
	    }
	  if(correlationFlags[j]>currentMax )
	    {
	      currentMaxPosition=j;
	      currentMax=correlationFlags[j];
	    }
	}
      swapVectors(i,currentMaxPosition);
    }
}

void HPidGeantTrackSet::sortSingleDetectors()
{ 
  Short_t currentMaxPosition=0;
  Int_t currentMax=0;
  
  for(Int_t i = 0;i<sNCorrTrackIds;i++)
    {
      if(!isSingleDetector(correlationFlags[i])) //We have not yet reached the singularly correlated detectors
	{
	  continue;
	}
      
      currentMaxPosition=i;
      currentMax=correlationFlags[i];
      for(Int_t j=i+1;j<sNCorrTrackIds;j++)
	{
	  if(correlationFlags[j]>currentMax )
	    {
	      currentMaxPosition=j;
	      currentMax=correlationFlags[j];
	    }
	}
      swapVectors(i,currentMaxPosition);
    }
}


Bool_t HPidGeantTrackSet::isSingleDetector(UInt_t flag)
{
  if(flag==kSeenInRICH ||
     flag==kSeenInInnerMDC ||
     flag==kSeenInOuterMDC ||
     flag==kSeenInMETA) //Exactly seen in ONE detector!
    return kTRUE;

  return kFALSE;
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

Bool_t HPidGeantTrackSet::getMomentumComponents(Float_t& momx, Float_t& momy, Float_t& momz, Int_t index)
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

Float_t HPidGeantTrackSet::getTotalMomentum(Int_t index)
{
  if(index>sNCorrTrackIds-1){
    ::Error("HPidGeanTrackSet::getTotalMomentum","requested momentum for particle beyond index!");
    return kFALSE;
  }
  Float_t totalMomentum=GeantMomX[index]*GeantMomX[index]+GeantMomY[index]*GeantMomY[index]+GeantMomZ[index]*GeantMomZ[index];
  return sqrt(totalMomentum);
}

void HPidGeantTrackSet::fillFromMetaMatch(HMetaMatch* pMetaMatch)
{
  Bool_t debugoutput=kFALSE;
  // Each track Id found in any of the hits is added once (and only once)
  // to the list of correlated tracks. We store the degree of 
  // correlation right from the beginning in the datamembers
  // correlatedTrackIds and correlationFlags. Also the geant PID, parent and process ID
  // momentum, medium and system are stored in the corresponding arrays.
  
  // The common tracks are implicitly ranked by their degree of commonness

  //dumpMetaMatch(pMetaMatch);
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
  if(pMetaMatch->getSystem()==0)
    {  
      if(debugoutput) cout << "System is ShowerTofino";
      pShwTof = HPidFL::getShowerHitTofTrack(pMetaMatch->getShowerHitInd());
      if(pShwTof!=NULL)
	{
	  //Here we add the index to the unique list of correlated Ids of this geanttrackset
	  if(pShwTof->getTrack()>=0)
	    {
	      nShowerTracks++;
	      //addToContributingTracks(pShwTof->getTrack(),kSeenInShower);
	      addToContributingTracks(pShwTof->getTrack(),kSeenInShower);
	      addTrackWeightToDetector(1.0,pShwTof->getTrack(),kSeenInShower);
	      if(debugoutput) cout <<"Geant Track id in Shower: " << pShwTof->getTrack()<<endl;
	    }
	  else 
	    {
	      ::Error("HPidGeantTrackSet::fillFromMetaMatch","Shower hit does not have valid geant track id");
	    }
	}
      else 
	{
	  ::Error("HPidGeantTrackSet::fillFromMetaMatch","System 0 marked in MetaMatch object, but no Hit with corresponding index");
	}
    }
  
	if(pMetaMatch->getSystem()==1) {  
		if(debugoutput) cout << "System is Tof" << endl;
		Int_t nClsSize = (Int_t)pMetaMatch->getTofClusterSize();
		//First check what we have from metamach - tofhit or tofcluster		
		Int_t nTrc0=-1;;
		Int_t nTrc1=-1;
		if ( nClsSize <= 0 ){
			////No tofcluster, take htofhitsim category
			pTofHitSim = HPidFL::getTofHitSim(pMetaMatch->getMetaHitInd());
			if(NULL!=pTofHitSim){
				//Get track numbers
				nTrc0 = pTofHitSim ->getNTrack1();
				nTrc1 = pTofHitSim ->getNTrack2();
				if( nTrc0 >= 0 ){
					nTOFTracks++;
                    addToContributingTracks(nTrc0,kSeenInTOF);
                    addTrackWeightToDetector(1.0,nTrc0,kSeenInTOF);
                } 
                if( nTrc1 >= 0 ){
					nTOFTracks++;
                    addToContributingTracks(nTrc1,kSeenInTOF);
                    addTrackWeightToDetector(1.0,nTrc1,kSeenInTOF);
                }
			}
			else{
				::Error("HPidGeantTrackSet::fillFromMetaMatch","System 1 marked in MetaMatch object, but no Hit with corresponding index");
			}
		}
		else if ( nClsSize >= 1 ){
			 //tofclustersim
			pTofClsSim = HPidFL::getTofClusterSim(pMetaMatch->getMetaHitInd());
			if(NULL!=pTofClsSim){
				//Take first set of tracks
				nTrc0 = pTofClsSim ->getNTrack1(0);
				nTrc1 = pTofClsSim ->getNTrack1(1);
				if( nTrc0 >= 0 ){
					nTOFTracks++;
					addToContributingTracks(nTrc0,kSeenInTOF);
					addTrackWeightToDetector(1.0,nTrc0,kSeenInTOF);	
				}	
				if( nTrc1 >= 0 ){
					nTOFTracks++;
					addToContributingTracks(nTrc1,kSeenInTOF);
                    addTrackWeightToDetector(1.0,nTrc1,kSeenInTOF);
				}
				//take second set of tracks
				nTrc0 = pTofClsSim ->getNTrack2(0);
                nTrc1 = pTofClsSim ->getNTrack2(1);
				if( nTrc0 >= 0 ){
					nTOFTracks++;
                    addToContributingTracks(nTrc0,kSeenInTOF);
                    addTrackWeightToDetector(1.0,nTrc0,kSeenInTOF);
                } 
                if( nTrc1 >= 0 ){
					nTOFTracks++;
                    addToContributingTracks(nTrc1,kSeenInTOF);
                    addTrackWeightToDetector(1.0,nTrc1,kSeenInTOF);
                }

			}
			else{
			  ::Error("HPidGeantTrackSet::fillFromMetaMatch","System 1 marked in MetaMatch object, but no Hit with corresponding index");
			}
		}
		else {
		  Warning("fillFromMetaMatch",
			  "What should I do with cluster size=%d?", nClsSize);
		}
		if(nTOFTracks==0){
		  ::Error("fillFromMetaMatch","Should be at least one track in TOF!");
		}
	}

	if(debugoutput) {cout << "Corrflag for track " << correlatedTrackIds[0] <<" after  meta: " <<correlationFlags[0] << endl;}
	//check whether MDC information is available in the meta match object
  if(pMetaMatch->getTrkCandInd()>-1) // we have a track candidate!
    {
      //cout << "trkcandindex is: " <<pMetaMatch->getTrkCandInd();
      pTrkCand = HPidFL::getMdcTrkCand(pMetaMatch->getTrkCandInd());
    }
  
  if(pTrkCand == NULL)
    {
      Warning("getGeantTrackSet", "no track cand: tracks should be taken"
	      " from HMdcSegs, but it is not implemented");
    }
  
  else
    {
      if(debugoutput) cout << "Track Candidate index:" << pMetaMatch->getTrkCandInd() << endl;
      //get inner and outer mdc segments
      if(pTrkCand->getSeg1Ind()>-1)
	pInnerMdcSeg = HPidFL::getMdcSegSim(pTrkCand->getSeg1Ind());
      if(pTrkCand->getSeg2Ind()>-1)
	pOuterMdcSeg = HPidFL::getMdcSegSim(pTrkCand->getSeg2Ind());
      //cout << "Segments found!" << endl;
      if(debugoutput) cout << "Inner segment index:" << pTrkCand->getSeg1Ind() <<endl;
      if(debugoutput) cout << "Outer segment index:" << pTrkCand->getSeg2Ind() <<endl;
      Int_t nDriftTimes=0;
      Float_t weight=0;
      
      nDriftTimes=0;
      weight=0;
      if(pInnerMdcSeg != NULL)
	{
	  for( i = 0; i < pInnerMdcSeg->getNTracks(); i++)
	    {  
	      nDriftTimes+=pInnerMdcSeg->getNTimes(i);
	      addToContributingTracks(pInnerMdcSeg->getTrack(i), kSeenInInnerMDC);
	      if(debugoutput) cout << "Geant Track ID number " << i << "in inner Segment is " << pInnerMdcSeg->getTrack(i) << endl;
	    }
	  nInnerMdcTracks+=pInnerMdcSeg->getNTracks();
	  
	  for( i = 0; i < pInnerMdcSeg->getNTracks(); i++)
	    {
	      weight=(Float_t)pInnerMdcSeg->getNTimes(i)/(Float_t)nDriftTimes;
	      //cout << weight << endl;
	      if(weight==-1) ::Error("HPidGeantTrackSet::fillFromMetaMatch","negative MDC segment weight");
	      addTrackWeightToDetector(weight,pInnerMdcSeg->getTrack(i),kSeenInInnerMDC);
	    }
      	}
      else
	{
	  ::Error("HPidGeantTrackSet::fillFromMetaMatch","No inner segment in this trk cand!");
	}
      if(debugoutput) {cout << "Corrflag for track " << correlatedTrackIds[0] <<" after  mdc1: " <<correlationFlags[0] << endl;}

      nDriftTimes=0;
      if(pOuterMdcSeg != NULL)
	{
	  //cout << "Outer seg OK" << endl;
	  for( i = 0; i < pOuterMdcSeg->getNTracks(); i++)
	    {
	      nDriftTimes+=pOuterMdcSeg->getNTimes(i);
	      addToContributingTracks(pOuterMdcSeg->getTrack(i),kSeenInOuterMDC);
	      if(debugoutput) cout << "Geant Track ID number " << i << "in outer Segment is " << pOuterMdcSeg->getTrack(i) << endl;
	    }
	  nOuterMdcTracks+=pOuterMdcSeg->getNTracks();

	  for( i = 0; i < pOuterMdcSeg->getNTracks(); i++)
	    {
	      weight=(Float_t)pOuterMdcSeg->getNTimes(i)/(Float_t)nDriftTimes;
		if(weight==-1) ::Error("HPidGeantTrackSet::fillFromMetaMatch","negative MDC segment weight");
	      addTrackWeightToDetector(weight,pOuterMdcSeg->getTrack(i),kSeenInOuterMDC);
	    }
	  
	}                
    }
  if(debugoutput) {cout << "Corrflag for track " << correlatedTrackIds[0] <<" after  mdc2: " <<correlationFlags[0] << endl;}
  if(pMetaMatch->getARichInd(0)>-1)
    {
      pRichHit = HPidFL::getRichHitSim(pMetaMatch->getARichInd(0));
      if(pRichHit==NULL)
	::Error("HPidGeantTrackSet::fillFromMetaMatch","Warning! NO rich hit for given index!");
    }
  
  if(pRichHit != NULL)
    {
      if(debugoutput) cout << "rich hit found"<<endl;
      i = 0;
      //cout << "RICH OK" <<endl;
      Float_t npads =(Float_t)(pRichHit->weigTrack1+pRichHit->weigTrack2+pRichHit->weigTrack3);
      
      if(pRichHit->weigTrack1 > 0.0 &&pRichHit->track1>0)
	{
	  nRICHTracks++;
	  addToContributingTracks(pRichHit->track1, kSeenInRICH);
	  addTrackWeightToDetector((Float_t)(pRichHit->weigTrack1)/npads,pRichHit->track1,kSeenInRICH);
	  if(debugoutput) cout << "Geant Track ID number 1 in RICH is " << pRichHit->track1 << endl;
	}
      if(pRichHit->weigTrack2 > 0.0 &&pRichHit->track2>0)
	{
	  if(debugoutput) cout << "second track id found - booking it" << endl;
	  nRICHTracks++;
	  addToContributingTracks(pRichHit->track2, kSeenInRICH);
	  addTrackWeightToDetector((Float_t)(pRichHit->weigTrack2)/npads,pRichHit->track2,kSeenInRICH);
	  if(debugoutput) cout << "Geant Track ID number 2 in RICH is " << pRichHit->track2 << endl;
	}
      
      if(pRichHit->weigTrack3 > 0.0 &&pRichHit->track3>0)
	{
	  if(debugoutput) cout << "third track id found - booking it" << endl;
	  nRICHTracks++;
	  addToContributingTracks(pRichHit->track3, kSeenInRICH);
	  addTrackWeightToDetector((Float_t)(pRichHit->weigTrack3)/npads,pRichHit->track3,kSeenInRICH);
	  if(debugoutput) cout << "Geant Track ID number 3 in RICH is " << pRichHit->track3 << endl;
	}
    }
  if(debugoutput) {cout << "Corrflag for track " << correlatedTrackIds[0] <<" after  rich: " <<correlationFlags[0] << endl;}
  if(debugoutput) cout << "After RICH " << endl;
  //Lowest bit: IPU
  if(pMetaMatch->getARichIPUInd(0)>-1)
    pRichHitIPU = HPidFL::getRichHitIPUSim(pMetaMatch->getARichIPUInd(0));
  
  if(pRichHitIPU != NULL)
    {
      i = 0;
      //cout << "IPU OK" << endl;
      if(pRichHitIPU->getWeightTrack1() > 0)
	{
	  nRICHIPUTracks++;
	  addToContributingTracks(pRichHitIPU->getTrack1(), kSeenInRICHIPU);
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

  //removeLonelyHits();
  sortsmart();
  shrinkArrays();
  diagnosis();
  //cout << "___"<<sNCorrTrackIds <<endl;
  //cout << "___"<<GeantMomY[sNCorrTrackIds-1] << endl;
}

Int_t HPidGeantTrackSet::shrinkArrays(void)
{
  correlatedTrackIds.Set(sNCorrTrackIds);
  correlationFlags.Set(sNCorrTrackIds);
  ProcessIds.Set(sNCorrTrackIds);
  Parents.Set(sNCorrTrackIds);
  ParentIds.Set(sNCorrTrackIds);
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

Int_t HPidGeantTrackSet::enlargeArrays(void)
{
  Int_t newsize=correlatedTrackIds.GetSize()+1;
  correlatedTrackIds.Set(newsize);
  correlationFlags.Set(newsize);
  ProcessIds.Set(newsize);
  Parents.Set(newsize);
  ParentIds.Set(newsize);
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

void HPidGeantTrackSet::removeLonelyHits() //Loop over all correlated track ids and remove those, which
{                                          //where seen in only one detector
  for(UInt_t i=0;i<getNCorrelatedTrackIds();i++)
    {
      if(correlationFlags[i]==kSeenInRICH ||
	 correlationFlags[i]==kSeenInInnerMDC ||
	 correlationFlags[i]==kSeenInOuterMDC ||
	 correlationFlags[i]==kSeenInMETA) //Exactly seen in ONE detector!
	correlationFlags[i]=0; //FIXME! This is only temporary, later we want to know in which detectors we had a loner!
    }
}

void HPidGeantTrackSet::diagnosis()
{
  Int_t wait;

  //Int_t nLeptonsInInnerMDC, nMatchesFull, nMatchesMDCMeta, geantpid, nLeptonicMatchMdcMeta; 
  /*cout << "This trackset has " << getNCorrelatedTrackIds() << endl;
  cout << "Staring diagnosis of geant correlation info!" << endl;
  cout << "Correlation flags: " << endl;
  */
  for (UInt_t i =0;i<getNCorrelatedTrackIds()-1;i++)
    {
      UInt_t j;
      j= i+1;
      if(isSingleDetector(correlationFlags[i])==isSingleDetector(correlationFlags[j])) //Both detectors are single or both are non-single
	{
	  if(correlationFlags[j]>correlationFlags[i])
	    {
	      cout <<"We have a funny ordering of correlations here!" << endl;
	      cout << "Flag "<<j<<" is "<<correlationFlags[j]<<"which is bigger than flag " << i << ": " << correlationFlags[i] << endl;
	      cin>>wait;
	    }
	}
      if(correlatedTrackIds[i]==correlatedTrackIds[j])
	{
	  cout <<"We have the same track id twice here!" << endl;
	  cout << correlatedTrackIds[i] << endl;
	  cin>>wait;
	}
    }
  /*
  for (Int_t z=0;z<sNCorrTrackIds;z++)
    {
      cout << correlationFlags[z] << " ";
    }
    cout << endl;*/
}


void HPidGeantTrackSet::Clear(Option_t *opt)
{
  //cout << "Clearing geant infor!" << endl;
  correlatedTrackIds.Set(0);
  correlationFlags.Set(0);
  ProcessIds.Set(0);
  Parents.Set(0);
  ParentIds.Set(0);
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

void HPidGeantTrackSet::addTrackWeightToDetector(Float_t weight, Int_t trackID, UInt_t detector)
{
  Int_t row = -1;
  Int_t col = -1;
  col=getParticleIndex(trackID);
  if(col==-1)
    ::Error("HPidGeantTrackSet::addTrackWeightToDetector","This particle is not yet part of the track set!");

  switch (detector) {
  case kSeenInRICHIPU:
    row = 0;
    RICHIPUWeights[col]=weight;
    break;

  case kSeenInRICH:
    row = 1;
    RICHWeights[col]=weight;
    break;

  case kSeenInInnerMDC:
    row = 2;
    InnerMDCWeights[col]=weight;
    break;

  case kSeenInOuterMDC:
    row = 3;
    OuterMDCWeights[col]=weight;
    break;

  case kSeenInShower:
    row = 4;
    ShowerWeights[col]=weight;
    break;

  case kSeenInTOF:
    row = 5;
    TOFWeights[col]=weight;
    break;

  default:
    ::Error("HPidGeantTrackSet::addTrackWeightToDetector","This detector does NOT EXIST!");
  break;
  }
}

Int_t HPidGeantTrackSet::getParticleIndex(Int_t trackID)
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

Bool_t HPidGeantTrackSet::isPartOfTrackSet(Int_t trackID) 
{
  if(getParticleIndex(trackID)==-1)
    return kFALSE;
  return kTRUE;
}

TLorentzVector* HPidGeantTrackSet::buildGeantLorentzVector(Int_t iPos) const
{
// Build TLorentzVector from the momentum and the particle mass from the track
  TLorentzVector* v = NULL;
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

TVector3* HPidGeantTrackSet::buildGeantMomentumVector(Int_t iPos) const
{
  TVector3* vPtr = NULL;
  vPtr = new TVector3(GeantMomX[iPos],GeantMomY[iPos],GeantMomZ[iPos]);
  return vPtr;
}

Int_t HPidGeantTrackSet::addToContributingTracks(Int_t trackID, UInt_t detector)
{
  Int_t parent, medium, process,parentId;
  Float_t momx, momy, momz;
  parent=medium=process=parentId=0;
  momx=momy=momz=0.0;

  if(!isPartOfTrackSet(trackID)) //Not yet in the array
    {
      if(sNCorrTrackIds==correlatedTrackIds.GetSize()) //Do we need to resize our arrays?
	{ 
	  enlargeArrays();
	}
      
      correlatedTrackIds[sNCorrTrackIds]=trackID;

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

      if(detector!=kSeenInTOF &&detector!=kSeenInShower)
	{
	  correlationFlags[sNCorrTrackIds]|=detector;
	}
      

      HGeantKine* pKine = getGeantKine(trackID);

      pKine->getCreator(parent,medium,process);
      //Get Parent particle
      HGeantKine* pKineParent = getGeantKine(parent);
      if(NULL!=pKineParent){
          pKineParent->getParticle(parent,parentId);
      }
      Parents[sNCorrTrackIds]=parent;
      ParentIds[sNCorrTrackIds]=parentId;
      MediumIds[sNCorrTrackIds]=medium;
      ProcessIds[sNCorrTrackIds]=process;

      
      pKine->getMomentum(momx,momy,momz);
      GeantMomX[sNCorrTrackIds]=momx;
      GeantMomY[sNCorrTrackIds]=momy;
      GeantMomZ[sNCorrTrackIds]=momz;


      GeantPIDs[sNCorrTrackIds]=pKine->getID();      
      sNCorrTrackIds++;
    }  
  else
    {
      Int_t index = getParticleIndex(trackID);
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

      if(detector!=kSeenInTOF &&detector!=kSeenInShower)
	{
	  correlationFlags[index]|=detector;
	}
    }
  return sNCorrTrackIds;
}


// -----------------------------------------------------------------------------

UInt_t HPidGeantTrackSet::getNMetaTracks(void) const
{
  Int_t nDistinctMetaTracks=0;
  //Loop over all correlated track ids  and check whether they are in TOF and in Shower
  
  for(Int_t i = 0;i < sNCorrTrackIds; i++)
    {
      if (wasSeenInDetector(correlationFlags[i], kSeenInMETA)  && hasHitInShower.At(i)>0)
	{
	  nDistinctMetaTracks++;
	}
      else if(wasSeenInDetector(correlationFlags[i], kSeenInMETA) && hasHitInTOF.At(i)>0)
	{
	  nDistinctMetaTracks++;
	}
	      
    }
  return nDistinctMetaTracks;
}
// -----------------------------------------------------------------------------

void HPidGeantTrackSet::print(void)
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

    printf("\n");
}

