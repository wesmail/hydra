#include "hpidfl.h"
#include "hruntimedb.h"
#include "hades.h"
#include "hgeantkine.h"
#include "hgeantrich.h"
#include "hgeantmdc.h"
#include "hmdcseg.h"
#include "hmdcsegsim.h"
#include "hhitmatchsim.h"
#include "hrichhitsim.h"
#include "hmdctrkcand.h"
#include "hdebug.h"
#include "hrichcuto.h"
#include "hades.h"
#include "hgeomvector2.h"
#include "hgeomvector.h"

#include "hparset.h"

#include "hgeomtransform.h"
#include "hmdcsizescells.h"
//#include "mptools.h"
#include "hlinearcategory.h"
#include "hmatrixcategory.h"
#include "hmatrixcatiter.h"
#include "htofhit.h"
#include "htofhitsim.h"
#include "htofcluster.h"
#include "htofclustersim.h"
#include "hiterator.h"
#include "hrichraw.h"
#include "hrichcal.h"
#include "richdef.h"
#include "hgeantdef.h"
#include "hpidgeanttrackset.h"
#include "hpidtrackcand.h"
#include "hpidtrackcandsim.h"
#include "hkicktrack.h"
#include "hkicktrackB.h"
#include "hkicktracksim.h"
#include "hsplinetrack.h"
#include "hrichgeometrypar.h"
#include "hrichpad.h"
#include "hrichhitIPU.h"
#include "hrichhitIPUSim.h"
#include "hshowerhittof.h"
#include "hshowerhittoftrack.h"
#include "richdef.h"
#include "kickdef.h"
#include "hmdctrackgdef.h"
#include "tofdef.h"
#include "hmdcdef.h"
#include "showertofinodef.h"
#include "triggerinfodef.h"
#include <TH1.h>
#include <TVector3.h>
#include "piddef.h"
#include "hmetamatch.h"
#include "hdatasource.h"
#include "hrktrackB.h"
///////////////////////////////////////////////////////////
// HPidFL
//
// library of static functions
// look here for auxiliary functions for:
// physics analysis, simulation analysis, ...
///////////////////////////////////////////////////////////

ClassImp(HPidFL)

  static HPidFL* gHPidFL=NULL;

HPidFL::HPidFL()
{
  //getParameterContainers();
}

HPidFL* HPidFL::Instance()
{
  if(gHPidFL)
    return gHPidFL;
  return new HPidFL();
}

HPidFL::~HPidFL(void) 
{

}

TObject* HPidFL::getHitObject(Int_t Index, Cat_t category, Bool_t bWarning)
{
  HCategory* hitCategory = getCategory(category,kTRUE);
  if(hitCategory)
    return hitCategory->getObject(Index);
  else
    {
      if(bWarning)
	::Error("HPidFL::getHitObject","Category %d not found",category);
      return NULL;
    }
}


HCategory* HPidFL::getCategory(Cat_t cat, Bool_t bWarning) 
{
// Find HCategory if gHades->getCurrentEvent() is accessible

HEvent    *pEvent;
HCategory *pCat;

    if((gHades == NULL) || ((pEvent = gHades->getCurrentEvent()) == NULL))
    {
        if(bWarning)
        {
            ::Error("HPidFL::getCategory",
                            "Cannot access current event");
        }

        return NULL;
    }

    if((pCat = pEvent->getCategory(cat)) == NULL)
    {
        if(bWarning)
            ::Error("HPidFL::getCategory", "No category: %d", cat);
    }

    return pCat;
}

Float_t HPidFL::getMdcPhi(Int_t iSector, Float_t fPhiMdc)
{
// Convert MDC's phi angle to the coordinate system used in other detectors

Float_t fPhi = TMath::RadToDeg() * fPhiMdc;

    switch(iSector)
    {
        case 0:
            break;

        case 1:
        case 2:
        case 3:
        case 4:
            fPhi += 60.0f * iSector;
            break;

        default:
            fPhi -= 60.0f;
            break;
    }

    return fPhi;
}

// -----------------------------------------------------------------------------

Float_t HPidFL::getMdcTheta(Float_t fThetaMdc)
{
// Convert MDC's theta angle to the coordinate system used in other detectors

    return  TMath::RadToDeg() * fThetaMdc;
}

// -----------------------------------------------------------------------------

HKickTrack* HPidFL::getKickTrack(Int_t iId, HCategory *pCat)
{
// Static function which returns HKickTrack object corresponding to iId.
// pCat must point on the catKickTrack category or gHades->getCurrentEvent()
// must be accessible.
// In case catKickTrack is category of HKickTrackSim then the object of this
// class is returned.

    if(iId < 0)
      {
	::Error("HPidFL::gezKickTrack","Index <0 not defined!");
	return NULL;
      }
    if((pCat == NULL) && ((pCat = getCategory(catKickTrack)) == NULL))
    {
        ::Error("HPidFL::getKickTrack", "No catKickTrack category");
        return NULL;
    }

    return (HKickTrack *)pCat->getObject(iId);
}
// -----------------------------------------------------------------------------

HKickTrackB* HPidFL::getKickTrackB(Int_t iId, HCategory *pCat)
{
// Static function which returns HKickTrack object corresponding to iId.
// pCat must point on the catKickTrack category or gHades->getCurrentEvent()
// must be accessible.
// In case catKickTrack is category of HKickTrackSim then the object of this
// class is returned.

    if(iId < 0)
      {
	::Error("HPidFL::getKickTrackB","Index <0 not defined!");
	return NULL;
      }
    if((pCat == NULL) && ((pCat = getCategory(catKickTrackB)) == NULL))
    {
        ::Error("HPidFL::getKickTrack", "No catKickTrackB category");
        return NULL;
    }

    return (HKickTrackB *)pCat->getObject(iId);
}
// -----------------------------------------------------------------------------



HKickTrackSim* HPidFL::getKickTrackSim(Int_t iId, HCategory *pCat)
{
// Static function which returns HKickTrackSim object corresponding to iId.
// Returns HKickTrackSim object corresponding to nKickTrackId (if exists)
// pCat must point on the catKickTrack category or gHades->getCurrentEvent()
// must be accessible.

    return (HKickTrackSim *)getKickTrack(iId, pCat);
}
// -----------------------------------------------------------------------------

HSplineTrack* HPidFL::getSplineTrack(Int_t iId, HCategory *pCat)
{
// Static function which returns HSplineTrack object corresponding to iId.
// pCat must point on the catSplineTrack category or gHades->getCurrentEvent()
// must be accessible.
// In case catKickTrack is category of HKickTrackSim then the object of this
// class is returned.

    if(iId < 0)
      {
	::Error("HPidFL::getSplineTrack", "Index <0 not defined!");
        return NULL;
      }

    if((pCat == NULL) && ((pCat = getCategory(catSplineTrack)) == NULL))
      {
        ::Error("HPidFL::getSplineTrack", "No catSplineTrack category");
        return NULL;
      }

    return (HSplineTrack *)pCat->getObject(iId);
}
// -----------------------------------------------------------------------------
HRKTrackB* HPidFL::getRKTrack(Int_t iId, HCategory *pCat)
{
// Static function which returns HRKTrackB  object corresponding to iId.
// pCat must point on the catRKTrackB category or gHades->getCurrentEvent()
// must be accessible.
    if(iId < 0)
      {
	::Error("HPidFL::getRKTrack", "Index <0 not defined!");
        return NULL;
      }

    if((pCat == NULL) && ((pCat = getCategory(catRKTrackB)) == NULL))
      {
        ::Error("HPidFL::getRKTrack", "No catRKTrackB category");
        return NULL;
      }

    return (HRKTrackB *)pCat->getObject(iId);
}
// -----------------------------------------------------------------------------

HMdcTrkCand* HPidFL::getMdcTrkCand(Int_t iId, HCategory *pCat)
{
  // Static function which returns HMdcTrkCand object corresponding to iId.
  // pCat must point on the catMdcTrkCand category or gHades->getCurrentEvent()
  // must be accessible.

    if(iId < 0)
        return NULL;

    if((pCat == NULL) && ((pCat = getCategory(catMdcTrkCand)) == NULL))
    {
        ::Error("HPidFL::getMdcTrkCand", "No catMdcTrkCand category");
        return NULL;
    }

    return (HMdcTrkCand *)pCat->getObject(iId);
}

HMdcSeg* HPidFL::getMdcSeg(Int_t iSector, Int_t iId, HCategory *pCat)
{
// Static function which returns HMdcSeg object corresponding to iId
// in sector iSector.
// pCat must point on the catMdcSeg category or gHades->getCurrentEvent()
// must be accessible.

HLocation  loc;

    if((iSector < 0) || (iSector > 5) || (iId < 0))
    {
      ::Error("HPidFL::getMdcSeg","invalid access-index!");
        return NULL;
    }

    if((pCat == NULL) && ((pCat = getCategory(catMdcSeg)) == NULL))
    {
        ::Error("HPidFL::getMdcSeg", "No catMdcSeg category");
        return NULL;
    }

    loc.setNIndex(3);
    loc[0] = iSector;
    loc[1] = 0;
    loc[2] = iId;

    return (HMdcSeg *)pCat->getObject(loc);
}
// -----------------------------------------------------------------------------

HMdcSegSim* HPidFL::getMdcSegSim(Int_t iSector, Int_t iId, HCategory *pCat)
{
// Static function which returns HMdcSegSim object corresponding to iId
// in sector iSector.
// pCat must point on the catMdcSeg category or gHades->getCurrentEvent()
// must be accessible.

    return (HMdcSegSim *) getMdcSeg(iSector, iId, pCat);
}

// -----------------------------------------------------------------------------

HRichHit* HPidFL::getRichHit(Int_t iId, HCategory *pCat)
{
// Static function which returns HRichHit object corresponding to iId index.
// pCat must point on the catRichHit category or gHades->getCurrentEvent()
// must be accessible.

    if(iId < 0)
        return NULL;

    if((pCat == NULL) && ((pCat = getCategory(catRichHit)) == NULL))
    {
        ::Error("HPidFL::getRichHit", "No catRichHit category");
        return NULL;
    }

    return (HRichHit *)pCat->getObject(iId);
}

// -----------------------------------------------------------------------------

HRichHitSim* HPidFL::getRichHitSim(Int_t iId, HCategory *pCat)
{
// Static function which returns HRichHitSim object corresponding to iId index.
// pCat must point on the catRichHitSim category or gHades->getCurrentEvent()
// must be accessible.
// In the second case catRichHit is used instead of catRichHitSim because
// some inaccurate in RICH code.

    if(iId < 0)
        return NULL;

    if((pCat == NULL) && ((pCat = getCategory(catRichHit)) == NULL))
    {
        ::Error("HPidFL::getRichHit", "No catRichHit category");
        return NULL;
    }

    return (HRichHitSim *)pCat->getObject(iId);
}
// -----------------------------------------------------------------------------

HTofHit* HPidFL::getTofHit(Int_t iId, HCategory *pCat)
{
// Static function which returns HTofHit object corresponding to iId.
// pCat must point on the catTofHit category or gHades->getCurrentEvent()
// must be accessible.
// In case HPidFL isSim() then HTofHitSim is returned.

    if(iId < 0)
        return NULL;

    if((pCat == NULL) && ((pCat = getCategory(catTofHit)) == NULL))
    {
        ::Error("HPidFL::getTofHit", "No catTofHit category");
        return NULL;
    }
    //cout << "Searching for location!" << endl;
HLocation loc;

    loc.setNIndex(3);
    loc[0] = iId / 1000;
    loc[1] = (iId % 1000) / 100;
    loc[2] = iId % 100;
    //cout << loc[0] << " " <<loc[1] << " " <<loc[2] << endl;
    //return (HTofHit *)pCat->getObject(loc);
    return (HTofHit *)pCat->getObject(iId);
}

// -----------------------------------------------------------------------------

HTofHitSim* HPidFL::getTofHitSim(Int_t iId, HCategory *pCat)
{
// Static function which returns HTofHitTrack object corresponding to iId index.
// pCat must point on the catTofHit category or gHades->getCurrentEvent()
// must be accessible.

    return (HTofHitSim *)getTofHit(iId, pCat);
}

// -----------------------------------------------------------------------------
HTofCluster *  HPidFL::getTofCluster(Int_t iId, HCategory *pCat){
//Static function which returns HTofCluster
	if(iId < 0)
        return NULL;

    if((pCat == NULL) && ((pCat = getCategory(catTofCluster)) == NULL))
    {
        ::Error("HPidFL::HTofCluster", "No catTofCluster category");
        return NULL;
    }
	HLocation loc;

    loc.setNIndex(3);
    loc[0] = iId / 1000;
    loc[1] = (iId % 1000) / 100;
    loc[2] = iId % 100;
    return (HTofCluster *)pCat->getObject(iId);

}
// -----------------------------------------------------------------------------
HTofClusterSim*  HPidFL::getTofClusterSim(Int_t iId, HCategory *pCat){
	return (HTofClusterSim*) getTofCluster(iId, pCat);
}
// -----------------------------------------------------------------------------

HShowerHitTof* HPidFL::getShowerHitTof(Int_t iId, HCategory *pCat)
{
// Static function which returns HShowerHitTof object corresponding to iId.
// pCat must point on the catShowerHitTof category or gHades->getCurrentEvent()
// must be accessible.

    if(iId < 0)
        return NULL;

    if((pCat == NULL) && ((pCat = getCategory(catShowerHitTof)) == NULL))
    {
        ::Error("HPidFL::getShowerHitTof",
                "No catShowerHitTof category");
        return NULL;
    }

    return (HShowerHitTof *)pCat->getObject(iId);
}

// -----------------------------------------------------------------------------

HShowerHitTofTrack* HPidFL::getShowerHitTofTrack(Int_t iId,
                    HCategory *pCat)
{
// Static function which returns HShowerHitTofTrack object corresponding to iId.
// pCat must point on the catShowerHitTof category or gHades->getCurrentEvent()
// must be accessible.

    if(iId < 0)
        return NULL;

    if((pCat == NULL) && ((pCat = getCategory(catShowerHitTofTrack)) == NULL))
    {
        ::Error("HPidFL::getShowerHitTofTrack",
                "No catShowerHitTofTrack category");
        return NULL;
    }

    return (HShowerHitTofTrack *)pCat->getObject(iId);
}

// -----------------------------------------------------------------------------

HMdcSeg* HPidFL::getMdcSeg(Int_t iId, HCategory *pCat)
{
  // Static function which returns HMdcSeg object corresponding to linear index iId
  // in category
  // pCat must point on the catMdcSeg category or gHades->getCurrentEvent()
  // must be accessible.
  if(iId < 0)
    {
      ::Error("HPidFL::getMdcSeg","invalid access-index!");
	  return NULL;
    }
  
  if((pCat == NULL) && ((pCat = getCategory(catMdcSeg)) == NULL))
    {
      ::Error("HPidFL::getMdcSeg", "No catMdcSeg category");
      return NULL;
    }
  
  return (HMdcSeg *)pCat->getObject(iId);
  
}
// -----------------------------------------------------------------------------
HMdcSegSim* HPidFL::getMdcSegSim(Int_t iId, HCategory *pCat)
{
  // Static function which returns HMdcSegSim object corresponding to iId
  // in category
  // pCat must point on the catMdcSeg category or gHades->getCurrentEvent()
  // must be accessible.
  
    return (HMdcSegSim*) getMdcSeg(iId);
}

// -----------------------------------------------------------------------------
HRichHitIPU* HPidFL::getRichHitIPU(Int_t iId, HCategory *pCat)
{
  // Static function which returns HRichHitIPU object corresponding to iId
  // in category
  // pCat must point on the catRichHitIPU category or gHades->getCurrentEvent()
  // must be accessible.
  
    if((pCat == NULL) && ((pCat = getCategory(catHardRichHit)) == NULL))
    {
        ::Error("HPidFL::getRichHitIPU", "No catRichHitIPU category");
        return NULL;
    }

    return (HRichHitIPU *)pCat->getObject(iId);
}

// -----------------------------------------------------------------------------
HRichHitIPUSim*  HPidFL::getRichHitIPUSim(Int_t iId, HCategory *pCat)
{
  return (HRichHitIPUSim*) getRichHitIPU(iId);
}

Int_t HPidFL::appendIndex(Int_t index, Int_t* listOfIndices, Int_t lengthOfList)
{
  // compare index to content of given array listOfIndices (dynamically growing!)
  // and decide if it is already stored or not
  // if not stored already, append the index 
  // returns the position in the array where the index was found/appended
  
  Int_t retval=0;

  if(lengthOfList==0)
    {
      //The array is empty still, let's allocate a new one
      enlargeArray(listOfIndices,lengthOfList);
      listOfIndices[0]=index;
      return retval;
    }
  
    Int_t n=0;
    
    //There is at least one index in the array
    while(n<lengthOfList)
      {
	if(index==listOfIndices[n]) return n;
	n++;
      } 

    // After this loop n is equal to the number of indices in the list - we need to enlarge the array
    
    if(n!=lengthOfList) //something is wrong!
      {
	::Error("HPidFL::appendIndex","The position to append the new index is NOT euqat to the array-length");
      }
    else
      {
	enlargeArray(listOfIndices,lengthOfList); //Make new array (larger by 1 field)
	listOfIndices[n]=index; //store index in new slot
	return n;
      }
    
    return -1;
    
}

// -----------------------------------------------------------------------------

HPidCandidate* HPidFL::getPidCandidate(Int_t candIndex, HCategory *pCat)
{
// Returns HPidCandidate object corresponding to candIndex (if exists)
// Works when pCat is set or gHades->getCurrentEvent() is accessible

    if(candIndex < 0)
        return NULL;

    if((pCat == NULL) && ((pCat = getCategory(catPidCandidate)) == NULL))
    {
        ::Error("HPidFL::getCandidate",
                "No catPidCandidate category");
        return NULL;
    }
    
    return (HPidCandidate *)pCat->getObject(candIndex);
}

HPidTrackCand* HPidFL::getTrackCand(Int_t trackCandIndex ,HCategory *pCat)
{
// Returns HPidTrackCand object corresponding to candIndex (if exists)
// Works when pCat is set or gHades->getCurrentEvent() is accessible

  if(trackCandIndex < 0)
      return NULL;

    if((pCat == NULL) && ((pCat = getCategory(catPidTrackCand)) == NULL))
    {
        ::Error("HPidFL::getTrackCand",
                "No catPidTrackCand category");
        return NULL;
    }
    
    return (HPidTrackCand *)pCat->getObject(trackCandIndex);
}

Int_t HPidFL::enlargeArray(Int_t* array, Int_t Length)
{
  //return new length of array
  if(Length==0)
    {
      if(array!=NULL)
	{
	  ::Error("HPidFL::enlargeArray","initialize your empty array-ptr to NULL!");
	  return -1;
	}
    }
  //save old array pointer
  Int_t* temp=array;

  //make new space
  array=new Int_t[Length+1];

  //copy old stuff to new place
  memcpy(array,temp,Length*sizeof(Int_t));
  memset(&array[Length],0,sizeof(Int_t));

  //get rid of old stuff
  delete [] temp;

  return Length+1;
}

Int_t HPidFL::enlargeArray(UInt_t* array, Int_t Length)
{
  //return new length of array
  if(Length==0)
    {
      if(array!=NULL)
	{
	  ::Error("HPidFL::enlargeArray","initialize your empty array-ptr to NULL!");
	  return -1;
	}
    }
  //save old array pointer
  UInt_t* temp=array;

  //make new space
  array=new UInt_t[Length+1];

  //copy old stuff to new place
  memcpy(array,temp,Length*sizeof(UInt_t));
  memset(&array[Length],0,sizeof(UInt_t));

  //get rid of old stuff
  delete [] temp;

  return Length+1;
}


Float_t HPidFL::getKineMomentum(HPidTrackCandSim* pTrackCandSim, UInt_t detectors)
{
  //This function returns the length of the three-momentum vector of the most common particle
  //particle with at least a hit in all <detectors> in the trackSet of the candidate
  HPidGeantTrackSet* pTrackSet = pTrackCandSim->getGeantTrackSet();
  UInt_t corrFlag=pTrackSet->getCorrelationFlag();
  if(detectors & corrFlag == detectors) //All requested detectors where present in the most common track!
    return pTrackSet->getTotalMomentum(0);
  return -1.0f;
}

UInt_t HPidFL::countLeptonsInInnerMdc(HPidGeantTrackSet* theTrackSet,Bool_t verbose)
{
  Int_t leptonCounter=0;
  Int_t wait;
  //Loop over all track ids contributing to this set
  if(verbose) cout <<"This particle has : " << theTrackSet->getNCorrelatedTrackIds() << " track ids " << endl;
  for(UInt_t i=0;i<theTrackSet->getNCorrelatedTrackIds();i++)
    { 
      if(verbose){
	cout <<"The correlation is: "  <<theTrackSet->getCorrelationFlag(i) << "Ckecking now!" <<endl;
	cout << "kSeenInInnerMMDC is coded as: " << kSeenInInnerMDC << endl;
	cout << ((UInt_t)(theTrackSet->getCorrelationFlag(i)) & kSeenInInnerMDC) << endl;
	cout << theTrackSet->wasSeenInDetector(theTrackSet->getCorrelationFlag(i),kSeenInInnerMDC) << endl;
      }
      if(theTrackSet->wasSeenInDetector((UInt_t)(theTrackSet->getCorrelationFlag(i)),kSeenInInnerMDC,verbose)) 
	{//If this particle was seen in the inner MDC
	  if(verbose)cout << "This was in mdc" << endl;
	  if(theTrackSet->getGeantPID(i)==2 || theTrackSet->getGeantPID(i)==3) //This was a lepton!
	    {
	      if(verbose) cout << "This was a lepton in mdc" << endl;
	      leptonCounter++;
	    }
	}
    }
  if(verbose) cin >>wait;
  return leptonCounter;

}

Bool_t HPidFL::hasSuccessfulTracking(HMetaMatch* pMetaMatch)
{
  if(pMetaMatch->getSplineInd()>=0 ||
     pMetaMatch->getKickInd()>=0 ||
     pMetaMatch->getKick123Ind()>=0 ||
     pMetaMatch->getRtInd()>=0)
    return kTRUE;

  return kFALSE;
     
}


Bool_t HPidFL::getGeantMatches(HPidTrackCandSim* giveCandidate,Int_t& nMatchesMdcMeta, Int_t& nMatchesFull,
			       Int_t& nLeptonicMatchesMdcMeta, Int_t& GeantPid)
{
  //cout << "kSeenInMETA is: " << kSeenInMETA << endl;
  Int_t wait;
  nMatchesMdcMeta=0;
  nMatchesFull=0;
  nLeptonicMatchesMdcMeta=0;
  GeantPid=-99;
  HPidGeantTrackSet* theGeantTrackSet = giveCandidate->getGeantTrackSet();
  if(theGeantTrackSet->getCorrelationFlag(0)>=kTrackKick) //This tracks was seen at least in MDC and META
    GeantPid=theGeantTrackSet->getGeantPID(0);

  //Loop over all correlated track ids
  for(UInt_t i = 0;i<theGeantTrackSet->getNCorrelatedTrackIds() ; i++)
    {
      if(theGeantTrackSet->getCorrelationFlag(i)>=kTrackKick)
	{
	  nMatchesMdcMeta++;
	  if(theGeantTrackSet->getGeantPID(i)==2 || theGeantTrackSet->getGeantPID(i)==3)
	    nLeptonicMatchesMdcMeta++;
	}
      if(theGeantTrackSet->getCorrelationFlag(i)>=kTrackKickRICH)
	{
	  nMatchesFull++;
	}
    }

  if(nLeptonicMatchesMdcMeta>0 && GeantPid==-99)
    {
      cout << "DIAGNOSIS REQUIRED!" << endl;
      for(UInt_t j=0;j<theGeantTrackSet->getNCorrelatedTrackIds();j++)
	{
	  cout << "Correlation: " << theGeantTrackSet->getCorrelationFlag(j) << endl;
	  cout << "Pid: " << theGeantTrackSet->getGeantPID(j) << endl;
	  cout << "Track ID: " << theGeantTrackSet->getGeantTrackID(j) << endl;
	}
      cin >> wait;
    }
  return kTRUE;
}

Bool_t HPidFL::isLepOnMirror(HRichHitSim* r,Int_t &nTrkNbOnMirror)
{

    HMatrixCategory * mirrcat = (HMatrixCategory*) getCategory(catRichGeantRaw+2);

    HRichGeometryPar* pGeo = (HRichGeometryPar*) getParamContainer("RichGeometryParameters");
    Float_t fYShift = pGeo->getSectorShift(); // to account for shifted volumes in HGEANT !
    fYShift = fYShift/TMath::Cos( 20. * TMath::DegToRad() );//20 degrees is padplane tilt
    HRichPadTab* pPadsTab = ((HRichGeometryPar*)pGeo)->getPadsPar();
    if (!pPadsTab) return kFALSE;
    HRichPad *pPad = 0;
    if(!r) cout << "No hitsim!" <<endl;

    if (!r) return kFALSE;
    if (r)
    {
 	Int_t xRing=r->getRingCenterX();
	Int_t yRing=r->getRingCenterY();
	Int_t secRing=r->getSector();
	// iterate over hits on mirror and compare hit ring x,y with
	// center of reflected photons, calculate pad hit by photon ctr
	HGeantRichMirror *mirr=0;
	HIterator *iter_mirror = (HIterator *)(mirrcat->MakeIterator("native"));
	iter_mirror->Reset();
	while((mirr=(HGeantRichMirror *)iter_mirror->Next())!=0)
	{
	    
	    Int_t aTrk,aID;
	    mirr->getTrack(aTrk,aID);
	    if(matchRingMirrTracks(r,aTrk,aID)>0) // ring has track of kine obj seen traversing the mirror
	      {

		// project back from mirror to padplane
		Float_t x=mirr->getYRing()/10.;//in mm of the padplane
		Float_t y=mirr->getXRing()/10. + fYShift;
		pPad=pPadsTab->getPad(x, // they are swapped, sic !
				      y );// divide by 10 to account for mm->cm
		if (!pPad) 
		  {cout << "No pad" << endl;
		  return kFALSE;
		  }
		Int_t sec=mirr->getSector();
		Int_t xMirrPhotCM=pPad->getPadX();
		Int_t yMirrPhotCM=pPad->getPadY();

		// check position of ring cntr and photon cntr
		if (
		    TMath::Abs(xMirrPhotCM-xRing) <  2 && 
		    TMath::Abs(yMirrPhotCM-yRing) <  2 && 
		    sec == secRing
		    )
		{
		    nTrkNbOnMirror = aTrk;
		    return kTRUE; // ring stems from lepton that made photons with cm
		                  // in position found by pattern recognition in correct place.
		} 
	    }
	}
    }
    return kFALSE;
}
HGeantKine* HPidFL::getGeantKineObjFromTrkNbFast(Int_t nTrkNb)
{
   HLinearCategory * cat = (HLinearCategory*) getCategory(catGeantKine);
   if(nTrkNb <= 0)
        return NULL;

    if(cat == NULL)
    {
        ::Error("HPidFL::getGeantKineObjFromTrkNbFast", "No catGeantKine category");
        return NULL;
    }

    return (HGeantKine *) cat->getObject(nTrkNb - 1);
}
Int_t HPidFL::getGeantParID(Int_t nTrkNb)
{
    HLinearCategory * cat = (HLinearCategory*) getCategory(catGeantKine);
    Int_t dummy,aID;
    aID=0;
    if (!cat) return aID;
    HGeantKine *kine = getGeantKineObjFromTrkNbFast(nTrkNb);
    if (kine)
    {
	kine->getParticle(dummy,aID);
    }
    return aID;
}
Int_t HPidFL::matchRingMirrTracks(HRichHitSim *r,Int_t trackMirr,Int_t idMirr)
{

    // return weight of photon contribution of track on mirror and in ring
    Int_t t1,t2,t3;
    t1=t2=t3=-1;
    t1=r->track1;t2=r->track2;t3=r->track3;
    // get particle id of 
    Int_t p1,p2,p3;p1=p2=p3=-1;
    if (t1>-1) p1=getGeantParID(t1);
    if (t2>-1) p2=getGeantParID(t2);
    if (t3>-1) p3=getGeantParID(t3);


    if (t1==trackMirr && idMirr==p1) return r->weigTrack1;
    if (t2==trackMirr && idMirr==p2) return r->weigTrack2;
    if (t3==trackMirr && idMirr==p3) return r->weigTrack3;
    return 0; 
}

HParSet* HPidFL::getParamContainer(Char_t * contname)
{
  return (HParSet*)((gHades->getRuntimeDb())->getContainer(contname));
}

Bool_t HPidFL::isDoubleLepOnMirror(HRichHitSim* r,Int_t &nTrkNbOnMirror1, Int_t &nTrkNbOnMirror2)
{

    HMatrixCategory * mirrcat = (HMatrixCategory*) getCategory(catRichGeantRaw+2);

    HRichGeometryPar* pGeo = (HRichGeometryPar*) getParamContainer("RichGeometryParameters");
    Float_t fYShift = pGeo->getSectorShift(); // to account for shifted volumes in HGEANT !
    fYShift = fYShift/TMath::Cos( 20. * TMath::DegToRad() );//20 degrees is padplane tilt
    HRichPadTab* pPadsTab = ((HRichGeometryPar*)pGeo)->getPadsPar();
    if (!pPadsTab) return kFALSE;
    HRichPad *pPad = 0;

    Bool_t kFirstLepton,kSecondLepton;kFirstLepton = kSecondLepton = kFALSE;

    if (!r) {
      cout << "No richhit" << endl;
      return kFALSE;
    }
    if (r)
    {
	Int_t xRing=r->getRingCenterX();
	Int_t yRing=r->getRingCenterY();
	Int_t secRing=r->getSector();
	// iterate over hits on mirror and compare hit ring x,y with
	// center of reflected photons, calculate pad hit by photon ctr
	HGeantRichMirror *mirr=0;
	HIterator *iter_mirror = (HIterator *)(mirrcat->MakeIterator("native"));
	iter_mirror->Reset();
	while((mirr=(HGeantRichMirror *)iter_mirror->Next())!=0)
	{

	  Int_t aTrk,aID;
	  mirr->getTrack(aTrk,aID);
	  if(matchRingMirrTracks(r,aTrk,aID)>0) // ring has track of kine obj seen traversing the mirror
	    {
	      // project back from mirror to padplane
	      Float_t x=mirr->getYRing()/10.;//in mm of the padplane
	      Float_t y=mirr->getXRing()/10. + fYShift;
	      pPad=pPadsTab->getPad(x, // they are swapped, sic !
				    y );// divide by 10 to account for mm->cm
	      //cout << "pPad:" <<pPad << endl;
	      if (!pPad) {
		cout << "No pad" << endl;
		return kFALSE;
	      }
	      //cout << "pad reconstructed" << endl;
	      Int_t sec=mirr->getSector();
	      Int_t xMirrPhotCM=pPad->getPadX();
	      Int_t yMirrPhotCM=pPad->getPadY();
	      
	      if (
		  TMath::Abs(xMirrPhotCM-xRing) <  2 && 
		  TMath::Abs(yMirrPhotCM-yRing) <  2 && 
		  sec == secRing                     &&
		  !kFirstLepton
		  )
		{
		  nTrkNbOnMirror1 = aTrk; 
		  kFirstLepton = kTRUE;
		  //cout << "first lepton found" << endl;
		} 
	      else if (
		       TMath::Abs(xMirrPhotCM-xRing) <  4 && 
		       TMath::Abs(yMirrPhotCM-yRing) <  4 && 
		       sec == secRing                     &&
		       !kSecondLepton
		       ) 
		{
		  nTrkNbOnMirror2 = aTrk;
		  kSecondLepton = kTRUE;
		  //cout << "second lepton found" << endl;
		}
	    }
	}
	if ( kFirstLepton && kSecondLepton ) return kTRUE;
    }

    return kFALSE;
}

HPidParticle* HPidFL::getPidParticle(Int_t iIdx, HCategory* pCat)
{
  if(iIdx < 0)
        return NULL;

    if((pCat == NULL) && ((pCat = getCategory(catPidPart, kFALSE)) == NULL))
    {
        ::Error("HPidParticle::getPidParticle", "No catPidPart category");

        return NULL;
    }

    return (HPidParticle *) pCat->getObject(iIdx);
}
/*
void  HPidFL::transformSectorToLab(HGeomVector &base,HGeomVector &dir, Int_t sector)
{
    // transforms base and direction vector from sector coordinate system into lab system using HMdcSizesCells
  if(!pItself)
    {
      if(!getParameterContainers())
	{
	  ::Error("HPidFL::transformSectorToLab","Initialization of geometry failed");
	}

    }
  

  HMdcSizesCells *fSizesCell=HMdcSizesCells::getExObject();

  if (! fSizesCell )
    {
      ::Error("transformSectorToLab","Geometry not available (HMdcSizesCells)...exiting");
      exit(1);
    }

  HMdcSizesCellsSec& fSec = (*fSizesCell)[sector];
  const HGeomTransform *trans = fSec.getLabTrans();
  
  base = HGeomVector(trans->transFrom(base));
  dir  = HGeomVector(trans->getRotMatrix()*dir);
}
*/ 
/*
Float_t HPidFL::computeDistanceToVertex(HMdcSeg* pSeg)
{
  HGeomVector base;
  HGeomVector dir;
  HEventHeader* pEvtHead =  gHades->getCurrentEvent()->getHeader();
  HVertex& pVertex = pEvtHead->getVertex();
  HGeomVector& pVect = pVertex.getPos();
  transformSegmentToLabStraight(pSeg,base, dir);

  //return MPTools::getDistancePointToStraight(pVect, base, dir);

	if (dir.length() > 0)
    {
      dir*=1/dir.length();
    }
  else
    {
      return -1.;
    }

    Double_t val =
      pow(pVect(0)-base(0),2)+
      pow(pVect(1)-base(1),2)+
      pow(pVect(2)-base(2),2)
      - (
     pow(
         (pVect(0)-base(0))*dir(0)+
         (pVect(1)-base(1))*dir(1)+
         (pVect(2)-base(2))*dir(2)
         ,2)
     );

  if (val<0.)
    {
      return -1.;
    }
  return sqrt(val);

}
*/
/*
void HPidFL::transformSegmentToLabStraight(HMdcSeg* pSeg, HGeomVector &base, HGeomVector &dir)
{
  // calculate for a particle, the equation for the straight of the segment

  if(!getParameterContainers())
    {
      ::Error("HPidFL::transformSegmentToLabStraight", "init of parameters failed");
      exit(-1);
    }

  transformToStraight(pSeg,base,dir);

  if (pSeg->getSec()<0||pSeg->getSec()>5)
    {
      ::Error("transformSegmentToLabStraight", "Cannot transform because sector %i is invalid",pSeg->getSec());
      //pSeg->print();
      return;
    }
  transformSectorToLab(base,dir,pSeg->getSec());
}
*/
// -----------------------------------------------------------------------------
/*
void HPidFL::transformToStraight(HMdcSeg *pSeg, HGeomVector &base,HGeomVector &dir) 
{
//Calculates position and direction vector for a HMdcSeg (HMdcParticle)
// calls transformSegToStraight(Double_t z, Double_t rho, Double_t phi, Double_t theta, HGeomVector &base, HGeomVector &dir)

  if (pSeg->getSec()<0||pSeg->getSec()>5)
	{
	  ::Error("transformToStraight", "Cannot transform because sector %i is invalid",pSeg->getSec());
	  //part->print();
	  return;
	}
  transformSegToStraight(pSeg->getZ(), pSeg->getR(), pSeg->getPhi(), pSeg->getTheta(), base, dir);
}
*/

/*
// -----------------------------------------------------------------------------

void HPidFL::transformSegToStraight(Double_t z, Double_t rho, Double_t phi, Double_t theta,
					     HGeomVector &base, HGeomVector &dir) 
{
  static const Double_t piOver2=TMath::PiOver2();
  
  base.setXYZ(rho*cos(phi+piOver2) , rho * sin(phi+piOver2), z);
  dir.setXYZ(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta));
}

Bool_t HPidFL::getParameterContainers()	
{
  // calls HMdcSizesCells::getExObject()
  // if object didn't exists before initSizesCells is called
  // 
  // expects hades instance, if not, returning kFALSE
  // if init fails, returning kFALSE
  // 
  // otherwise return kTRUE
  
  //pItself= HPidFL::Instance();  

  if (!gHades) 
    {
      ::Warning("HMdcParticlePairFiller::getParameterContainers","gHades is not defined, you have to create an hades instance before calling");
      return kFALSE;
    }  
    
  if (!HMdcSizesCells::getExObject())
    {
      if(! initSizesCells()) 
	{
	  return kFALSE;
	}
    }


  return kTRUE;
     
}
*/
/*
Bool_t HPidFL::initSizesCells()
{
  HMdcSizesCells *fSizesCells;
  
  HDataSource *source = gHades->getDataSource();
  if (!source) 
    {
      gHades->Warning("HMdcParticlePairFiller::initSizesCells()","no data source defined for hades");
      return kFALSE;
    }
  
  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  
  if(!getParamContainer("MdcGeomStruct")) { return kFALSE;}
  if(!getParamContainer("MdcRawStruct")) { return kFALSE;}
  
  rtdb->initContainers(source->getCurrentRunId(),source->getCurrentRefId());
  rtdb->setContainersStatic();
  
  if(!getParamContainer("MdcGeomPar")) { return kFALSE;}
  if(!getParamContainer("MdcLayerGeomPar")) { return kFALSE;}
  if(!getParamContainer("SpecGeomPar")) { return kFALSE;}
  
  if(!getParamContainer("MdcLookupGeom")) { return kFALSE;}
  if(!getParamContainer("MdcLookupRaw")) { return kFALSE;}
  
  if (!rtdb->initContainers(source->getCurrentRunId(),source->getCurrentRefId()))
    {
      ::Error("initSizesCells()","rtdb initContainers failed for currentRunId %i and currentReferenceRunId %i ... return kFALSE!",
	      source->getCurrentRunId(),source->getCurrentRefId());
      return kFALSE;
    }
  rtdb->setContainersStatic();
  
  fSizesCells=HMdcSizesCells::getObject();
  if(!fSizesCells)
    {
      ::Error("initSizesCells()","cannot get HMdcSizesCells object ... exiting!");
      return kFALSE;
    }
  Int_t test=fSizesCells->initContainer();
  if(!test)
    {
      ::Error("initSizesCells()","Init of HMdcSizesCells failed ... exiting!");
      return kFALSE;
    }
  
  return kTRUE;
}
*/
/*
HParSet* HPidFL::getParameterContainer(TString name)
{
  // retrieves parameter container from runtime data base
  // if container doesn't exists NULL is returned.
  // expects:
  //    hades instance
  //
  // if not NULL pointer is returned
  // else the HParSet of the container

  if (!gHades) 
    {
      gROOT->Warning("HMdcParticlePairFiller::getParameterContainer","gHades is not defined, you have to create an hades instance before calling");
      return NULL;
    }  

  HParSet* container = NULL;
  container = gHades->getRuntimeDb()->getContainer((Text_t*)name.Data());
  if (!container)
    {
      gHades->Warning("HMdcParticlePairFiller::getParameterContainer","%s parameter container not available",name.Data());
    }
  return container;
}
*/

/*
Bool_t HPidFL::getGeantMatches(HPidTrackCand* giveCandidate, Int_t& nMatchesRichMdc,
			       Int_t& nMatchesMdcMeta, Int_t& nMatchesFull,
			       Int_t& nLeptonicMatchesFull, Int_t& nLeptonicMatchesMdcMeta, Int_t& GeantPid)
{
  Char_t ListOfTracks[10];
  Short_t IdsOfTracks[10];
  for (Int_t i =0;i< 10;i++){
    ListOfTracks[i]=-99;
    IdsOfTracks[i]=-99;
  }
  nMatchesMdcMeta=0;
  nMatchesRichMdc=0;
  nMatchesFull=0;
  nLeptonicMatchesFull=0;
  nLeptonicMatchesMdcMeta=0;
  GeantPid=-99;

  HGeantKine* theKine;
  HLinearCategory* fGeantKineCat = (HLinearCategory*) getCategory(catGeantKine);
  static HIterator* pIteratorGeantKine = (HIterator *)fGeantKineCat ->MakeIterator("native");

  //This function checks, how many matches have been found in the common track set 
  giveCandidate->getGeantTrackSet()->fillCommonTracks(ListOfTracks,IdsOfTracks,10);
  Int_t i=0;
  while(ListOfTracks[i]>=0)
    {
      if(ListOfTracks[i]==HPidCommonTrack::kTrackFull){
	nMatchesFull++;
	nMatchesRichMdc++;
	nMatchesMdcMeta++;
      }
      if(ListOfTracks[i]==HPidCommonTrack::kTracklet){
	nMatchesRichMdc++;
      }
      if(ListOfTracks[i]==HPidCommonTrack::kTrackKickPlane){
	nMatchesMdcMeta++;
      }

      pIteratorGeantKine->Reset();
      while((theKine=(HGeantKine *)pIteratorGeantKine->Next())!=0)
	{
	  Int_t aTrack, aID;
	  theKine->getParticle(aTrack,aID);
	  if(i==0 && (ListOfTracks[0]==HPidCommonTrack::kTrackKickPlane || 
		      ListOfTracks[0]==HPidCommonTrack::kTrackFull))
	    {
	      if (aTrack==IdsOfTracks[0])
		  GeantPid=aID; //set geant PID to most common track ID if it was seen at least in MDC and META
	    }

	  if (aTrack==IdsOfTracks[i] && (aID==2 ||aID==3))
	    {
	      if(ListOfTracks[i]==HPidCommonTrack::kTrackFull)
		{
		  nLeptonicMatchesFull++;
		  nLeptonicMatchesMdcMeta++;
		}
	      if(ListOfTracks[i]==HPidCommonTrack::kTrackKickPlane)
		{
		  nLeptonicMatchesMdcMeta++;
		}
	      
	    }
	} // end while kine loop
      
      i++;
    }
  return kTRUE;
  }::
*/

//When a track has multiple rings matching to it we pick the best match according to the angular difference of the coordinates
Int_t HPidFL::pickBestRing(HMetaMatch* pMatch, Float_t fMdcPhi, Float_t fMdcTheta)
{
  Int_t bestRingIndex=-1;

  Float_t dist[3];
  Float_t closestDistance=-1.0;

  HRichHit* pRing0=getRichHit(pMatch->getARichInd(0));
  if(!pRing0)
    {
      ::Error("HPidFL::pickBestRing","No first ring found!");
      return -1;
    }
  bestRingIndex=0;


  if(pMatch->getARichInd(1)>-1)
    {
      HRichHit* pRing1=getRichHit(pMatch->getARichInd(1));

      //compare 0 and 1
      dist[0]=TMath::Power(TMath::Abs(pRing0->getPhi()-fMdcPhi),2);
      dist[0]+=TMath::Power(TMath::Abs(pRing0->getTheta()-fMdcTheta),2);

      dist[1]=TMath::Power(TMath::Abs(pRing1->getPhi()-fMdcPhi),2);
      dist[1]+=TMath::Power(TMath::Abs(pRing1->getTheta()-fMdcTheta),2);


      if(dist[1] < dist[0])
	{
	  //second ring matches better than first one
	  bestRingIndex=1;
	  closestDistance=dist[1];
	}
      else
	{
	  closestDistance=dist[0];
	}
    }
  
  
  if(pMatch->getARichInd(2)>-1)
    {
      HRichHit* pRing2=getRichHit(pMatch->getARichInd(2));

      //compare 2 and best of 0/1
      dist[2]=TMath::Power(TMath::Abs(pRing2->getPhi()-fMdcPhi),2);
      dist[2]+=TMath::Power(TMath::Abs(pRing2->getTheta()-fMdcTheta),2);
      if(dist[2]<closestDistance)
	{
	  //third ring matches best!
	  bestRingIndex=2;
	  closestDistance=dist[2];
	}
    }
  return bestRingIndex;
}
